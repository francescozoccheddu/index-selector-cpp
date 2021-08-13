#ifndef INCLUDED_INDEX_SELECTOR_LIB_IMMUTABLE_ARRAY
#define INCLUDED_INDEX_SELECTOR_LIB_IMMUTABLE_ARRAY

#ifndef INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY
#error  
#endif

#include <index-selector/immutable_array.hpp>
#include <stdexcept>

namespace IndexSelector
{

#pragma region Private

	template<typename TValue>
	template<typename TConvertible>
	requires std::convertible_to<TConvertible, TValue>
		 TValue* ImmutableArray<TValue>::copy (const TConvertible* _pData, size_t _nData)
	{
		if (_nData < 0)
		{
			throw std::invalid_argument{ "Negative size" };
		}
		TValue* pCopy = new TValue[_nData];
		while (_nData-- > 0)
		{
			pCopy[_nData] = static_cast<const TValue>(_pData[_nData]);
		}
		return pCopy;
	}

	template<typename TValue>
	 ImmutableArray<TValue>::ImmutableArray (TValue* _pData, size_t _nData) : m_data{ _pData }, m_nData{ _nData }
	{
		if (_nData < 0)
		{
			throw std::invalid_argument{ "Negative size" };
		}
	}


#pragma endregion

#pragma region Public

#pragma region Constructors

	template<typename TValue>
	template<typename TConvertible>
	requires std::convertible_to<TConvertible, TValue>
		 ImmutableArray<TValue>::ImmutableArray (std::initializer_list<TConvertible> _data) : ImmutableArray{ _data.begin (), _data.size () }
	{}

	template<typename TValue>
	template<typename TConvertible>
	requires std::convertible_to<TConvertible, TValue>
		 ImmutableArray<TValue>::ImmutableArray (const TConvertible* _pData, size_t _nData) : ImmutableArray{ ImmutableArray::copy (_pData, _nData), _nData }
	{}

#pragma endregion

#pragma region Methods

	template<typename TValue>
	 ImmutableArray<TValue> ImmutableArray<TValue>::take_ownership (TValue* _pData, size_t _nData)
	{
		return ImmutableArray{ _pData, _nData };
	}

	template<typename TValue>
	 const TValue* ImmutableArray<TValue>::data () const
	{
		return *m_data;
	}

	template<typename TValue>
	 const TValue* ImmutableArray<TValue>::operator* () const
	{
		return *m_data;
	}

	template<typename TValue>
	size_t ImmutableArray<TValue>::size () const
	{
		return m_nData;
	}

	template<typename TValue>
	 const TValue& ImmutableArray<TValue>::operator[](size_t _index) const
	{
		if (_index < 0 or _index >= m_nData)
		{
			throw std::invalid_argument{ "Index out of range" };
		}
		return m_data[_index];
	}

	template<typename TValue>
	 ImmutableArray<TValue>::Iterator ImmutableArray<TValue>::cbegin () const
	{
		return Iterator{ &m_data[0] };
	}

	template<typename TValue>
	 ImmutableArray<TValue>::Iterator ImmutableArray<TValue>::cend ()   const
	{
		return Iterator{ &m_data[m_nData] };
	}

	template<typename TValue>
	 ImmutableArray<TValue>::Iterator ImmutableArray<TValue>::begin () const
	{
		return cbegin ();
	}

	template<typename TValue>
	 ImmutableArray<TValue>::Iterator ImmutableArray<TValue>::end ()   const
	{
		return cend ();
	}

#pragma endregion

#pragma region Iterator

	template<typename TValue>
	 ImmutableArray<TValue>::Iterator::Iterator (const TValue* _p) : m_p{ _p }
	{}

	template<typename TValue>
	 const TValue& ImmutableArray<TValue>::Iterator::operator*() const
	{
		return *m_p;
	}
	template<typename TValue>
	 const TValue* ImmutableArray<TValue>::Iterator::operator->()
	{
		return m_p;
	}

	template<typename TValue>
	 ImmutableArray<TValue>::Iterator& ImmutableArray<TValue>::Iterator::operator++()
	{
		m_p++;
		return *this;
	}

	template<typename TValue>
	 ImmutableArray<TValue>::Iterator ImmutableArray<TValue>::Iterator::operator++(int)
	{
		ImmutableArray::Iterator tmp = *this;
		++(*this);
		return tmp;
	}

#pragma endregion

#pragma endregion

}

#endif