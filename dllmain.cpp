#include "includes.h"

#define _CRT_SECURE_NO_WARNINGS；
#pragma warning(disable:4996)；
using namespace std;



void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;
WNDPROC oWndProc;
bool show_imgui_window = false;


//Declare imguiHandler functions
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}


// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
	{
		pDevice = o_pDevice;
		InitImGui(pDevice);

	}


	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		show_imgui_window = !show_imgui_window;
	}


	// drawing stuff

	for (int i = 1; i < 10; i++) {
		//这一行是当前的敌人 从ENTITYLIST中取出
		Ent* curEnt = hack->entList->ents[i].ent;


		if (!hack->CheckValidEnt(curEnt))
			continue;

		D3DCOLOR color;
		//队伍不一样就是敌人，指示线改变颜色
		if (curEnt->team == hack->localEnt->team)
			color = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			color = D3DCOLOR_ARGB(255, 255, 0, 0);

		Vec2 entPos2D;
		// snapline
		if (hack->WorldToScreen(curEnt->pos, entPos2D, windowWidth, windowHeight))



			//从屏幕底部中心 画到屏幕坐标 entPost2D
		DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 5, color);
		//DrawLine(0,0, windowWidth / 2, windowHeight, 10, color);
	}

	// crosshair
	DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2 - 5, 8, 8, D3DCOLOR_ARGB(0, 0, 255, 255));


	if (show_imgui_window) {
		ImGuiMenu();
	}


	// call og function
	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {

	// also need to do a hook on WndProc function 
	//在开发某些应用时，需要替换掉原来系统默认的WndProc，使用我们自己定义的WndProc
	oWndProc = (WNDPROC)SetWindowLongPtr(FindWindow(NULL, "Counter-Strike Source"), GWL_WNDPROC, (LONG_PTR)WndProc);



	// Replace the EndScene function by TrampHook function
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	//创建 hack结构体
	hack = new Hack();

	hack->Init();

	hack->Update();

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {
		hack->Update();

	}

	// unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	// uninject
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}




//OutputDebugString 可格式化版
void OutputDebugPrintf(const char* strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugString(strBuffer);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		//#define  DLL_PROCESS_ATTACH  "DLL_PROCESS_ATTACH"
				//OutputDebugString(_T("已成功注入！ 注入reason : %d",DLL_PROCESS_ATTACH));

		//OutputDebugPrintf(_T("DBG DLL_PROCESS_ATTACH ------------------------ %d ", DLL_PROCESS_ATTACH));
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	return TRUE;
}




