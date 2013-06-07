#pragma once
#ifndef _RXML_VALUE_HPP
#define _RXML_VALUE_HPP


#include <rapidxml.hpp>
#include <regex>
#include "get.hpp"

namespace rxml {


namespace detail {

	template<typename _Ch, typename _F>
	bool apply_check(const std::basic_string<_Ch>& seq, const _F& check)
	{
		return check(seq);
	}

	template<typename _Ch>
	bool apply_check(const std::basic_string<_Ch>& seq, const std::basic_regex<_Ch>& regex)
	{
		return std::regex_match(seq, std::regex(regex));
	}

	template<typename _Ch>
	bool apply_check(const std::basic_string<_Ch>& seq, const std::basic_string<_Ch>& regex)
	{
		return apply_check(seq, std::basic_regex<_Ch>(regex));
	}

	template<typename _Ch>
	bool apply_check(const std::basic_string<_Ch>& seq, const _Ch* regex)
	{
		return apply_check(seq, std::basic_regex<_Ch>(regex));
	}


}


// ########################################### value ###########################################
template<typename _Ch, typename _TGen>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const _Ch* path, _TGen throw_notfound, std::size_t path_size = 0)
{
	auto& entity = rxml::get(node, path, throw_notfound, path_size);
	return std::basic_string<_Ch>(entity.value(), entity.value_size());
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const _Ch* path, std::size_t path_size = 0)
{
	return rxml::value(node, path, detail::default_notfound_generator(), path_size);
}

template<typename _Ch, typename _TGen>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return rxml::value(node, path.c_str(), throw_notfound, path.size());
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path)
{
	return rxml::value(node, path, detail::default_notfound_generator());
}

template<typename _Ch, typename _TGen>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>* node, const _Ch* path, _TGen throw_notfound, std::size_t path_size = 0)
{
	assert(node);
	return rxml::value(*node, path, throw_notfound, path_size);
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>* node, const _Ch* path, std::size_t path_size = 0)
{
	return rxml::value(node, path, detail::default_notfound_generator(), path_size);
}

template<typename _Ch, typename _TGen>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return rxml::value(node, path.c_str(), throw_notfound, path.size());
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path)
{
	return rxml::value(node, path, detail::default_notfound_generator());
}


// ########################################### value with regex-check ###########################################
template<typename _Ch, typename _F, typename _TGen, typename _RGen>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const _Ch* path, const _F& checker, _TGen throw_notfound, _RGen throw_nomatch, std::size_t path_size = 0)
{
	std::basic_string<_Ch> result = rxml::value(node, path, throw_notfound, path_size);

	if(!detail::apply_check(result, checker))
	{
		throw_nomatch(result, &node, path);
		rxml_assert(!"An exception should have been thrown!");
	}

	return result;
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const _Ch* path, const _F& checker, std::size_t path_size = 0)
{
	return rxml::valuex(node, path, checker, detail::default_notfound_generator(), detail::default_nomatch_generator(), path_size);
}

template<typename _Ch, typename _F, typename _TGen, typename _RGen>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, const _F& checker, _TGen throw_notfound, _RGen throw_nomatch)
{
	return rxml::valuex(node, path.c_str(), checker, throw_notfound, throw_nomatch, path.size());
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, const _F& checker)
{
	return rxml::valuex(node, path, checker, detail::default_notfound_generator(), detail::default_nomatch_generator());
}

template<typename _Ch, typename _F, typename _TGen, typename _RGen>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>* node, const _Ch* path, const _F& checker, _TGen throw_notfound, _RGen throw_nomatch, std::size_t path_size = 0)
{
	assert(node);
	return rxml::valuex(*node, path, checker, throw_notfound, path_size);
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>* node, const _Ch* path, const _F& checker, std::size_t path_size = 0)
{
	return rxml::valuex(node, path, checker, detail::default_notfound_generator(), detail::default_nomatch_generator(), path_size);
}

template<typename _Ch, typename _F, typename _TGen, typename _RGen>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, const _F& checker, _RGen throw_nomatch, _TGen throw_notfound)
{
	return rxml::valuex(node, path.c_str(), checker, throw_notfound, path.size());
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, const _F& checker)
{
	return rxml::valuex(node, path, checker, detail::default_notfound_generator(), detail::default_nomatch_generator());
}


// ########################################### value with regex-check ###########################################




}










#endif
