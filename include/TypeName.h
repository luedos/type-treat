#pragma once

#include <string>

#if __cplusplus >= 201703L
#include <sstream>
#include <iomanip>
#endif

namespace bwn
{

enum RefEnum
{
	NONE = 0x0,
	LValue = 0x1,
	RValue = 0x2
};

template<typename T, typename...Args>
struct TypeTreat;

#if __cplusplus >= 201703L
template<auto T, auto...Args>
struct VariableTreat;
#endif


	// FunctionTreat
#if true

template<typename T>
struct FunctionTreat
{
	using return_type = T;

	static const std::string ArgsToString();
};

template<typename T>
const std::string FunctionTreat<T>::ArgsToString()
{
	return std::string{};
}

template<typename R>
struct FunctionTreat<R()>
{
	using return_type = R;

	static const std::string ArgsToString();
};

template<typename R>
const std::string FunctionTreat<R()>::ArgsToString()
{
	return std::string{};
}

template<typename R, typename...Args>
struct FunctionTreat<R(Args...)>
{
	using return_type = R;

	static const std::string ArgsToString();
};

template<typename R, typename...Args>
const std::string FunctionTreat<R(Args...)>::ArgsToString()
{
	return TypeTreat<Args...>::get_name();
}


#endif // FunctionTreat

	// PtrLess
#if true
template<typename T, size_t PtrNum = 0, size_t RefNum = 0>
struct PtrLess
{
	using type = T;
	enum { ref = RefNum, ptrs = PtrNum };
	static std::string to_string();
};
template<typename T, size_t PtrNum, size_t RefNum>
std::string PtrLess<T, PtrNum, RefNum>::to_string() {
	return std::string{};
}

template<typename T>
struct PtrLess<T&>
{
	enum { ref = LValue, ptrs = PtrLess<T, 0, ref>::ptrs };
	using type = typename PtrLess<T, ptrs, ref>::type;
	static std::string to_string();
};
template<typename T>
std::string PtrLess<T&>::to_string() {
	return PtrLess<T>::to_string() + std::string{ "&" };
}

template<typename T>
struct PtrLess<T&&>
{
	enum { ref = RValue, ptrs = PtrLess<T, 0, ref>::ptrs };
	using type = typename PtrLess<T, ptrs, ref>::type;
	static std::string to_string();
};
template<typename T>
std::string PtrLess<T&&>::to_string() {
	return PtrLess<T>::to_string() + std::string{ "&&" };
}

template<typename T, size_t PtrNum, size_t RefNum>
struct PtrLess<T*, PtrNum, RefNum>
{
	enum { ref = RefNum, ptrs = PtrLess<T, PtrNum + 1, ref>::ptrs };
	using type = typename PtrLess<T, ptrs, ref>::type;
	static std::string to_string();
};
template<typename T, size_t PtrNum, size_t RefNum>
std::string PtrLess<T*, PtrNum, RefNum>::to_string() {
	return PtrLess<T>::to_string() + std::string{ "*" };
}

template<typename T>
struct PtrLess<T*>
{
	enum { ref = NONE, ptrs = PtrLess<T, 1, ref>::ptrs  };
	using type = typename PtrLess<T, ptrs, ref>::type;
	static std::string to_string();
};
template<typename T>
std::string PtrLess<T*>::to_string() {
	return PtrLess<T>::to_string() + std::string{ "*" };
}

template<typename T, size_t PtrNum, size_t RefNum>
struct PtrLess<T* const, PtrNum, RefNum>
{
	enum { ref = RefNum, ptrs = PtrLess<T, PtrNum + 1, ref>::ptrs };
	using type = typename PtrLess<T, ptrs, ref>::type;
	static std::string to_string();
};
template<typename T, size_t PtrNum, size_t RefNum>
std::string PtrLess<T* const, PtrNum, RefNum>::to_string() {
	return PtrLess<T>::to_string() + std::string{ "*const" };
}

template<typename T>
struct PtrLess<T* const>
{
	enum { ref = NONE, ptrs = PtrLess<T, 1, ref>::ptrs };
	using type = typename PtrLess<T, ptrs, ref>::type;
	static std::string to_string();
};
template<typename T>
std::string PtrLess<T* const>::to_string() {
	return PtrLess<T>::to_string() + std::string{ "*const" };
}

#endif // PtrLess

