#include "includes.h"

// window height and width
int windowHeight, windowWidth;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
	//io.Fonts->AddFontFromFileTTF("../misc/fonts/Cousine-Regular.ttf",
	//	18.0f,
	//	NULL,
	//	io.Fonts->GetGlyphRangesChineseFull()
	//);
	//io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());


}


bool esp_box = false;
bool esp_snaplines = false;
bool ImGuiMenu() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("css v91 ESP hack ...");
	ImGui::Checkbox(u8"方框透视", &esp_box);      // Edit bools storing our window open/close state
	ImGui::Checkbox(u8"指向线", &esp_snaplines);

	ImGui::End();

	//ImGui::EndFrame(); //开不开这行 都行
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return 1;
}

// callback to be called for each toplevel window
BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);
	if (GetCurrentProcessId() != procId)
		return TRUE;

	window = handle;
	return FALSE;
}

// get process window
HWND GetProcessWindow() {
	window = NULL;

	EnumWindows(enumWind, NULL);

	RECT screen_size;
	GetWindowRect(window, &screen_size);

	//以下是DPI缩放比例
		//float ScreenDPIScale = 1.5f;
		//screen_size.left *= ScreenDPIScale;
		//screen_size.right *= ScreenDPIScale;
		//screen_size.top *= ScreenDPIScale;
		//screen_size.bottom *= ScreenDPIScale;


	windowWidth = screen_size.right - screen_size.left;
	windowHeight = screen_size.bottom - screen_size.top;


	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}

// get get vTable 
bool GetD3D9Device(void** pTable, size_t size) {
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK) {
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return true;
}


