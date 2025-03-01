#include <Windows.h>

#include "Utils.h"

#include "War3.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE instancePrev, LPSTR cmdLine, int cmdShow) {
	try {
#ifdef IRINABOT
		SetCurrentDirectory(Utils::getEnv("GamePath").data());
#endif
		War3& war3 = War3::getInstance();

		/*std::vector<std::string> libraries = Utils::slice(Utils::getEnv("Libraries"), ';');
		for (const auto& library : libraries) {
			LoadLibrary(Utils::trim(library).data());
		}*/

		war3.run();
	}
	catch (const std::runtime_error& exception) {
		MessageBox(NULL, exception.what(), "Error", MB_ICONERROR);
	}

	return 0;
}