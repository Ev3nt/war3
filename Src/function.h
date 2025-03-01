#pragma once

template <typename UnusedType>
class function;

template <typename ReturnType, typename ... ArgumentTypes>
class function<ReturnType(ArgumentTypes ...)> {
public:
	function() {}

	template <typename FunctionType>
	function(FunctionType function): address_((void*)function) {}

	ReturnType operator()(ArgumentTypes... args) {
		return address_ ? reinterpret_cast<ReturnType(__cdecl*)(ArgumentTypes...)>(address_)(args...) : ReturnType();
	}

	template <typename FunctionType>
	function& operator=(FunctionType function) {
		address_ = (void*)function;

		return *this;
	}

	void* address() {
		return address_;
	}

	template <typename FunctionType>
	bool operator==(FunctionType function) {
		return address_ == (void*)function;
	}

	operator void* () {
		return address_;
	}

protected:
	void* address_ = nullptr;
};

template <typename UnusedType>
class sfunction;

template <typename ReturnType, typename ... ArgumentTypes>
class sfunction<ReturnType(ArgumentTypes ...)> : public function<ReturnType(ArgumentTypes ...)> {
public:
	template <typename FunctionType>
	sfunction(FunctionType function) : function<ReturnType(ArgumentTypes ...)>(function) {}

	ReturnType operator()(ArgumentTypes... args) {
		return function<ReturnType(ArgumentTypes ...)>::address_ ? reinterpret_cast<ReturnType(__stdcall*)(ArgumentTypes...)>(function<ReturnType(ArgumentTypes ...)>::address_)(args...) : ReturnType();
	}
};

template <typename UnusedType>
class ffunction;

template <typename ReturnType, typename ... ArgumentTypes>
class ffunction<ReturnType(ArgumentTypes ...)> : public function<ReturnType(ArgumentTypes ...)> {
public:
	template <typename FunctionType>
	ffunction(FunctionType function) : function<ReturnType(ArgumentTypes ...)>(function) {}

	ReturnType operator()(ArgumentTypes... args) {
		return function<ReturnType(ArgumentTypes ...)>::address_ ? reinterpret_cast<ReturnType(__fastcall*)(ArgumentTypes...)>(function<ReturnType(ArgumentTypes ...)>::address_)(args...) : ReturnType();
	}
};

template <typename UnusedType>
class tfunction;

template <typename ReturnType, typename This, typename ... ArgumentTypes>
class tfunction<ReturnType(This, ArgumentTypes ...)> : public function<ReturnType(ArgumentTypes ...)> {
public:
	template <typename FunctionType>
	tfunction(FunctionType function) : function<ReturnType(ArgumentTypes ...)>(function) {}

	ReturnType operator()(This self, ArgumentTypes... args) {
		return function<ReturnType(ArgumentTypes ...)>::address_ ? reinterpret_cast<ReturnType(__thiscall*)(This, ArgumentTypes...)>(function<ReturnType(ArgumentTypes ...)>::address_)(self, args...) : ReturnType();
	}
};