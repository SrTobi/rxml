#pragma once
#ifndef _RXML_GET_HPP
#define _RXML_GET_HPP

#include <type_traits>
#include <rapidxml.hpp>
#include <string>
#include <cassert>
#include "error.hpp"


namespace rxml {


template<typename _Ch>
rapidxml::xml_node<_Ch>* getroot(rapidxml::xml_node<_Ch>* _entity)
{
	for(rapidxml::xml_node<_Ch>* e = _entity;
		e;
		e = _entity->parent())
	{ _entity = e; }

	return _entity;
}

template<typename _Ch>
const rapidxml::xml_node<_Ch>* getroot(const rapidxml::xml_node<_Ch>* _entity)
{
	for(const rapidxml::xml_node<_Ch>* e = _entity;
		e;
		e = _entity->parent())
	{ _entity = e; }

	return _entity;
}

template<typename _Ch>
rapidxml::xml_node<_Ch>& getroot(rapidxml::xml_base<_Ch>& _entity)
{
	return *getroot(&_entity);
}

template<typename _Ch>
const rapidxml::xml_node<_Ch>& getroot(const rapidxml::xml_base<_Ch>& _entity)
{
	return *getroot(&_entity);
}




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


	template<typename _Result, typename _Node, typename _Ch>
	struct get_impl
	{
		static const bool be_const = std::is_const<_Node>::value;
		static const bool extract_both = std::is_same<_Result, rapidxml::xml_base<_Ch>>::value;
		static const bool extract_node = extract_both || std::is_same<_Result, rapidxml::xml_node<_Ch>>::value;
		static const bool extract_attr = extract_both || std::is_same<_Result, rapidxml::xml_attribute<_Ch>>::value;

		typedef typename std::conditional<
						be_const
					,
						const rapidxml::xml_node<_Ch>
					,
						rapidxml::xml_node<_Ch>
				>::type node_type;
		
		typedef typename std::conditional<
						be_const
					,
						typename std::add_const<_Result>::type
					,
						_Result
				>::type return_type;



		static inline return_type* execute(node_type* node, const _Ch* path, std::size_t path_size)
		{
			const char node_delimiter = _Ch('/');
			const char attr_delimiter = _Ch(':');
			const char point_char = _Ch('.');

			const _Ch* end = path + path_size;

			if(*path == node_delimiter)
			{
				node = getroot(node);
				++path;
			}

			node_type* n = node;
			while(path < end && n)
			{
				const _Ch* p = path;

				if(*path != attr_delimiter)
				{
					for(;p < end && *p != node_delimiter && *p != attr_delimiter; ++p);

					if(p == path + 2 && *path == point_char && *(path+1) == point_char)
					{
						n = n->parent();
					}else{
						rxml_assert(p != path);
						n = n->first_node(path, p - path);
					}

					path = p + (*p == attr_delimiter? 0 : 1);
				}else{
					++path;
					if(extract_attr)
					{
						return n? return_if<extract_attr>::ret(n->first_attribute(path, end - path))
								: nullptr;
					}

					break;
				}
			}

			// if this fails, the path has a bad format
			rxml_assert(extract_node);

			return return_if<extract_node>::ret(n);
		}

		template<typename _TGen>
		static inline return_type& execute(node_type& node, const _Ch* path, std::size_t path_size, _TGen throw_notfound)
		{
			auto* result = execute(&node, path, path_size);

			if(!result)
			{
				throw_notfound(node, path);
				rxml_assert(!"An exception should have been thrown!");
			}

			return *result;
		}
	};

