#pragma once
#ifndef _RXML_ITERATORS_HPP
#define _RXML_ITERATORS_HPP

#include <rapidxml.hpp>
#include <iterator>
#include "error.hpp"


namespace rxml {

namespace detail {

	
	template<	typename _EntTy,	// type of entity iterated
				typename _ValTy,	// type of value to access
				typename _NextGet,	// type of function getting the next element
				typename _Select,	// type of function selecting entity
				typename _ValEx>	// type of function extracting the value
	class forward_iterator_base
	{
	private:
		typedef _EntTy		_entity_type;
		typedef _Select		_select_type;
		typedef _NextGet	_nextget_type;
		typedef _ValEx		_value_extractor;
	public:

		typedef _ValTy			value_type;
		typedef value_type&		reference;
		typedef value_type*		pointer;

		forward_iterator_base(_entity_type entity, const _select_type& selector)
			: m_entitiy(entity)
			, m_value(entity)
			, m_select(selector)
		{
		}

		forward_iterator_base(const forward_iterator_base& other)
			: m_entitiy(other.m_entitiy)
			, m_value(other.m_value)
			, m_select(other.m_select)
		{
		}

		// Don't need that?
		/*forward_iterator_base(forward_iterator_base&& other)
			: m_entitiy(other.m_entitiy)
			, m_value(std::move(other.m_value))
			, m_select(std::move(other.m_select))
		{
		}*/

		reference operator *()
		{
			return m_value.get_ref(m_entitiy);
		}

		pointer operator->()
		{
			return m_value.get_ptr(m_entitiy);
		}

		reference operator *() const
		{
			return m_value.get_ref(m_entitiy);
		}

		pointer operator->() const
		{
			return m_value.get_ptr(m_entitiy);
		}


		bool operator ==(const forward_iterator_base& other) const
		{
			return m_entitiy == other.m_entitiy;
		}

		bool operator !=(const forward_iterator_base& other) const
		{
			return m_entitiy != other.m_entitiy;
		}

	protected:
		void _next()
		{
			_nextget_type next;
			do {
				m_entitiy = next(m_entitiy);

			} while(!m_select(m_entitiy));
			_set();
		}

		void _set()
		{
			m_value.set(m_entitiy);
		}

	protected:
		_select_type		m_select;
		_value_extractor	m_value;
		_entity_type		m_entitiy;
	};


	template<	typename _EntTy,	// type of entity iterated
				typename _ValTy,	// type of value to access
				typename _NextGet,	// type of function getting the next element
				typename _PrevGet,	// type of function getting the prev element
				typename _Select,	// type of function selecting entity
				typename _ValEx>	// type of function extracting the value
	class backward_iterator_base
		: public forward_iterator_base<_EntTy, _ValTy, _NextGet, _Select, _ValEx>
	{
	private:
		typedef _EntTy		_entity_type;
		typedef _PrevGet	_prevget_type;
		typedef _Select		_select_type;
	public:
		typedef forward_iterator_base<_EntTy, _ValTy, _NextGet, _Select, _ValEx>	base_type;
		typedef typename base_type::value_type										value_type;
		typedef typename base_type::reference										reference;
		typedef typename base_type::pointer											pointer;

		backward_iterator_base(_entity_type entity, const _select_type& selector)
			: base_type(entity, selector)
		{
		}

	protected:
		void _prev()
		{
			_prevget_type prev;
			do {
				this->m_entitiy = prev(this->m_entitiy);

			} while(!this->m_select(this->m_entitiy));
			this->_set();
		}
	};

	template<typename _Entity>
	struct trivial_everything_selector
	{
		bool operator ()(_Entity* e)
		{
			return true;
		}
	};

	template<typename _Entity>
	struct trivial_ptr_value
	{
		trivial_ptr_value(_Entity*)
		{
		}

		void set(_Entity*)
		{
		}

		_Entity* get_ptr(_Entity* entity)
		{
			rxml_assert(entity);
			return entity;
		}

		_Entity& get_ref(_Entity* entity)
		{
			rxml_assert(entity);
			return *entity;
		}
	};

