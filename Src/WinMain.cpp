#include <Windows.h>

#include "HookManager.h"
#include "Utils.h"

#include "d3d8to9/d3d8to9.hpp"

#include "War3.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE instancePrev, LPSTR cmdLine, int cmdShow) {
	try {
#ifdef IRINABOT
		SetCurrentDirectory(Utils::getEnv("GamePath").data());
#endif
		War3& war3 = War3::getInstance();

#ifdef IRINABOT
		std::vector<std::string> libraries = Utils::slice(Utils::getEnv("Libraries"), ';');
		for (const auto& library : libraries) {
			LoadLibrary(Utils::trim(library).data());
		}
	#endif IRINABOT

		HMODULE d3d8Base = LoadLibrary("d3d8.dll");
		if (d3d8Base) {
			HookManager::Attach(GetProcAddress(d3d8Base, "Direct3DCreate8"), Direct3DCreate8);
		}

		war3.run();
	}
	catch (const std::runtime_error& exception) {
		MessageBox(NULL, exception.what(), "Error", MB_ICONERROR);
	}

	return 0;
}