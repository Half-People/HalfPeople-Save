#pragma once
// By Hell People

#include "GameHack.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#include <dwmapi.h>
#pragma comment (lib,"dwmapi.lib")
#include "csgo.hpp"

//#include "CSGO_H.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
bool ImguiDraw;

CONST TCHAR* HNULL(L"");
static HWND GameHwnd;
static HWND ESPHwnd;

struct wnd_rec : public RECT
{
	int width() { return right - left; }
	int height() { return  bottom - top; }
};

LPCWSTR IntToLPCWSTR(int input)
{
	return (LPCWSTR)std::to_string(input).c_str();
}

namespace ESPW {
	class ESP
	{
		struct wnd_rec : public RECT
		{
			int width() { return right - left; }
			int height() { return  bottom - top; }
		};
	public:
		ESP(LPCWSTR window_name);
		~ESP();
		IDirect3DDevice9* m_device = nullptr;
		wnd_rec Game_rect;
	private:
		void create_ESP(LPCWSTR window_name);
		static LRESULT CALLBACK m_win_proc(HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparam)
		{
			switch (msg)
			{
			case WM_DESTROY:
				exit(0);
			default:
				return DefWindowProc(hwnd, msg, wparm, lparam);
			}
		}

		void init_imgui();

		wnd_rec ESP_rect;
		void init_dx9();
		IDirect3D9* m_d3d = nullptr;

		bool m_initialized = false;
	};

	ESP::ESP(LPCWSTR window_name)
	{
		if (window_name == HNULL)
		{
			throw exception("windows name is null");
		}
		create_ESP(window_name);
	}
	ESP::~ESP()
	{
	}

	inline void ESP::create_ESP(LPCWSTR window_name)
	{
		WNDCLASSEX wc;
		wc.cbSize = sizeof WNDCLASSEX;
		wc.lpfnWndProc = m_win_proc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandleA(0);
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = HBRUSH(RGB(0, 0, 0));
		wc.lpszMenuName = L"";
		wc.lpszClassName = L"HellPeople是你爸爸";
		wc.hIconSm = NULL;
		if (!RegisterClassExW(&wc))
		{
			throw exception("window create false");
			cout << "window create false";
		}
		GameHwnd = FindWindowW(NULL, window_name);
		GetWindowRect(GameHwnd, &Game_rect);
		ESPHwnd = CreateWindowA("HellPeople是你爸爸", "By HellPeople", WS_POPUP, Game_rect.left, Game_rect.top, Game_rect.width(), Game_rect.height(), NULL, NULL, NULL, NULL);
		//WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT             // WS_POPUP | WS_VISIBLE
		MARGINS m = { Game_rect.left, Game_rect.top, Game_rect.width(), Game_rect.height() };
		DwmExtendFrameIntoClientArea(ESPHwnd, &m);
		SetLayeredWindowAttributes(ESPHwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
		ShowWindow(ESPHwnd, SW_SHOW);
		init_dx9();
		init_imgui();
	}
	inline void ESP::init_imgui()
	{
	}
	inline void ESP::init_dx9()
	{
		m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		D3DPRESENT_PARAMETERS pp;
		ZeroMemory(&pp, sizeof D3DPRESENT_PARAMETERS);
		pp.Windowed = true;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.BackBufferFormat = D3DFMT_UNKNOWN;
		pp.BackBufferWidth = Game_rect.width();
		pp.BackBufferHeight = Game_rect.height();
		pp.AutoDepthStencilFormat = D3DFMT_D16;
		pp.hDeviceWindow = ESPHwnd;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;//D3DPRESENT_INTERVAL_IMMEDIATE
		if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, ESPHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &m_device)))
		{
			cout << "\n\n m_device !!";
		}
		m_initialized = true;
	}

	class dx_Esp
	{
	public:
		dx_Esp(IDirect3DDevice9* m_device);
		~dx_Esp();
		void begain_renderer();
		void end_renderer();
		void draw_Box(int x0, int y0, int w, int h, long color);
		void draw_Line(int x0, int y0, int x2, int y2, long color);
		void draw_Text(LPCWSTR text, int x, int y, long color);

	private:
		IDirect3DDevice9* d3d_device;
		ID3DXFont* m_font = nullptr;
		ID3DXLine* m_line = nullptr;
	};

	dx_Esp::dx_Esp(IDirect3DDevice9* m_device)
	{
		if (m_device == NULL)
		{
			cout << "\n\n m_device == null";
		}
		d3d_device = m_device;
		if (FAILED(D3DXCreateLine(d3d_device, &m_line)))
		{
			cout << "\n\n d3dline create == false ";
		}
		if (FAILED(D3DXCreateFontW(d3d_device, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, L"Tahoma", &m_font)))
		{
			cout << "\n\n d3dfont create == false ";
		}
	}

	dx_Esp::~dx_Esp()
	{
		if (m_line)
		{
			m_line->Release();
		}
		if (m_font)
		{
			m_font->Release();
		}
	}
	inline void dx_Esp::begain_renderer()
	{
		d3d_device->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.f, 0);
		d3d_device->BeginScene();
	}
	inline void dx_Esp::end_renderer()
	{
		d3d_device->EndScene();
		d3d_device->Present(NULL, NULL, NULL, NULL);
	}
	inline void dx_Esp::draw_Box(int x0, int y0, int w, int h, long color)
	{
		draw_Line(x0, y0, x0 + w, y0, color);
		draw_Line(x0, y0 + h, x0 + w, y0 + h, color);
		draw_Line(x0, y0, x0, y0 + h, color);
		draw_Line(x0 + w, y0, x0 + w, y0 + h, color);
	}
	inline void dx_Esp::draw_Line(int x0, int y0, int x2, int y2, long color)
	{
		D3DXVECTOR2 line[2] = { D3DXVECTOR2(x0,y0),D3DXVECTOR2(x2,y2) };
		m_line->Begin();
		m_line->Draw(line, 2, color);
		m_line->End();
	}
	inline void dx_Esp::draw_Text(LPCWSTR text, int x, int y, long color)
	{
		RECT r = { x,y,x,y };
		m_font->DrawTextW(NULL, text, -1, &r, DT_NOCLIP, color);
	}
}
struct HalfColor// 0- 255
{
	float R;// 0- 255
	float G;// 0- 255
	float B;// 0- 255
	float A;// 0- 255
};

