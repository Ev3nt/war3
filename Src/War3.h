#pragma once

#include <Windows.h>
#include <AclAPI.h>

#include <stdexcept>

#undef IRINABOT

class War3 {
public:
	static War3& getInstance();

	BOOL run();

private:
	War3();
	~War3();

	//static BOOL WINAPI _CRT_INIT(HMODULE module, UINT reason, LPVOID reserved);
	static DWORD WINAPI GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
	static DWORD WINAPI SetSecurityInfo(HANDLE handle, SE_OBJECT_TYPE ObjectType, SECURITY_INFORMATION SecurityInfo, PSID psidOwner, PSID psidGroup, PACL pDacl, PACL pSacl);

	static DWORD WINAPI AIL_startup();

	static BOOL APIENTRY DllMain(HMODULE module, UINT reason, LPVOID reserved);

	HMODULE stormBase_ = nullptr;
	HMODULE gameBase_ = nullptr;
};