#pragma once
#include <Windows.h>
// v 1.0    By HalfPeople
#include <iostream>
using namespace std;

/*

Demo :

#include <Windows.h>
#include "HalfPeople Registry.h"
#include <iostream>
#include <string>
#include <vector>
#include <direct.h>

using namespace std;

int main()
{
	WCHAR Test[MAX_PATH] = { '\0' };
	char Test_Out[MAX_PATH];
	string save;

	HackTook::registry::Write(NULL, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"NMSL", REG_SZ, L"D:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQ.exe");

	HackTook::registry::Read(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Steam", NULL, Test);
	//Test[i + 2] != WCHAR("-")

	for (size_t i = 0; i < 520; i++)
	{
		Test_Out[i] = Test[i + 1];
	}

	save = Test_Out;
	int pos = save.rfind("\\s");
	if (pos != string::npos)
		save = save.substr(0, pos + 1);

	cout << "\n\n\n cout : " << save;
	cout << "\n\n\n";
	system("pause");
}

*/

struct HpWchar
{
	WCHAR save[MAX_PATH] = { '\0' };
};

namespace HackTook
{
	string GetPath(HpWchar HpWchar)
	{
		char Test_Out[MAX_PATH];
		string save;
		for (size_t i = 0; i < 520; i++)
		{
			Test_Out[i] = HpWchar.save[i + 1];
		}
		save = Test_Out;
		int pos = save.rfind("\\");
		if (pos != string::npos)
			save = save.substr(0, pos);
		return save;
	}

	namespace registry
	{
		//RootPath is 	HKEY_CLASSES_ROOT , HKEY_CURRENT_USER ,	HKEY_LOCAL_MACHINE	, HKEY_CURRENT_CONFIG
		void Write(HKEY RootPath, const wchar_t* Path, const wchar_t* Name, DWORD Type, const wchar_t* material)
		{
			if (RootPath == NULL)
			{
				RootPath = HKEY_LOCAL_MACHINE;
			}
			//LPCTSTR lpSubKey = Path;

			HKEY hKey;
			REGSAM flag = KEY_WOW64_64KEY;
			DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
			LONG lRet = RegOpenKeyEx(RootPath, Path, 0, KEY_ALL_ACCESS | flag, &hKey);
			//LONG lRet = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, NULL, NULL, REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
			if (ERROR_SUCCESS != lRet)
			{
				return;
			}

			lRet = ::RegSetValueExW(hKey, Name, NULL, Type, (LPBYTE)material, wcslen(material) * sizeof(TCHAR) + 1); //设置注册表项
			if (ERROR_SUCCESS != lRet)
			{
				return;
			}

			::RegCloseKey(hKey); //与RegCreateKeyEx配对写
		};
		template <typename T>
		bool Read(HKEY RootPath, const wchar_t* Path, const wchar_t* Name, DWORD Type/* NULL */, T& t)
		{
			if (RootPath == NULL)
			{
				RootPath = HKEY_LOCAL_MACHINE;
			}
			DWORD BufferSize = 8192;
			HKEY hKey;
			REGSAM flag = KEY_WOW64_64KEY;
			DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
			LONG lRet = RegOpenKey(RootPath, Path, &hKey);
			DWORD dwType = REG_SZ;
			//WCHAR SaveText[MAX_PATH] = { '\0' };;

			if (ERROR_SUCCESS != lRet)
			{
				cout << "\n 1 Error !!  : ";
				wcout << lRet;
				return false;
			}
			if (Type == NULL)
			{
				//lRet = ::RegQueryValueExW(hKey, Name, 0, &dwType,(LPBYTE) &SaveText, &BufferSize);
				//wcout << "\n 2:2  return :" << lRet << "   Size :" << BufferSize << "    Data : " << SaveText;
				//
				//lRet = ::RegGetValueW(hKey, NULL, Name, RRF_RT_ANY, NULL, (PVOID)&SaveText, &BufferSize);
				lRet = ::RegGetValueW(hKey, NULL, Name, RRF_RT_ANY, NULL, &t, &BufferSize);
				//wcout << "\n 2:3  return :" << lRet << "   Size :" << BufferSize << "    Data : " << SaveText;
			}
			else
			{
				lRet = ::RegGetValueW(hKey, NULL, Name, Type, NULL, &t, &BufferSize);
			}
			if (ERROR_SUCCESS != lRet)
			{
				cout << "\n 2 Error !!  : ";
				wcout << lRet;
				return false;
			}
			::RegCloseKey(hKey);

			return true;
		}
		bool Remove(HKEY RootPath, const wchar_t* Path, const wchar_t* Name, int bit/* 64 or 32 */)
		{
			if (RootPath == NULL)
			{
				RootPath = HKEY_LOCAL_MACHINE;
			}
			DWORD BufferSize = 8192;
			HKEY hKey;
			REGSAM flag = KEY_WOW64_64KEY;
			DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
			LONG lRet = RegOpenKey(RootPath, Path, &hKey);
			DWORD dwType = REG_SZ;
			//WCHAR SaveText[MAX_PATH] = { '\0' };;
			if (ERROR_SUCCESS != lRet)
			{
				cout << "\n 1 Error !!  : ";
				wcout << lRet;
				return false;
			}
			if (bit == 32)
				lRet = RegDeleteKeyExW(hKey, Name, KEY_WOW64_32KEY, NULL);
			else
				lRet = RegDeleteKeyExW(hKey, Name, KEY_WOW64_64KEY, NULL);
			if (ERROR_SUCCESS != lRet)
			{
				cout << "\n 2 Error !!  : ";
				wcout << lRet;
				return false;
			}
			return true;
		}
	}
}