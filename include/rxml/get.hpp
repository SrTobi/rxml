#pragma once
#ifndef _RXML_GET_HPP
#define _RXML_GET_HPP

#include <type_traits>
#include <rapidxml.hpp>
#include <string>
#include "error.hpp"


namespace rxml {


namespace detail {

	template<bool _Cond>
	struct return_if
	{
		template<typename _Ty1>
		inline static _Ty1* ret(_Ty1* r)
		{
			return r;
		}
	};

	template<>
	struct return_if<false>
	{
		template<typename _Ty1>
		inline static std::nullptr_t ret(_Ty1*)
		{
			return nullptr;
		}
	};


	template<typename _Result, typename _Ch>
	inline _Result* get_impl(rapidxml::xml_node<_Ch>* node, const _Ch* path, std::size_t path_size)
	{
		static const bool extract_both = std::is_same<_Result, rapidxml::xml_base<_Ch>>::value;
		static const bool extract_node = extract_both || std::is_same<_Result, rapidxml::xml_node<_Ch>>::value;
		static const bool extract_attr = extract_both || std::is_same<_Result, rapidxml::xml_attribute<_Ch>>::value;
		
		rapidxml::xml_node<_Ch>* n = node;
		const char node_delimiter = _Ch('/');
		const char attr_delimiter = _Ch(':');
		const _Ch* end = path + path_size;

		while(path < end && n)
		{
			const _Ch* p = path;
			for(;p < end && *p != node_delimiter && *p != attr_delimiter; ++p);

			n = n->first_node(path, p - path);

			path = p + 1;

			if(*p == attr_delimiter)
			{
				if(extract_attr && n)
					return return_if<extract_attr>::ret(n->first_attribute(path, end - path));
				else
					break;
			}
		}

		// if this fails, the path has a bad format
		assert(extract_node);

		return return_if<extract_node>::ret(n);
	}
}



template<typename _Ch>
std::basic_string<_Ch> name(const rapidxml::xml_base<_Ch>* _entitiy)
{
	if(!_entitiy)
		return basic_string<_Ch>();
	return basic_string<_Ch>(_entitiy->name(), _entitiy->name_size());
}

template<typename _Ch>
std::basic_string<_Ch> name(const rapidxml::xml_base<_Ch>& _entitiy)
{
	return basic_string<_Ch>(_entitiy.name(), _entitiy.name_size());
}




// ########################################### getnode - with ptr ###########################################
template<typename _Ch>
rapidxml::xml_node<_Ch>* getnode(rapidxml::xml_node<_Ch>* node, const _Ch* path, std::size_t path_size)
{
	return detail::get_impl<rapidxml::xml_node<_Ch>, _Ch>(node, path, path_size);
}

template<typename _Ch>
rapidxml::xml_node<_Ch>* getnode(rapidxml::xml_node<_Ch>* node, const _Ch* path)
{
	return getnode(node, path, rapidxml::internal::measure(path));
}

template<typename _Ch>
rapidxml::xml_node<_Ch>* getnode(rapidxml::xml_node<_Ch>* node, const std::basic_string<_Ch>& path)
{
	return getnode(node, path.c_str(), path.size());
}

// ########################################### getnode - with reference and throw function ###########################################
template<typename _Ch, typename _TGen>
rapidxml::xml_node<_Ch>& getnode(rapidxml::xml_node<_Ch>& node, const _Ch* path, std::size_t path_size, _TGen throw_notfound)
{
	rapidxml::xml_node<_Ch>* result = getnode(&node, path, path_size);

	if(!result)
	{
		throw_notfound(node, path);
		assert(!"An exception should have been thrown!");
	}

	return *result;
}

template<typename _Ch, typename _TGen>
rapidxml::xml_node<_Ch>& getnode(rapidxml::xml_node<_Ch>& node, const _Ch* path, _TGen throw_notfound)
{
	return getnode(node, path, rapidxml::internal::measure(path), throw_notfound);
}

template<typename _Ch, typename _TGen>
rapidxml::xml_node<_Ch>& getnode(rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return getnode(node, path.c_str(), path.size(), throw_notfound);
}



// ########################################### getnode - with reference ###########################################


template<typename _Ch>
rapidxml::xml_node<_Ch>& getnode(rapidxml::xml_node<_Ch>& node, const _Ch* path, std::size_t path_size)
{
	return getnode(node, path, detail::default_notfound_generator());
}

template<typename _Ch>
rapidxml::xml_node<_Ch>& getnode(rapidxml::xml_node<_Ch>& node, const _Ch* path)
{
	return getnode(node, path, rapidxml::internal::measure(path), detail::default_notfound_generator());
}

template<typename _Ch>
rapidxml::xml_node<_Ch>& getnode(rapidxml::xml_node<_Ch>& node, const std::basic_string<_Ch>& path)
{
	return getnode(node, path.c_str(), path.size(), detail::default_notfound_generator());
}


}



#endif