	template<typename _Result, typename _Node>
	struct return_type
	{
		typedef typename std::conditional
			<
				std::is_const<_Node>::value
			,
				typename std::add_const<_Result>::type
			,
				_Result
			>::type type;
	};
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



// ########################################### getnode  ###########################################
template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_node<_Ch>, _Node>::type* getnode(_Node* node, const _Ch* path, std::size_t path_size = 0)
{
	return detail::get_impl<rapidxml::xml_node<_Ch>, _Node, _Ch>::execute(node, path, path_size? path_size : rapidxml::internal::measure(path));
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_node<_Ch>, _Node>::type* getnode(_Node* node, const std::basic_string<_Ch>& path)
{
	return detail::get_impl<rapidxml::xml_node<_Ch>, _Node, _Ch>::execute(node, path.c_str(), path.size());
}

template<typename _Node, typename _Ch, typename _TGen>
typename detail::return_type<rapidxml::xml_node<_Ch>, _Node>::type& getnode(_Node& node, const _Ch* path, _TGen throw_notfound, std::size_t path_size = 0)
{
	return detail::get_impl<rapidxml::xml_node<_Ch>, _Node, _Ch>::execute(node, path, path_size? path_size : rapidxml::internal::measure(path), throw_notfound);
}

template<typename _Node, typename _Ch, typename _TGen>
typename detail::return_type<rapidxml::xml_node<_Ch>, _Node>::type& getnode(_Node& node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return detail::get_impl<rapidxml::xml_node<_Ch>, _Node, _Ch>::execute(node, path.c_str(), path.size(), throw_notfound);
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_node<_Ch>, _Node>::type& getnode(_Node& node, const _Ch* path, std::size_t path_size = 0)
{
	return getnode(node, path, path_size, detail::default_notfound_generator());
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_node<_Ch>, _Node>::type& getnode(_Node& node, const std::basic_string<_Ch>& path)
{
	return getnode(node, path, detail::default_notfound_generator());
}


// ########################################### getattr  ###########################################
template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_attribute<_Ch>, _Node>::type* getattr(_Node* node, const _Ch* path, std::size_t path_size = 0)
{
	return detail::get_impl<rapidxml::xml_attribute<_Ch>, _Node, _Ch>::execute(node, path, path_size? path_size : rapidxml::internal::measure(path));
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_attribute<_Ch>, _Node>::type* getattr(_Node* node, const std::basic_string<_Ch>& path)
{
	return detail::get_impl<rapidxml::xml_attribute<_Ch>, _Node, _Ch>::execute(node, path.c_str(), path.size());
}

template<typename _Node, typename _Ch, typename _TGen>
typename detail::return_type<rapidxml::xml_attribute<_Ch>, _Node>::type& getattr(_Node& node, const _Ch* path, _TGen throw_notfound, std::size_t path_size = 0)
{
	return detail::get_impl<rapidxml::xml_attribute<_Ch>, _Node, _Ch>::execute(node, path, path_size? path_size : rapidxml::internal::measure(path), throw_notfound);
}

template<typename _Node, typename _Ch, typename _TGen>
typename detail::return_type<rapidxml::xml_attribute<_Ch>, _Node>::type& getattr(_Node& node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return detail::get_impl<rapidxml::xml_attribute<_Ch>, _Node, _Ch>::execute(node, path.c_str(), path.size(), throw_notfound);
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_attribute<_Ch>, _Node>::type& getattr(_Node& node, const _Ch* path, std::size_t path_size = 0)
{
	return getnode(node, path, path_size, detail::default_notfound_generator());
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_attribute<_Ch>, _Node>::type& getattr(_Node& node, const std::basic_string<_Ch>& path)
{
	return getnode(node, path, detail::default_notfound_generator());
}

// ########################################### get  ###########################################
template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_base<_Ch>, _Node>::type* get(_Node* node, const _Ch* path, std::size_t path_size = 0)
{
	return detail::get_impl<rapidxml::xml_base<_Ch>, _Node, _Ch>::execute(node, path, path_size? path_size : rapidxml::internal::measure(path));
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_base<_Ch>, _Node>::type* get(_Node* node, const std::basic_string<_Ch>& path)
{
	return detail::get_impl<rapidxml::xml_base<_Ch>, _Node, _Ch>::execute(node, path.c_str(), path.size());
}

template<typename _Node, typename _Ch, typename _TGen>
typename detail::return_type<rapidxml::xml_base<_Ch>, _Node>::type& get(_Node& node, const _Ch* path, _TGen throw_notfound, std::size_t path_size = 0)
{
	return detail::get_impl<rapidxml::xml_base<_Ch>, _Node, _Ch>::execute(node, path, path_size? path_size : rapidxml::internal::measure(path), throw_notfound);
}

template<typename _Node, typename _Ch, typename _TGen>
typename detail::return_type<rapidxml::xml_base<_Ch>, _Node>::type& get(_Node& node, const std::basic_string<_Ch>& path, _TGen throw_notfound)
{
	return detail::get_impl<rapidxml::xml_base<_Ch>, _Node, _Ch>::execute(node, path.c_str(), path.size(), throw_notfound);
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_base<_Ch>, _Node>::type& get(_Node& node, const _Ch* path, std::size_t path_size = 0)
{
	return getnode(node, path, path_size, detail::default_notfound_generator());
}

template<typename _Node, typename _Ch>
typename detail::return_type<rapidxml::xml_base<_Ch>, _Node>::type& get(_Node& node, const std::basic_string<_Ch>& path)
{
	return getnode(node, path, detail::default_notfound_generator());
}

}



#endif