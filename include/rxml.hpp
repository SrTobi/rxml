#pragma once
#ifndef _RXML_HPP
#define _RXML_HPP


#include <type_traits>
#include <string>
#include <cassert>
#include <rapidxml.hpp>


namespace rxml {

template<typename _Ch = char>
class node;
template<typename _Ch = char>
class attribute;


namespace detail {

	template<typename _Ch, typename _ITy>
	class impl_base
	{
	public:
		typedef _Ch								char_type;
		typedef _ITy							impl_type;
		typedef std::basic_string<char_type>	string_type;
		typedef impl_base<char_type, impl_type>	self_type;
		typedef self_type*						this_type;
		typedef rapidxml::xml_base<char_type>	raw_base_type;
		typedef rapidxml::xml_node<char_type>	raw_node_type;
		typedef node<char_type>					node_type;
		typedef attribute<char_type>			attr_type;

		static_assert(std::is_base_of<
								raw_base_type,
								impl_type
								>::value,
						"_ITy must be derived from rapidxml::xml_base<_Ch>");
	public:
	
		string_type name() const
		{
			return string_type(_base()->name(), _base()->name_size());
		}

		void name(const string_type& _name)
		{
			_base()->name(_name.c_str(), _name.size());
		}

		void name(const char_type* _name)
		{
			_base()->name(_name.c_str());
		}

		void name(const char_type* _name, std::size_t _size)
		{
			_base()->name(_name, _size);
		}


		string_type value() const
		{
			return string_type(_base()->value(), _base()->value_size());
		}

		void value(const string_type& _value)
		{
			_base()->value(_value.c_str(), _value.size());
		}

		void value(const char_type* _value)
		{
			_base()->value(_value.c_str());
		}

		void value(const char_type* _value, std::size_t _size)
		{
			_base()->value(_value, _size);
		}


		impl_type* impl() const
		{
			return m_impl;
		}

		node_type parent() const;

	protected:
		impl_base(impl_type* _impl)
			: m_impl(_impl)
		{
			assert(_impl);
		}

		raw_base_type* _base() const
		{
			return m_impl;
		}

	private:
		impl_type* m_impl;
	};



}

template<typename _Ch>
class node
	: public detail::impl_base<_Ch, rapidxml::xml_node<_Ch>>
{
public:
	typedef detail::impl_base<_Ch, rapidxml::xml_node<_Ch>>	base_type;
	typedef typename base_type::char_type					char_type;
	typedef typename base_type::impl_type					impl_type;
	typedef typename base_type::string_type					string_type;
	typedef node<char_type>									self_type;
	typedef self_type*										this_type;
	typedef typename base_type::raw_base_type				raw_base_type;
	typedef typename base_type::raw_node_type				raw_node_type;
	typedef typename base_type::node_type					node_type;
	typedef typename base_type::attr_type					attr_type;

public:
	node(raw_node_type& node)
		: base_type(&node)
	{
	}

	node(raw_node_type* node)
		: base_type(node)
	{
	}


};



}














#endif