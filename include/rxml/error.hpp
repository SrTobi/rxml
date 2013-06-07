#pragma once
#ifndef _RXML_ERROR_HPP
#define _RXML_ERROR_HPP

#include <stdexcept>
#include <string>
#include <cassert>



#ifndef rxml_assert
#define rxml_assert(_expr)	assert(_expr)
#endif

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


class nomatch_error
	: public std::runtime_error
{
public:
	nomatch_error(const std::string& _what)
		: std::runtime_error(_what)
	{
	}
};








struct notfound_error_generator
{
	void operator ()(...) const
	{
		throw notfound_error("failed to find entity");
	}
};

struct nomatch_error_generator
{
	void operator ()(...) const
	{
		throw nomatch_error("sequence does not match the regex");
	}
};





namespace defaults {
	// Default tags
	struct not_found;
	struct no_match;

	template<typename Tag, bool IsStd = false>
	struct registry
	{
		static_assert(sizeof(Tag) != sizeof(Tag), "Tag is not in the registry!");
	};


	// Default error generators
	template<bool _Std>
	struct registry<not_found, _Std>
	{
		typedef notfound_error_generator generator;
	};

	template<bool _Std>
	struct registry<no_match, _Std>
	{
		typedef nomatch_error_generator generator;
	};

}

namespace detail {

	typedef defaults::registry<defaults::not_found>::generator default_notfound_generator;
	typedef defaults::registry<defaults::no_match>::generator default_nomatch_generator;
}



}










#endif