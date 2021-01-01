#pragma once


//bool init = false;
//extern bool show_imgui_window ;

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

bool GetD3D9Device(void** pTable, size_t size);

static HWND window;
HWND GetProcessWindow();

void InitImGui(LPDIRECT3DDEVICE9 pDevice);
bool ImGuiMenu();

//extern bool show_imgui_window = false;

extern int windowHeight, windowWidth;

extern LPDIRECT3DDEVICE9 pDevice;
