#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <string_view>

//HANDLE mem_handle = NULL;

using namespace std;
using namespace std::literals;
namespace GameHackTook {
	struct HalfDate {
		DWORD64 Dll;		 // 模组基址
		vector<DWORD64> vec; // = { 基址, 第一偏移 ,第二偏移 ,第三偏移, ... ... };
	};

	/// <summary>
	///
	///
	///
	///
	///
	///
	/// </summary>

	class HackUse {
	public:
		int ProcessID;
		HANDLE ProcessHandle = NULL;

	private:
		template <typename T>
		void Write(DWORD64 address, T Input) {
			WriteProcessMemory(ProcessHandle, reinterpret_cast<LPVOID>(address), Input, sizeof T, nullptr);
		}

		template <typename T>
		void read_list(DWORD64 address, T* t, SIZE_T count, HANDLE mem_handle) {
			ReadProcessMemory(ProcessHandle, reinterpret_cast<LPVOID>(address), t, count * (sizeof T), nullptr);
		}

	public:
		template <typename T>
		T read(DWORD64 address) {
			T t;
			ReadProcessMemory(ProcessHandle, reinterpret_cast<LPVOID>(address), &t, sizeof T, nullptr);
			return t;
		}

		DWORD64 getOffset(DWORD64 BaseAddress, SIZE_T ModuleSize, const char* Signature, const char* SignatureMask) {
			if (!BaseAddress)
				return 0;

			BOOL Found = FALSE;
			int Step = 0;
			int SignatureSize = strlen(SignatureMask);
			BYTE* BytesRead = (BYTE*)calloc(SignatureSize + 1, sizeof(BYTE));
			do {
				uintptr_t CurrentAddress = BaseAddress + Step;
				if (ReadProcessMemory(ProcessHandle, (BYTE*)CurrentAddress, BytesRead, SignatureSize, NULL)) {
					Found = TRUE;
					for (int i = 0; i < SignatureSize; i++)
						if (SignatureMask[i] == 'x' && (BYTE)Signature[i] != BytesRead[i]) {
							Found = FALSE;
							break;
						}
				}
				Step++;
			} while (Found == FALSE && Step < ModuleSize);
			if (Found) {
				char TempBuffer[17] = ""; //17 = Two octets + 1 (to support 64bit addresses)
				for (int i = SignatureSize - 1; i >= 0; i--)
					if (SignatureMask[i] == '?') {
						char CurrentByte[3] = "";
						sprintf(CurrentByte, "%.2X", (uintptr_t)(BytesRead[i]));
						strcat(TempBuffer, CurrentByte);
					}
				free(BytesRead);
				return (uintptr_t)(strtoul(TempBuffer, NULL, 16));
			}
			free(BytesRead);
			return 0;
		}

		MODULEENTRY32 GetModuleBaseAddressV2(CONST TCHAR* Dll_Name) {
			if (!ProcessID)
				return { 0 };
			MODULEENTRY32 sEntryModule = { 0 };
			HANDLE hModuleList = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
			if (hModuleList != INVALID_HANDLE_VALUE) {
				sEntryModule.dwSize = sizeof(sEntryModule);
				if (Module32First(hModuleList, &sEntryModule))
					do {
						if (!_wcsicmp(Dll_Name, sEntryModule.szModule)) {
							CloseHandle(hModuleList);
							return sEntryModule;
						}
					} while (Module32Next(hModuleList, &sEntryModule));
			}
			CloseHandle(hModuleList);
			return { 0 };
		}
		//
		// MODULEENTRY32 Module = GetModule(ProcessId, L"crossfire.exe");
		// DWORD64 ModuleAddr = (DWORD64)Module.modBaseAddr;
		// size_t ModuleSize = (DWORD64)Module.modBaseSize;
		//
		//