	template<typename _Entity>
	struct trivial_next_node_getter
	{
		_Entity* operator ()(_Entity* e)
		{
			rxml_assert(e);
			return e->next_sibling();
		}
	};

	template<typename _Entity>
	struct trivial_prev_node_getter
	{
		_Entity* operator ()(_Entity* e)
		{
			rxml_assert(e && e->previous_sibling());
			return e->previous_sibling();
		}
	};

	template<typename _Entity>
	struct trivial_next_attr_getter
	{
		_Entity* operator ()(_Entity* e)
		{
			rxml_assert(e);
			return e->next_attribute();
		}
	};

	template<typename _Entity>
	struct trivial_prev_attr_getter
	{
		_Entity* operator ()(_Entity* e)
		{
			rxml_assert(e && e->previous_attribute());
			return e->previous_attribute();
		}
	};

#define RXML_BUILD_ITERATOR_SELECTOR(_name, ...)	struct _name: public __VA_ARGS__ { typedef __VA_ARGS__ base_type;
#define RXML_END_SELECTOR()							};


	template<typename _Entity>
	RXML_BUILD_ITERATOR_SELECTOR(node_iterator_base, backward_iterator_base
													<
														_Entity*,
														_Entity,
														trivial_next_node_getter<_Entity>,
														trivial_prev_node_getter<_Entity>,
														trivial_everything_selector<_Entity>,
														trivial_ptr_value<_Entity>
													>)
		node_iterator_base(_Entity* entity)
			: base_type(entity, trivial_everything_selector<_Entity>())
		{
		}
	RXML_END_SELECTOR();

	
	template<typename _Entity>
	RXML_BUILD_ITERATOR_SELECTOR(attr_iterator_base, backward_iterator_base
													<
														_Entity*,
														_Entity,
														trivial_next_attr_getter<_Entity>,
														trivial_prev_attr_getter<_Entity>,
														trivial_everything_selector<_Entity>,
														trivial_ptr_value<_Entity>
													>)
		attr_iterator_base(_Entity* entity)
			: base_type(entity, trivial_everything_selector<_Entity>())
		{
		}
	RXML_END_SELECTOR();
}


#define RXML_ADD_INC_TO_ITERATOR(_cls)	_cls& operator++()		{ this->_next(); return *this; }	\
										_cls  operator++(int)	{ auto tmp = std::move(*this); this->_next(); return tmp; }
#define RXML_ADD_DEC_TO_ITERATOR(_cls)	_cls& operator--()		{ this->_prev(); return *this; }	\
										_cls  operator--(int)	{ auto tmp = std::move(*this); this->_prev(); return tmp; }

// ########################################### node_iterator ###########################################
template<typename _Ch = char>
class node_iterator
	: public detail::node_iterator_base<rapidxml::xml_node<_Ch>>
	, public std::iterator<std::bidirectional_iterator_tag, rapidxml::xml_node<_Ch>>
{
public:
	typedef detail::node_iterator_base<rapidxml::xml_node<_Ch>> base_type;

	node_iterator() : base_type(nullptr) {}
	node_iterator(rapidxml::xml_node<_Ch>* node) : base_type(node->first_node()) {}
	node_iterator(rapidxml::xml_node<_Ch>& node) : base_type(node.first_node()) {}

	RXML_ADD_INC_TO_ITERATOR(node_iterator<_Ch>);
	RXML_ADD_DEC_TO_ITERATOR(node_iterator<_Ch>);
};

// ########################################### const_node_iterator###########################################
template<typename _Ch = char>
class const_node_iterator
	: public detail::node_iterator_base<const rapidxml::xml_node<_Ch>>
	, public std::iterator<std::bidirectional_iterator_tag, const rapidxml::xml_node<_Ch>>
{
public:
	typedef detail::node_iterator_base<const rapidxml::xml_node<_Ch>> base_type;

	const_node_iterator() : base_type(nullptr) {}
	const_node_iterator(const rapidxml::xml_node<_Ch>* node) : base_type(node->first_node()) {}
	const_node_iterator(const rapidxml::xml_node<_Ch>& node) : base_type(node.first_node()) {}

	RXML_ADD_INC_TO_ITERATOR(const_node_iterator<_Ch>);
	RXML_ADD_DEC_TO_ITERATOR(const_node_iterator<_Ch>);
};

// ########################################### attribute_iterator ###########################################
template<typename _Ch = char>
class attribute_iterator
	: public detail::attr_iterator_base<rapidxml::xml_attribute<_Ch>>
	, public std::iterator<std::bidirectional_iterator_tag, rapidxml::xml_attribute<_Ch>>
{
public:
	typedef detail::attr_iterator_base<rapidxml::xml_attribute<_Ch>> base_type;

	attribute_iterator() : base_type(nullptr) {}
	attribute_iterator(rapidxml::xml_node<_Ch>* node) : base_type(node->first_attribute()) {}
	attribute_iterator(rapidxml::xml_node<_Ch>& node) : base_type(node.first_attribute()) {}

	RXML_ADD_INC_TO_ITERATOR(attribute_iterator<_Ch>);
	RXML_ADD_DEC_TO_ITERATOR(attribute_iterator<_Ch>);
};

// ########################################### const_attribute_iterator###########################################
template<typename _Ch = char>
class const_attribute_iterator
	: public detail::attr_iterator_base<const rapidxml::xml_attribute<_Ch>>
	, public std::iterator<std::bidirectional_iterator_tag, const rapidxml::xml_attribute<_Ch>>
{
public:
	typedef detail::attr_iterator_base<const rapidxml::xml_attribute<_Ch>> base_type;

	const_attribute_iterator() : base_type(nullptr) {}
	const_attribute_iterator(const rapidxml::xml_node<_Ch>* node) : base_type(node->first_attribute()) {}
	const_attribute_iterator(const rapidxml::xml_node<_Ch>& node) : base_type(node.first_attribute()) {}

	RXML_ADD_INC_TO_ITERATOR(const_attribute_iterator<_Ch>);
	RXML_ADD_DEC_TO_ITERATOR(const_attribute_iterator<_Ch>);
};






















// ###########################################  ###########################################
namespace detail
{
	template<typename _Entity, typename Iter>
	class simple_range_wrapper
	{
	public:
		typedef Iter iterator_type;