	// ArrayTreat
#if true
template<typename T>
struct ArrayTreat
{
	using type = T;
	using arryptrless = T;
	enum { is_array = false };
	static const std::string to_string();
};
template<typename T>
const std::string ArrayTreat<T>::to_string() {
	return std::string();
}

template<typename T, size_t SZ>
struct ArrayTreat<T[SZ]>
{
	using type = T;
	using arryptrless = typename ArrayTreat<typename PtrLess<T>::type>::arryptrless;
	enum { is_array = true };
	static const std::string to_string();
};
template<typename T, size_t SZ>
const std::string ArrayTreat<T[SZ]>::to_string() {
	return "[" + std::to_string(SZ) + "]";
}

template<typename T>
struct ArrayTreat<T[]>
{
	using type = T;
	using arryptrless = typename ArrayTreat<typename PtrLess<T>::type>::arryptrless;
	enum { is_array = true };
	static const std::string to_string();
};
template<typename T>
const std::string ArrayTreat<T[]>::to_string() {
	return "[]";
}

template<typename T>
std::string ArraysToString()
{
	std::string out{};

	using clear = typename PtrLess<T>::type;
	if (ArrayTreat<clear>::is_array)
	{
		out = PtrLess<T>::to_string();
		if (!out.empty()) {
			out = "(" + out + ")";
		}

		out = ArraysToString<typename ArrayTreat<clear>::type>() + out + ArrayTreat<clear>::to_string();

		return out;
	}

	return PtrLess<T>::to_string() + out;
}

#endif // ArrayTreat

	// TypeName
#if true

template<typename T>
struct TypeName
{
	static const std::string to_string();
};
template<typename T>
const std::string TypeName<T>::to_string()
{
	return "default";
}

template<typename T>
struct TypeName<const T>
{
	static const std::string to_string();
};
template<typename T>
const std::string TypeName<const T>::to_string()
{
	return "const " + TypeName<T>::to_string();
}

template<template <typename...> class T, typename ...Args>
struct TypeName<T<Args...>>
{
	static const std::string to_string();
};
template<template <typename...> class T, typename ...Args>
const std::string TypeName<T<Args...>>::to_string()
{
	return std::string("template") + "<" + TypeTreat<Args...>::get_name() + ">";
}
	