namespace GameHackTook
{
	namespace Imgui
	{
		int WIDTH = 500;
		int HEIGHT = 300;

		bool exit = true;

		HWND window = nullptr;
		WNDCLASSEXA windowClass = {};

		POINTS position = {};
	}
	class ESPUse
	{
	public:
		bool Dx_initialized = false;

		~ESPUse();
		HWND InitializationESP(LPCWSTR Game_WindowsName, bool Init_Dx9);
		void start_rending();
		void End_rending();
		void d3d_Draw_Box(int x0, int y0, int w, int h, long color);
		void d3d_Draw_Line(int x0, int y0, int x2, int y2, long color);
		void d3d_Draw_Text(LPCWSTR text, int x, int y, long color);
		void UpDateWindowsLocation();

		void RunESP(int 人數, HackUse UHack, DWORD64 人与人之间偏移, DWORD64 敌人基址, DWORD64 敌人模块基址, DWORD64 矩阵基址, DWORD64  矩阵模块基址, int r, int g, int b, int a, bool DrawBox, bool DrawLine, bool Auto_aiming, int BoxSize);
		//IDirect3DDevice9* h_device = nullptr;
		IDirect3DDevice9* GetDx();
		wnd_rec ESP_rect;
		wnd_rec Game_rect;
	private:

		D3DXVECTOR2 AimBotSave = { 100000,100000 };

		float rec[4][4];
		DWORD64 Ai_BigLocation = 0x0;
		float AiZ;
		float AiX;
		float AiY;

		int CameraW;
		int CameraH;

		float CameraZ;
		float ZoomSize;
		//HalfDate

		float CameraX;
		float CameraY;
		float CameraY2;

		int LockIndex;

		float recH;
		float recW;

		IDirect3D9* m_d3d = nullptr;

		IDirect3DDevice9* m_device = nullptr;

		ID3DXFont* m_font = nullptr;
		ID3DXLine* m_line = nullptr;

		//		static LRESULT CALLBACK m_win_proc(HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparam)		//Not Imgui
		//		{
		//			switch (msg)
		//			{
		//			case WM_DESTROY:
		//				exit(0);
		//			default:
		//				return DefWindowProc(hwnd, msg, wparm, lparam);
		//			}
		//		}

