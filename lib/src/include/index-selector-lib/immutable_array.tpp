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

	template<typename Type>
	template<typename T>
	requires std::convertible_to<T, Type>
		 Type* ImmutableArray<Type>::copy (const T* _pData, int _nData)
	{
		if (_nData < 0)
		{
			throw std::invalid_argument{ "Negative size" };
		}
		Type* pCopy = new Type[_nData];
		while (_nData-- > 0)
		{
			pCopy[_nData] = static_cast<const Type>(_pData[_nData]);
		}
		return pCopy;
	}

	template<typename Type>
	 ImmutableArray<Type>::ImmutableArray (Type* _pData, int _nData) : m_data{ _pData }, size{ _nData }
	{
		if (_nData < 0)
		{
			throw std::invalid_argument{ "Negative size" };
		}
	}


#pragma endregion

#pragma region Public

#pragma region Constructors

	template<typename Type>
	template<typename T>
	requires std::convertible_to<T, Type>
		 ImmutableArray<Type>::ImmutableArray (const std::vector<T>& _data) : ImmutableArray{ _data.data (), static_cast<int>(_data.size ()) }
	{}

	template<typename Type>
	template<typename T, size_t N>
	requires std::convertible_to<T, Type>
		 ImmutableArray<Type>::ImmutableArray (const std::array<T, N>& _data) : ImmutableArray{ _data.data (), static_cast<int>(N) }
	{}

	template<typename Type>
	template<typename T>
	requires std::convertible_to<T, Type>
		 ImmutableArray<Type>::ImmutableArray (std::initializer_list<T> _data) : ImmutableArray{ _data.begin (), static_cast<int>(_data.size ()) }
	{}

	template<typename Type>
	template<typename T>
	requires std::convertible_to<T, Type>
		 ImmutableArray<Type>::ImmutableArray (const T* _pData, int _nData) : ImmutableArray{ ImmutableArray::copy (_pData, _nData), _nData }
	{}

#pragma endregion

#pragma region Methods

	template<typename Type>
	 ImmutableArray<Type> ImmutableArray<Type>::from_immutable_data (Type* _pData, int _nData)
	{
		return ImmutableArray{ _pData, _nData };
	}

	template<typename Type>
	 const Type* ImmutableArray<Type>::data () const
	{
		return m_data.get ();
	}

	template<typename Type>
	 const Type* ImmutableArray<Type>::operator* () const
	{
		return m_data.get ();
	}

	template<typename Type>
	 Type ImmutableArray<Type>::operator[](int _index) const
	{
		if (_index < 0 or _index >= size)
		{
			throw std::invalid_argument{ "Index out of range" };
		}
		return m_data.get ()[_index];
	}

	template<typename Type>
	 ImmutableArray<Type>::Iterator ImmutableArray<Type>::cbegin () const
	{
		return Iterator{ m_data.get () };
	}

	template<typename Type>
	 ImmutableArray<Type>::Iterator ImmutableArray<Type>::cend ()   const
	{
		return Iterator{ m_data.get () + size };
	}

	template<typename Type>
	 ImmutableArray<Type>::Iterator ImmutableArray<Type>::begin () const
	{
		return cbegin ();
	}

	template<typename Type>
	 ImmutableArray<Type>::Iterator ImmutableArray<Type>::end ()   const
	{
		return cend ();
	}

#pragma endregion

#pragma region Iterator

	template<typename Type>
	 ImmutableArray<Type>::Iterator::Iterator (const Type* _p) : m_p{ _p }
	{}

	template<typename Type>
	 const Type& ImmutableArray<Type>::Iterator::operator*() const
	{
		return *m_p;
	}
	template<typename Type>
	 const Type* ImmutableArray<Type>::Iterator::operator->()
	{
		return m_p;
	}

	template<typename Type>
	 ImmutableArray<Type>::Iterator& ImmutableArray<Type>::Iterator::operator++()
	{
		m_p++;
		return *this;
	}

	template<typename Type>
	 ImmutableArray<Type>::Iterator ImmutableArray<Type>::Iterator::operator++(int)
	{
		ImmutableArray::Iterator tmp = *this;
		++(*this);
		return tmp;
	}

#pragma endregion

#pragma endregion

}

#endif