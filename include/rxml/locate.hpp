#pragma once
#ifndef _RXML_LOCATE_HPP
#define _RXML_LOCATE_HPP

#include <vector>
#include <type_traits>
#include <rapidxml.hpp>
#include "traits.hpp"

namespace rxml {

namespace detail {



	template<typename _Ty, typename _Ch>
	struct parents_impl
	{		
		typedef typename std::conditional
			<
				std::is_const<_Ty>::value
				,
					const rapidxml::xml_node<_Ch>
				, 
					rapidxml::xml_node<_Ch>
			>::type node_type;


		static std::vector<node_type*> execute(_Ty& entity, bool descending)
		{

			std::vector<node_type*> result;

			for(node_type* node = entity.parent();
				node;
				node = node->parent())
			{
				result.emplace_back(node);
			}

			if(descending)
				std::reverse(result.begin(), result.end());

			return std::move(result);
		}

		static std::vector<node_type*> execute(_Ty* entity, bool descending)
		{
			if(entity)
				return execute(*entity, descending);
			else
				return std::vector<node_type*>();
		}
	};

	template<typename _Entity, typename _Ch>
	std::basic_string<_Ch> locate_imple(const _Entity& entity)
	{
		std::basic_string<_Ch> result;
		auto parents = parents_impl<const _Entity, _Ch>::execute(entity, false);

		if(parents.empty())
		{
			result.append(1, _Ch('/'));
		}else{
			for(auto it = parents.rbegin();
				it != parents.rend();
				++it)
			{
				result.append((*it)->name(), (*it)->name_size());
				result.append(1, _Ch('/'));
			}
		}

		// Check if the entity is an attribute
		if(traits::is_attribute_type<_Entity>::value
#ifndef _RXML_TYPE_CHECK_HACK
			/*
			 * Use this hack to check if entity is an attribute.
			 * Quite likely type() is 0 (no previous attribute) or higher then node_pi (there is a previous attribute) if
			 * entity is an attribute. Only problem is that type() might be 0, if entity is the document node.
			 * In this case we are wrong... but never mind...
			 */
			|| (traits::is_base_type<_Entity>::value
				&&	(  static_cast<const rapidxml::xml_node<_Ch>&>(entity).type() > rapidxml::node_pi
					|| static_cast<const rapidxml::xml_node<_Ch>&>(entity).type() == rapidxml::node_document)
			)
#endif
			)
			result.back() = _Ch(':');

		result.append(entity.name(), entity.name_size());

		return std::move(result);
	}

}




template<typename _Ch>
std::vector<const rapidxml::xml_node<_Ch>*> parents(const rapidxml::xml_base<_Ch>& entity, bool descending = true)
{
	return detail::parents_impl<const rapidxml::xml_node<_Ch>, _Ch>::execute(entity, descending);
}

template<typename _Ch>
std::vector<rapidxml::xml_node<_Ch>*> parents(rapidxml::xml_base<_Ch>& entity, bool descending = true)
{
	return detail::parents_impl<rapidxml::xml_node<_Ch>, _Ch>::execute(entity, descending);
}


template<typename _Ch>
std::vector<const rapidxml::xml_node<_Ch>*> parents(const rapidxml::xml_base<_Ch>* entity, bool descending = true)
{
	return detail::parents_impl<const rapidxml::xml_node<_Ch>, _Ch>::execute(entity, descending);
}

template<typename _Ch>
std::vector<rapidxml::xml_node<_Ch>*> parents(rapidxml::xml_base<_Ch>* entity, bool descending = true)
{
	return detail::parents_impl<rapidxml::xml_node<_Ch>, _Ch>::execute(entity, descending);
}


template<typename _Entity>
std::basic_string<typename traits::char_type<_Entity>::type> locate(const _Entity& entity)
{
	return detail::locate_imple<_Entity, typename traits::char_type<_Entity>::type>(entity);
}





}





#endif
