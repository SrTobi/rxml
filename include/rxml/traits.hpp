#pragma once
#ifndef _RXML_TRAITS_HPP
#define _RXML_TRAITS_HPP

#include <type_traits>
#include <rapidxml.hpp>

namespace rxml {
namespace traits {

enum rapid_xml_types
{
	no_xml_type,
	xml_base_type,
	xml_node_type,
	xml_attribute_type,
	xml_document_type
};


namespace detail {

	template<typename _XmlEntity>
	struct rapidxml_type_impl
	{
		static const rapid_xml_types xml_type = no_xml_type;
	};


	// ###################### rapidxml::xml_base ######################
	template<typename _Ch>
	struct rapidxml_type_impl<rapidxml::xml_base<_Ch>>
	{
		typedef _Ch char_type;
		static const rapid_xml_types xml_type = xml_base_type;
	};


	// ###################### rapidxml::xml_attribute ######################
	template<typename _Ch>
	struct rapidxml_type_impl<rapidxml::xml_attribute<_Ch>>
	{
		typedef _Ch char_type;
		static const rapid_xml_types xml_type = xml_attribute_type;
	};


	// ###################### rapidxml::xml_node ######################
	template<typename _Ch>
	struct rapidxml_type_impl<rapidxml::xml_node<_Ch>>
	{
		typedef _Ch char_type;
		static const rapid_xml_types xml_type = xml_node_type;
	};


	// ###################### rapidxml::xml_document ######################
	template<typename _Ch>
	struct rapidxml_type_impl<rapidxml::xml_document<_Ch>>
	{
		typedef _Ch char_type;
		static const rapid_xml_types xml_type = xml_document_type;
	};

	template<typename _XmlEntity>
	struct check_xml_type
	{
		static_assert(rapidxml_type_impl<_XmlEntity>::xml_type != no_xml_type, "_XmlEntity is not a rapidxml type!");
	};

	template<bool _Cond>
	struct bool_result
		: public std::conditional<_Cond, std::true_type, std::false_type>::type
	{
	};
}


template<typename _Ty>
struct is_rapidxml_type
	: public detail::bool_result<detail::rapidxml_type_impl<_Ty>::xml_type != no_xml_type>
{
};

template<typename _Ty>
struct is_base_type
	: public detail::bool_result<detail::rapidxml_type_impl<_Ty>::xml_type == xml_base_type>
{
};


template<typename _Ty>
struct is_node_type
	: public detail::bool_result<detail::rapidxml_type_impl<_Ty>::xml_type == xml_node_type>
{
};

template<typename _Ty>
struct is_attribute_type
	: public detail::bool_result<detail::rapidxml_type_impl<_Ty>::xml_type == xml_attribute_type>
{
};

template<typename _Ty>
struct is_document_type
	: public detail::bool_result<detail::rapidxml_type_impl<_Ty>::xml_type == xml_document_type>
{
};

template<typename _XmlType>
struct char_type
	: detail::check_xml_type<_XmlType>
{
	typedef typename detail::rapidxml_type_impl<_XmlType>::char_type type;
};





}}

#endif