	// Returning to TypeTreat
#if true
template<typename R, typename...Args>
struct TypeName<R(Args...)>
{
	static const std::string to_string();
};
template<typename R, typename...Args>
const std::string TypeName<R(Args...)>::to_string()
{
	return TypeTreat<R(Args...)>::get_name();
}

template<typename T, size_t SZ>
struct TypeName<T[SZ]>
{
	static const std::string to_string();
};
template<typename T, size_t SZ>
const std::string TypeName<T[SZ]>::to_string()
{
	return TypeTreat<T[SZ]>::get_name();
}

template<typename T>
struct TypeName<T[]>
{
	static const std::string to_string();
};
template<typename T>
const std::string TypeName<T[]>::to_string()
{
	return TypeTreat<T[]>::get_name();
}

template<typename T>
struct TypeName<T*>
{
	static const std::string to_string();
};
template<typename T>
const std::string TypeName<T*>::to_string()
{
	return TypeTreat<T*>::get_name();
}

template<typename T>
struct TypeName<T* const>
{
	static const std::string to_string();
};
template<typename T>
const std::string TypeName<T* const>::to_string()
{
	return TypeTreat<T* const>::get_name();
}
#endif // Returning to TypeTreat

#if __cplusplus >= 201703L
template<template <auto...> typename T, auto...Args>
struct TypeName<T<Args...>>
{
	static const std::string to_string();
};
template<template <auto...> typename T, auto...Args>
const std::string TypeName<T<Args...>>::to_string()
{
	return std::string("template") + "<" + VariableTreat<Args...>::to_string() + ">";
}
#endif

// TypeName Variables
#if true
// Char
template<>
const std::string TypeName<char>::to_string();
template<>
const std::string TypeName<unsigned char>::to_string();

// short
template<>
const std::string TypeName<short>::to_string();
template<>
const std::string TypeName<unsigned short>::to_string();

// Int
template<>
const std::string TypeName<int>::to_string();
template<>
const std::string TypeName<unsigned int>::to_string();

// Long
template<>
const std::string TypeName<long>::to_string();
template<>
const std::string TypeName<unsigned long>::to_string();

// Long long
template<>
const std::string TypeName<long long>::to_string();
template<>
const std::string TypeName<unsigned long long>::to_string();

// float
template<>
const std::string TypeName<float>::to_string();

// double
template<>
const std::string TypeName<double>::to_string();

// long double
template<>
const std::string TypeName<long double>::to_string();

// Rest
template<>
const std::string TypeName<void>::to_string();
template<>
const std::string TypeName<bool>::to_string();
template<>
const std::string TypeName<decltype(nullptr)>::to_string();
#endif // TypeName Variables


#endif // TypeName

	// TypeTreat
#if true

template<typename T, typename...Args>
struct TypeTreat
{
	static const std::string get_name();
};

template<typename T, typename...Args>
const std::string TypeTreat<T, Args...>::get_name()
{
	return TypeTreat<T>::get_name() 
		+ (sizeof...(Args) 
			? (", " + TypeTreat<Args...>::get_name()) 
			: std::string{});
}

template<typename T>
struct TypeTreat<T>
{
	static const std::string get_name();
};

template<typename T>
const std::string TypeTreat<T>::get_name()
{
	using firstLayer = typename PtrLess<T>::type;
	
	std::string out{};

	if
#if __cplusplus >= 201703L
		constexpr
#endif
		(ArrayTreat<firstLayer>::is_array)
	{
		out = ArraysToString<T>();
	}
	else
	{
		out = PtrLess<T>::to_string();
	}


	using secondLayer = typename ArrayTreat<firstLayer>::arryptrless;
	if
#if __cplusplus >= 201703L
		constexpr 
#endif
		(std::is_function<secondLayer>::value)
	{
		if (!out.empty()) {
			out = "(" + out + ")";
		}

		out = TypeTreat<typename FunctionTreat<secondLayer>::return_type>::get_name()
			+ out
			+ "(" + FunctionTreat<secondLayer>::ArgsToString() + ")";
	}
	else
	{
		out = TypeName<secondLayer>::to_string() + out;
	}
	return out;
}



#endif // TypeTreat

	// VariableTreat
#if true

#if __cplusplus >= 201703L
template<typename T>
std::string VariableToString(T var)
{
	return std::to_string(var);
}
template<typename T>
std::string VariableToString(T* var)
{
	std::stringstream stream;
	stream << "0x" << std::hex << std::setw(sizeof(T*) * 2) << std::setfill('0') << var;

	return stream.str();
}

std::string VariableToString(std::nullptr_t);

template<auto T, auto...Args>
struct VariableTreat
{
	static const std::string to_string();
};

template<auto T, auto...Args>
const std::string VariableTreat<T, Args...>::to_string()
{
	return VariableTreat<T>::to_string()
		+ (sizeof...(Args) 
			? (", " + VariableTreat<Args...>::to_string()) 
			: std::string{});
}

template<auto T>
struct VariableTreat<T>
{
	static const std::string to_string();
};

template<auto T>
const std::string VariableTreat<T>::to_string()
{
	return VariableToString(T);
}
#endif

#endif // VariableTreat

}