		static LRESULT CALLBACK m_win_proc(HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparam)		// Imgui
		{
			switch (msg)
			{
			case WM_DESTROY:
				exit(0);
			default:

				return IMGUI_IMPL_API(hwnd, msg, wparm, lparam);//DefWindowProc(hwnd, msg, wparm, lparam);
			}
		}

		void init_dx9();
		void init_dx9_Draw();
	};

	ESPUse::~ESPUse()
	{
		if (m_line)
		{
			m_line->Release();
		}
		if (m_font)
		{
			m_font->Release();
		}
	}

	inline void ESPUse::init_dx9()
	{
		m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		D3DPRESENT_PARAMETERS pp;
		ZeroMemory(&pp, sizeof D3DPRESENT_PARAMETERS);
		pp.Windowed = true;
		pp.SwapEffect = D3DSWAPEFFECT_FLIP;
		pp.BackBufferFormat = D3DFMT_A8R8G8B8;
		pp.BackBufferWidth = Game_rect.width();
		pp.BackBufferHeight = Game_rect.height();
		pp.hDeviceWindow = ESPHwnd;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, ESPHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &m_device)))
		{
			cout << "\n\n m_device !!";
		}
		Dx_initialized = true;
		//h_device = m_device;
		return;
	}
	inline void ESPUse::init_dx9_Draw()
	{
		if (FAILED(D3DXCreateLine(m_device, &m_line)))
		{
			cout << "\n\n d3dline create == false ";
		}
		if (FAILED(D3DXCreateFontW(m_device, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, L"Tahoma", &m_font)))
		{
			cout << "\n\n d3dfont create == false ";
		}
	}
	inline HWND ESPUse::InitializationESP(LPCWSTR Game_WindowsName, bool Init_Dx9)
	{
		if (Game_WindowsName == HNULL)
		{
			throw exception("windows name is null");
		}
		WNDCLASSEX wc;
		wc.cbSize = sizeof WNDCLASSEX;
		wc.lpfnWndProc = m_win_proc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = NULL;//NULL
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = HBRUSH(RGB(0, 0, 0));
		wc.lpszMenuName = L"";
		wc.lpszClassName = L"ESP";
		wc.hIconSm = NULL;
		if (!RegisterClassExW(&wc))
		{
			throw exception("window create false");
			cout << "window create false";
		}
		GameHwnd = FindWindowW(NULL, Game_WindowsName);
		GetWindowRect(GameHwnd, &Game_rect);
		if (Init_Dx9)
		{
			ESPHwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, L"ESP", L"By HellPeople", WS_POPUP | WS_VISIBLE, Game_rect.left, Game_rect.top, Game_rect.width(), Game_rect.height(), NULL, NULL, NULL, NULL); ESPHwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, L"ESP", L"By HellPeople", WS_POPUP | WS_VISIBLE, Game_rect.left, Game_rect.top, Game_rect.width(), Game_rect.height(), NULL, NULL, NULL, NULL);
		}
		else
		{
			return CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, L"ESP", L"By HellPeople", WS_POPUP | WS_VISIBLE, Game_rect.left, Game_rect.top, Game_rect.width(), Game_rect.height(), NULL, NULL, NULL, NULL);
		}

		//WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT

		MARGINS m = { Game_rect.left, Game_rect.top, Game_rect.width(), Game_rect.height() };

		SetLayeredWindowAttributes(ESPHwnd, RGB(0, 0, 0), 255, LWA_ALPHA); //LWA_ALPHA
		DwmExtendFrameIntoClientArea(ESPHwnd, &m);

		ShowWindow(ESPHwnd, SW_SHOW);
		//UpdateWindow(ESPHwnd);

		if (Init_Dx9)
		{
			init_dx9();
			init_dx9_Draw();
		}
		return ESPHwnd;
	}

	inline void ESPUse::start_rending()
	{
		m_device->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.f, 0);
		m_device->BeginScene();
	}
	inline void ESPUse::End_rending()
	{
		m_device->EndScene();
		m_device->Present(NULL, NULL, NULL, NULL);
	}
	inline void ESPUse::d3d_Draw_Box(int x0, int y0, int w, int h, long color)
	{
		d3d_Draw_Line(x0, y0, x0 + w, y0, color);
		d3d_Draw_Line(x0, y0 + h, x0 + w, y0 + h, color);
		d3d_Draw_Line(x0, y0, x0, y0 + h, color);
		d3d_Draw_Line(x0 + w, y0, x0 + w, y0 + h, color);
	}
	inline void ESPUse::d3d_Draw_Line(int x0, int y0, int x2, int y2, long color)
	{
		D3DXVECTOR2 line[2] = { D3DXVECTOR2(x0,y0),D3DXVECTOR2(x2,y2) };
		m_line->Begin();
		m_line->Draw(line, 2, color);
		m_line->End();
	}
	inline void ESPUse::d3d_Draw_Text(LPCWSTR text, int x, int y, long color)
	{
		RECT r = { x,y,x,y };
		m_font->DrawTextW(NULL, text, -1, &r, DT_NOCLIP, color);
	}

	inline void ESPUse::UpDateWindowsLocation()
	{
		GetWindowRect(GameHwnd, &Game_rect);
		SetWindowPos(ESPHwnd, HWND_TOPMOST, Game_rect.left, Game_rect.top, Game_rect.width(), Game_rect.height(), NULL);
	}

	float ABS(float in)
	{
		if (in < 0)
		{
			return in * -1;
		}
		else
		{
			return in;
		}
	}

	inline void ESPUse::RunESP(int 人數, HackUse UHack, DWORD64 人与人之间偏移, DWORD64 敌人基址, DWORD64 敌人模块基址, DWORD64 矩阵基址, DWORD64  矩阵模块基址, int r, int g, int b, int a, bool DrawBox, bool DrawLine, bool Auto_aiming, int BoxSize)
	{
		int hx;
		int hy;
		bool IsMyTeam = true;
		int HP;

		int mousePos_x = (Game_rect.left + Game_rect.width() / 2) * 1;
		int mousePos_y = (Game_rect.top + Game_rect.height() / 2) * 1;
		DWORD64 entity;
		d3d_Draw_Text(L"By HellPeople", 10, 10, D3DCOLOR_RGBA(42, 198, 141, 255));
		//UpdateDate(ESP, dx_ESP);
		ReadProcessMemory(UHack.ProcessHandle, reinterpret_cast<LPVOID>  (矩阵模块基址 + 矩阵基址), &rec[1][1], 64, 0);
		//cout << "\n\n AIX : " << AiX << "\n\n AIY :" << AiY << "\n\n AiZ :" << AiZ;
		//cout << "\n\n\n" << rec[1][1] << "	" << rec[1][2] << "	" << rec[1][3] << "	" << rec[1][4];
		//cout << "\n\n\n" << rec[2][1] << "	" << rec[2][2] << "	" << rec[2][3] << "	" << rec[2][4];
		//cout << "\n\n\n" << rec[3][1] << "	" << rec[3][2] << "	" << rec[3][3] << "	" << rec[3][4];
		//cout << "\n\n\n" << rec[4][1] << "	" << rec[4][2] << "	" << rec[4][3] << "	" << rec[4][4];

		for (size_t i = 0; i < 人數; i++)
		{
			ReadProcessMemory(UHack.ProcessHandle, reinterpret_cast<LPVOID>  (敌人模块基址 + 敌人基址 + (i * 人与人之间偏移)), &Ai_BigLocation, sizeof Ai_BigLocation, nullptr);

			ReadProcessMemory(UHack.ProcessHandle, reinterpret_cast<LPVOID>  (Ai_BigLocation + 0x138), &AiX, sizeof AiX, nullptr);
			ReadProcessMemory(UHack.ProcessHandle, reinterpret_cast<LPVOID>  (Ai_BigLocation + 0x13C), &AiY, sizeof AiY, nullptr);
			ReadProcessMemory(UHack.ProcessHandle, reinterpret_cast<LPVOID>  (Ai_BigLocation + 0x140), &AiZ, sizeof AiZ, nullptr);

			//cout << "\n\n AiZ :" << AiZ;
			CameraW = Game_rect.width() / 2;
			CameraH = Game_rect.height() / 1.97;

			CameraZ = rec[3][1] * AiX + rec[3][2] * AiY + rec[3][3] * AiZ + rec[3][4];
			ZoomSize = 1 / CameraZ;

			///cout << "\n\n local Team :" << UHack.read<float>(敌人模块基址 + hazedumper::netvars::m_Local);

			if (CameraZ > 0)
			{
				CameraX = CameraW + (rec[1][1] * AiX + rec[1][2] * AiY + rec[1][3] * AiZ + rec[1][4]) * ZoomSize * CameraW;
				CameraY = CameraH - (rec[2][1] * AiX + rec[2][2] * AiY + rec[2][3] * AiZ + rec[2][4]) * ZoomSize * CameraH;
				CameraY2 = CameraH - (rec[2][1] * AiX + rec[2][2] * AiY + rec[2][3] * (AiZ + 75) + rec[2][4]) * ZoomSize * CameraH;

				recH = CameraY - CameraY2;
				recW = recH * 0.5;

				if (DrawBox)
				{
					if (IsMyTeam)
					{
						d3d_Draw_Box(CameraX - recW / 2, CameraY2, recW, recH, D3DCOLOR_RGBA(r, g, b, a));

						//d3d_Draw_Box(CameraX - recW / 2, CameraY2, recW / 12, recH * ((float)HP / 100.f), D3DCOLOR_RGBA(255, 0, 0, a));
					}
					else
					{
						d3d_Draw_Box(CameraX - recW / 2, CameraY2, recW, recH, D3DCOLOR_RGBA(255, 0, 0, a));

						//d3d_Draw_Box(CameraX - recW / 2, CameraY2, recW / 12, recH * ((float)HP / 100.f), D3DCOLOR_RGBA(255, 0, 0, a));
					}

					//d3d_Draw_Text(IntToLPCWSTR(ZoomSize), CameraX - recW / 2, CameraY2, D3DCOLOR_RGBA(r, g, b, a));
				}
				if (DrawLine)
				{
					d3d_Draw_Line(CameraW, 0, (CameraX - recW / 20), CameraY2, D3DCOLOR_RGBA(r, g, b, a));
				}
				if (Auto_aiming)
				{
					d3d_Draw_Box(CameraW - BoxSize / 2, CameraH - BoxSize / 2, BoxSize, BoxSize, D3DCOLOR_RGBA(r, g, b, a));
					if (i == LockIndex || LockIndex == -1)
					{
						if (((CameraX - recW / 20) > CameraW - BoxSize / 2) && (CameraX - recW < CameraW + BoxSize / 2) && (CameraY2 + recH > CameraH - BoxSize / 2) && (CameraY2 < CameraH - BoxSize / 2 + BoxSize))//(CameraX - recW / 2 > CameraW - BoxSize / 2) && (CameraX - recW / 2 + recH < CameraH - BoxSize / 2 + BoxSize))
						{
							hx = ((CameraX - recW / 20) - CameraW);
							hy = (CameraY2 - CameraH + (ESP_rect.top + recH * 0.3));   // head 0.2 //body 0.3 //dick 0.5 // foot 0.8
							if (i == LockIndex)
							{
								//GameHackTook::CSGOH::NoBackSeat::MoveMouse(hy * 0.02, hx * -0.02);
								//cout << "\nUpDate";
								MouseMove(hx, hy, 0.1);
							}

							//cout << "\nIndex :" << i << " 	X : " << (float)ABS(hx - CameraW) << "	Y : " << ABS((float)hy - CameraH);
							if (AimBotSave.y > ABS(hy - CameraH) | AimBotSave.x > ABS(hx - CameraW))
							{
								AimBotSave = D3DXVECTOR2(ABS(hx - CameraW), ABS(hy - CameraH));
								LockIndex = i;
								cout << "\nIndex :" << i << " 	X : " << (float)ABS(hx - CameraW) << "	Y : " << ABS((float)hy - CameraH);
							}
						}
						else
						{
							if (i == LockIndex)
							{
								AimBotSave = D3DXVECTOR2(100, 100);
							}
							//
							//LockIndex = -1;
							//cout << "a";
						}
					}
					//cout << "\n\n " << LockIndex;
				}
				else
				{
					AimBotSave = D3DXVECTOR2(10000, 10000);
					LockIndex = -1;
				}
			}

			//cout << "\n CameraZ :" << CameraZ;
			//cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		}
	}
	inline IDirect3DDevice9* ESPUse::GetDx()
	{
		return m_device;
	}
}