		simple_range_wrapper(_Entity entity)
			: m_entity(entity)
		{
		}


		iterator_type begin()
		{
			return iterator_type(m_entity);
		}

		iterator_type end()
		{
			return iterator_type();
		}

	private:
		_Entity const m_entity;
	};
}

template<typename _Ch>
detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, node_iterator<_Ch>>
	children(rapidxml::xml_node<_Ch>* node)
{
	assert(node);
	return detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, node_iterator<_Ch>>(node);
}

template<typename _Ch>
detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_node_iterator<_Ch>>
	children(const rapidxml::xml_node<_Ch>* node)
{
	assert(node);
	return detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_node_iterator<_Ch>>(node);
}

template<typename _Ch>
detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, node_iterator<_Ch>>
	children(rapidxml::xml_node<_Ch>& node)
{
	return detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, node_iterator<_Ch>>(&node);
}

template<typename _Ch>
detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_node_iterator<_Ch>>
	children(const rapidxml::xml_node<_Ch>& node)
{
	return detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_node_iterator<_Ch>>(&node);
}




template<typename _Ch>
detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, attribute_iterator<_Ch>>
	attributes(rapidxml::xml_node<_Ch>* node)
{
	assert(node);
	return detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, attribute_iterator<_Ch>>(node);
}

template<typename _Ch>
detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_attribute_iterator<_Ch>>
	attributes(const rapidxml::xml_node<_Ch>* node)
{
	assert(node);
	return detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_attribute_iterator<_Ch>>(node);
}

template<typename _Ch>
detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, attribute_iterator<_Ch>>
	attributes(rapidxml::xml_node<_Ch>& node)
{
	return detail::simple_range_wrapper<rapidxml::xml_node<_Ch>*, attribute_iterator<_Ch>>(&node);
}

template<typename _Ch>
detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_attribute_iterator<_Ch>>
	attributes(const rapidxml::xml_node<_Ch>& node)
{
	return detail::simple_range_wrapper<const rapidxml::xml_node<_Ch>*, const_attribute_iterator<_Ch>>(&node);
}




}









#endif
