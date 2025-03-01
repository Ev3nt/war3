#pragma once

#include <map>
#include <windef.h>
#include "detours.h"

class HookManager {
	inline static std::map<LPVOID, LPVOID> detours_;
public:
	template <typename Function, typename Detour>
	static BOOL Attach(Function function, Detour detour) {
		detours_[(LPVOID)detour] = (LPVOID)function;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		BOOL status = DetourAttach(&detours_[(LPVOID)detour], (LPVOID)detour);
		DetourTransactionCommit();

		if (status) {
			DetourTransactionAbort();
			detours_.erase((LPVOID)detour);
		}

		return status;
	}

	template <typename Detour>
	static BOOL Detach(Detour detour) {
		BOOL status = ERROR_INVALID_HANDLE;

		if (detours_.find((LPVOID)detour) != detours_.end()) {
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			status = DetourDetach(&detours_[(LPVOID)detour], (LPVOID)detour);
			DetourTransactionCommit();

			if (status) {
				DetourTransactionAbort();
			}
			else {
				detours_.erase((LPVOID)detour);
			}
		}

		return status;
	}

	template <typename Detour>
	static Detour GetOrigin(Detour detour) {
		return (Detour)detours_[(LPVOID)detour];
	}

	template <typename Return, typename ...Args>
	static Return Invoke(Return(__cdecl* detour)(Args...), Args ... args) {
		auto origin = GetOrigin(detour);

		return origin ? origin(args...) : Return();
	}

	template <typename Return, typename ...Args>
	static Return Invoke(Return(__stdcall* detour)(Args...), Args ... args) {
		auto origin = GetOrigin(detour);

		return origin ? origin(args...) : Return();
	}

	template <typename Return, typename ...Args>
	static Return Invoke(Return(__fastcall* detour)(Args...), Args ... args) {
		auto origin = GetOrigin(detour);

		return origin ? origin(args...) : Return();
	}
};