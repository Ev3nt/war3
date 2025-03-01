#include "War3.h"

#include "HookManager.h"
#include "function.h"
#include "Utils.h"

#include <string>

War3& War3::getInstance() {
	static War3 war3;

	return war3;
}

War3::War3() {
	std::string gamePath = "";
#ifdef IRINABOT
	gamePath += Utils::getEnv("GamePath") + "\\";
#endif
	stormBase_ = LoadLibrary((gamePath + "storm.dll").data());
	if (!stormBase_) {
		throw std::runtime_error("Не удалось открыть Storm.dll." /*"Couldn't open Storm.dll."*/);
	}

	function<BOOL()> SFileDestroy(GetProcAddress(stormBase_, (LPCSTR)262));
	SFileDestroy();
	
	/*HANDLE mpq;
	sfunction<BOOL(LPCSTR, DWORD, DWORD, HANDLE*)> SFileOpen(GetProcAddress(stormBase_, (LPCSTR)266));
	SFileOpen("Test Archive.mpq", 9, 0, &mpq);*/

	HookManager::Attach(::GetModuleFileNameA, War3::GetModuleFileNameA);
	HookManager::Attach(::SetSecurityInfo, War3::SetSecurityInfo);

	gameBase_ = LoadLibrary((gamePath + "game.dll").data());
	if (!gameBase_) {
		throw std::runtime_error("Не удалось открыть Game.dll." /*"Couldn't open Game.dll."*/);

		return;
	}

	if (!GetProcAddress(gameBase_, "GameMain")) {
		throw std::runtime_error("Game.dll повреждён." /*"Game.dll is corrupt."*/);

		return;
	}

	HMODULE mss32Base = GetModuleHandle("mss32.dll");
	if (mss32Base) {
		HookManager::Attach(GetProcAddress(mss32Base, "_AIL_startup@0"), War3::AIL_startup);
	}

	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)gameBase_ + ((PIMAGE_DOS_HEADER)gameBase_)->e_lfanew);
	PIMAGE_OPTIONAL_HEADER optionalHeader = &ntHeader->OptionalHeader;

	HookManager::Attach((DWORD)gameBase_ + optionalHeader->AddressOfEntryPoint, War3::DllMain);

	//HookManager::Attach((DWORD)gameBase_ + 0x7E12DF, War3::_CRT_INIT);
}

War3::~War3() {
	FreeLibrary(gameBase_);
	FreeLibrary(stormBase_);
}

BOOL War3::run() {
	sfunction<BOOL(HMODULE)> gameMain(GetProcAddress(gameBase_, "GameMain"));

	return gameMain(gameBase_);
}

//BOOL WINAPI War3::_CRT_INIT(HMODULE module, UINT reason, LPVOID reserved) {
//	return TRUE;
//}

DWORD WINAPI War3::GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize) {
	if (hModule == GetModuleHandle(nullptr) || !hModule) {
		DWORD lenght = GetCurrentDirectory(nSize, lpFilename);

		return lenght ? (!strncat_s(lpFilename, nSize, "\\", 1) ? (lenght + 1) : lenght) : lenght;
	}

	return HookManager::Invoke(War3::GetModuleFileNameA, hModule, lpFilename, nSize);
}

DWORD WINAPI War3::SetSecurityInfo(HANDLE handle, SE_OBJECT_TYPE ObjectType, SECURITY_INFORMATION SecurityInfo, PSID psidOwner, PSID psidGroup, PACL pDacl, PACL pSacl) {
	return ERROR_SUCCESS;
}

DWORD WINAPI War3::AIL_startup() {
	std::vector<std::string> libraries = Utils::slice(Utils::getEnv("DelayLibraries"), ';');
	for (const auto& library : libraries) {
		LoadLibrary(Utils::trim(library).data());
	}

	return HookManager::Invoke(War3::AIL_startup);
}

BOOL APIENTRY War3::DllMain(HMODULE module, UINT reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_DETACH) {
		return TRUE;
	}

	return HookManager::Invoke(War3::DllMain, module, reason, reserved);
}