#pragma once
#ifndef _RXML_ERROR_HPP
#define _RXML_ERROR_HPP

#include <stdexcept>
#include <string>


namespace rxml {


class notfound_error
	: public std::runtime_error
{
public:
	notfound_error(const std::string& _what)
		: std::runtime_error(_what)
	{
	}
};












struct notfound_generator
{
	void operator ()(...)
	{
		throw notfound_error("failed to find entity");
	}
};






namespace defaults {
	// Default tags
	struct not_found;

	template<typename Tag, bool IsStd = false>
	struct registry
	{
		static_assert(sizeof(Tag) != sizeof(Tag), "Tag is not in the registry!");
	};


	// Default error generators
	template<bool _Std>
	struct registry<not_found, _Std>
	{
		typedef notfound_generator generator;
	};

}

namespace detail {

	typedef defaults::registry<defaults::not_found>::generator default_notfound_generator;
}



}










#endif