		void InitializationProcess(CONST TCHAR* name) // 初始化进程PID   ( 进程名称  : L"Hpeople.exe" )
		{
			PROCESSENTRY32 pro;
			pro.dwSize = sizeof pro;

			do {
				Sleep(100);
				HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

				for (Process32First(_handle, &pro); Process32Next(_handle, &pro);) {
					if (wcscmp(pro.szExeFile, name) == 0) {
						ProcessID = pro.th32ProcessID;
						ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);
						break;
					}
				}
			} while (ProcessID == NULL);
		}

		DWORD64 GetModuleBaseAddress(CONST TCHAR* Dll_Name) //  获取模组基址 ( 模组名称 : L"HalfPeople.dll" , 模组基址输出 : )
		{
			HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);

			MODULEENTRY32 mod;
			mod.dwSize = sizeof mod;

			for (Module32First(_handle, &mod); Module32Next(_handle, &mod);) {
				if (wcscmp(mod.szModule, Dll_Name) == 0) {
					return (DWORD64)mod.modBaseAddr;
				}
			}
		}

		float x = 100;

		template <typename T>
		void GetAndSetMemoryData(HalfDate Date, T& Out, T In) {
			for (size_t i = 0; i < Date.vec.size(); i++) {
				if (i + 1 == Date.vec.size()) {
					//Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
					Out = read<T>(Date.Dll + Date.vec[i]);
					//cout << "		\n\n	" << i << "		" << (DWORD64*)(Date.Dll + Date.vec[i]) << "	" << Date.vec[i] << "	" << Date.Dll;
					WriteProcessMemory(ProcessHandle, reinterpret_cast<LPVOID>(Date.Dll + Date.vec[i]), (LPCVOID)&In, sizeof(In), nullptr);
				}
				else
					Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
			}
		}

		template <typename T>
		void SetMemoryData(HalfDate Date, T In) {
			for (size_t i = 0; i < Date.vec.size(); i++) {
				if (i + 1 == Date.vec.size()) {
					//Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
					WriteProcessMemory(ProcessHandle, reinterpret_cast<LPVOID>(Date.Dll + Date.vec[i]), (LPCVOID)&In, sizeof(In), nullptr);
					//cout << "		\n\n	" << i << "		" << (DWORD64*)(Date.Dll + Date.vec[i]) << "	" << Date.vec[i] << "	" << Date.Dll;
				}
				else
					Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
			}
		}
		template <typename T>
		void GetMemoryData(HalfDate Date, T& Out) {
			for (size_t i = 0; i < Date.vec.size(); i++) {
				if (i + 1 == Date.vec.size()) {
					//Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
					Out = read<T>(Date.Dll + Date.vec[i]);
					//cout << "		\n\n	" << i << "		" << (DWORD64*)(Date.Dll + Date.vec[i]) << "	" << Date.vec[i] << "	" << Date.Dll;
				}
				else
					Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
			}
		}

		template <typename T>
		void _GetMemoryData(HalfDate Date, T& Out) {
			for (size_t i = 0; i < Date.vec.size(); i++) {
				if (i + 1 == Date.vec.size()) {
					//Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
					Out = read<T>(Date.Dll - Date.vec[i]);
					//cout << "		\n\n	" << i << "		" << (DWORD64*)(Date.Dll + Date.vec[i]) << "	" << Date.vec[i] << "	" << Date.Dll;
				}
				else
					Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
			}
		}

		template <typename T>
		void _SetMemoryData(HalfDate Date, T In) {
			for (size_t i = 0; i < Date.vec.size(); i++) {
				if (i + 1 == Date.vec.size()) {
					//Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
					WriteProcessMemory(ProcessHandle, reinterpret_cast<LPVOID>(Date.Dll - Date.vec[i]), (LPCVOID)&In, sizeof(In), nullptr);
					//cout << "		\n\n	" << i << "		" << (DWORD64*)(Date.Dll + Date.vec[i]) << "	" << Date.vec[i] << "	" << Date.Dll;
				}
				else
					Date.Dll = read<DWORD64>(Date.Dll + Date.vec[i]);
			}
		}
	};
};
