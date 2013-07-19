#pragma once
#ifndef _RXML_VALUE_HPP
#define _RXML_VALUE_HPP


#include <rapidxml.hpp>
#include <regex>
#include "get.hpp"

namespace rxml {


namespace detail {

	template<typename _Ch>
	struct no_checker
	{
	};

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

	template<typename _Ch>
	bool apply_check(const std::basic_string<_Ch>& seq, const no_checker<_Ch>& check)
	{
		return true;
	}

	template<typename _Ty>
	struct identity
	{
		typedef _Ty type;
	};
}


// ########################################### value ###########################################
template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_base<_Ch>& entity)
{
	return std::basic_string<_Ch>(entity.value(), entity.value_size());
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_base<_Ch>* entity)
{
	assert(node);
	return rxml::value(*entity)
}

template<typename _Ch, typename _TGen>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const _Ch* path, _TGen throw_notfound, std::size_t path_size = 0)
{
	auto& entity = rxml::get(node, path, throw_notfound, path_size);
	return rxml::value(entity);
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const _Ch* path, std::size_t path_size = 0)
{
	return rxml::value(node, path, defaults::registry<defaults::not_found>::generator(), path_size);
}

template<typename _Ch, typename _TGen>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return rxml::value(node, path.c_str(), throw_notfound, path.size());
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path)
{
	return rxml::value(node, path, defaults::registry<defaults::not_found>::generator());
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
	return rxml::value(node, path, defaults::registry<defaults::not_found>::generator(), path_size);
}

template<typename _Ch, typename _TGen>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return rxml::value(node, path.c_str(), throw_notfound, path.size());
}

template<typename _Ch>
std::basic_string<_Ch> value(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path)
{
	return rxml::value(node, path, defaults::registry<defaults::not_found>::generator());
}


// ########################################### value with regex-check ###########################################
template<typename _Ch, typename _F, typename _TGen, typename _RGen>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const _Ch* path, const _F& checker, _TGen throw_notfound, _RGen throw_nomatch, std::size_t path_size = 0)
{
	std::basic_string<_Ch> result = rxml::value(node, path, throw_notfound, path_size);

	if(!detail::apply_check(result, checker))
	{
		throw_nomatch(result, &node, std::basic_string<_Ch>(path));
		rxml_assert(!"An exception should have been thrown!");
	}

	return result;
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const _Ch* path, const _F& checker, std::size_t path_size = 0)
{
	return rxml::valuex(node, path, checker, defaults::registry<defaults::not_found>::generator(), defaults::registry<defaults::no_match>::generator(), path_size);
}

template<typename _Ch, typename _F, typename _TGen, typename _RGen>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, const _F& checker, _TGen throw_notfound, _RGen throw_nomatch)
{
	return rxml::valuex(node, path.c_str(), checker, throw_notfound, throw_nomatch, path.size());
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, const _F& checker)
{
	return rxml::valuex(node, path, checker, defaults::registry<defaults::not_found>::generator(), defaults::registry<defaults::no_match>::generator());
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
	return rxml::valuex(node, path, checker, defaults::registry<defaults::not_found>::generator(), defaults::registry<defaults::no_match>::generator(), path_size);
}

template<typename _Ch, typename _F, typename _TGen, typename _RGen>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, const _F& checker, _RGen throw_nomatch, _TGen throw_notfound)
{
	return rxml::valuex(node, path.c_str(), checker, throw_notfound, path.size());
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuex(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, const _F& checker)
{
	return rxml::valuex(node, path, checker, defaults::registry<defaults::not_found>::generator(), defaults::registry<defaults::no_match>::generator());
}


// ########################################### value with default parameter ###########################################
template<typename _Ch, typename _F>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>* node, const _Ch* path, const std::basic_string<_Ch>& fallback, std::size_t path_size, const _F& checker)
{
	auto* entity = rxml::get(node, path, path_size);

	if(entity)
	{
		std::basic_string<_Ch> val(entity->value(), entity->value_size());
		if(detail::apply_check(val, checker))
		{
			return std::move(val);
		}
	}

	return fallback;
}



template<typename _Ch, typename _F>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, const typename detail::identity<std::basic_string<_Ch>>::type& fallback, const _F& checker)
{
	return rxml::valuefb(node, path.c_str(), fallback, path.size(), checker);
}

template<typename _Ch>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>* node, const _Ch* path, const typename detail::identity<std::basic_string<_Ch>>::type& fallback, std::size_t path_size = 0)
{
	return rxml::valuefb(node, path, fallback, path_size, detail::no_checker<_Ch>());
}

template<typename _Ch>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path, const typename detail::identity<std::basic_string<_Ch>>::type& fallback)
{
	return rxml::valuefb(node, path, fallback, detail::no_checker<_Ch>());
}



template<typename _Ch, typename _F>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>& node, const _Ch* path, const typename detail::identity<std::basic_string<_Ch>>::type& fallback, std::size_t path_size, const _F& checker)
{
	return rxml::valuefb(&node, path, fallback, path_size, detail::no_checker<_Ch>());
}

template<typename _Ch, typename _F>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, const typename detail::identity<std::basic_string<_Ch>>::type& fallback, const _F& checker)
{
	return rxml::valuefb(node, path.c_str(), fallback, path.size(), checker);
}


template<typename _Ch>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>& node, const _Ch* path, const typename detail::identity<std::basic_string<_Ch>>::type& fallback, std::size_t path_size = 0)
{
	return rxml::valuefb(&node, path, fallback, path_size, detail::no_checker<_Ch>());
}

template<typename _Ch>
std::basic_string<_Ch> valuefb(const rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, const typename detail::identity<std::basic_string<_Ch>>::type& fallback)
{
	return rxml::valuefb(node, path, fallback, detail::no_checker<_Ch>());
}

}










#endif
