### Usage
Simply write ```bwn::TypeTreat<your type>::Name()```, and it will return string view of your type string representation. String view type by default set as ```std::string_view``` (or ```std::experimental::string_view``` if you are using standart below c++17). To change that you can define ```TYPE_TREAT_STRING_VIEW``` with your desired type like so:
```cpp
// ...
#define TYPE_TREAT_STRING_VIEW boost::string_view
#include "TypeTreat.h"
// ...
```
```TypeTreat``` can work with pointers, references, arrays (sized and unsized), functions and templates.
It doesn't use compiler macros so it can't guess name of your type. But you can help it:
Simply create specialisation of type ```TypeName``` for your class:
```cpp
template<>
struct bwn::TypeName<std::string>>
{
	using String =  CtimeString<'s','t','d',':',':','s','t','r','i','n','g'>;
};
```
If your type is template, instead of full specialisation you need to write part specialization of ```TypeName``` for your class.
Here is few examples on that.
```cpp
template<typename FirstT, typename SecondT, typename ThirdT>
struct bwn::TypeName<HugeTemplate<FirstT,SecondT,ThirdT>>
{
	using String =  CtimeConcatT<
	    CtimeString<'H','u','g','e','T','e','m','p','l','a','t','e','<'>,
	    typename bwn::TypeTreat<FirstT, SecondT, ThirdT>::String,
	    CtimeString<'>'>>;
};
```
```TypeTreat``` by itself can't handle templates with nontype parameters, but you still can write part specialization for those cases. You can find few helper utils for this like ```CtimeParseUintT```, ```CtimeParseUintHexT``` and ```CtimeParseIntT```.
```cpp
template<unsigned int UintV, int IntV, int* PtrV>
struct bwn::TypeName<HugeTemplate<IntV,UintV,PtrV>>
{
	using String =  CtimeConcatT<
	    CtimeString<'H','u','g','e','T','e','m','p','l','a','t','e','<'>,
	    CtimeParseUintT<UintV>,
	    CtimeString<','>,
	    CtimeParseIntT<IntV>,
	    CtimeString<','>,
	    CtimeParseUintHexT<static_cast<uint64_t>(PtrV)>,
	    CtimeString<'>'>>;
};
```
For each your template specialization you need to write separate ```TypeName``` specialization.
```TypeTreat``` has no runtime overhead, but on the other hand it has not small compiletime overhead, so be carefull using it. 

