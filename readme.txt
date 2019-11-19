HOW TO USE
Simply write "TypeTreat<your type>::get_name()", and it will return you string representation of your type.
It can work with pointers, references, arrays (sized and unsized), functions, templates.
Of course it is not magic and it can't guess name of your type. But you can help it:
Simply create full specialisation of function "to_string()" from type "TypeName" for your class:
Example:

	const std::string bwn::TypeName<char>::to_string() 
	{
		return "char";
	}

If your type is template, instead of full specialisation you need to write part specialization of "TypeName" for your class.
Here is few examples on that.
1:

	template<typename T1, typename T2, typename T3>
	struct bwn::TypeName<HugeTemplate<T1,T2,T3>>
	{
		static const std::string to_string();
	};
	template<typename T1, typename T2, typename T3>
	const std::string TypeName<HugeTemplate<T1, T2, T3>>::to_string()
	{
		static std::string singelton = std::string("HugeTemplate") + "<" + TypeTreat<T1>::get_name() + ", " + TypeTreat<T2>::get_name() + ", " + TypeTreat<T3>::get_name() + ">";
		return singelton;
	}

2:

	template<tpyname T1, int T2, int* T3>
	struct bwn::TypeName<HugeTemplate<T1,T2,T3>>
	{
		static const std::string to_string();
	};
	template<typename T1, typename T2, typename T3>
	const std::string TypeName<HugeTemplate<T1, T2, T3>>::to_string()
	{
		static std::string singelton = std::string("HugeTemplate") + "<" + TypeTreat<T1>::get_name() + ", " + TypeTreat<T2>::to_string() + ", " + TypeTreat<T3>::to_string() + ">";
		return singelton;
	}

3:

	template<auto T1, auto T2, auto T3>
	struct bwn::TypeName<HugeTemplate<T1,T2,T3>>
	{
		static const std::string to_string();
	};
	template<typename T1, typename T2, typename T3>
	const std::string TypeName<HugeTemplate<T1, T2, T3>>::to_string()
	{
		static std::string singelton = std::string("HugeTemplate") + "<" + VariableTreat<T1>::to_string() + ", " + TypeTreat<T2>::to_string() + ", " + TypeTreat<T3>::to_string() + ">";
		return singelton;
	}

For each your template specialization you need to write separate "TypeName" specialization.
There are few cases where TypeTreat fails.
1. If template parameters of your class consists from both typename's and auto's, TypeTreat will not found out that your class is template, and return just "default" (example "2").
2. If one of your "auto" template parameters is lvalue reference, "TypeTreat" will return error.
Part specialization will save in all of those cases.

TODO
- Replace string concatination with compile time templates and fully transfer project to compile time.
