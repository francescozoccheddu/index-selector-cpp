#ifndef INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY
#define INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY

#include <vector>
#include <concepts>
#include <memory>
#include <initializer_list>
#include <iterator>
#include <cstddef>
#include <stdexcept>

namespace IndexSelector
{

	template<typename TValue>
	class ImmutableArray final
	{

		std::shared_ptr<TValue[]> m_data{};
		size_t m_nData{};

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			static  TValue* copy (const TConvertible* _pData, size_t _nData);

	public:

		class Iterator
		{

		private:

			friend class ImmutableArray;

			const TValue* m_p;

			Iterator (const TValue* _p);

		public:

			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = TValue;
			using pointer = TValue*;
			using reference = TValue&;

			const TValue& operator*() const;
			const TValue* operator->();
			Iterator& operator++();
			Iterator operator++(int);
			friend bool operator== (const Iterator& _a, const Iterator& _b) = default;

		};

		static ImmutableArray<TValue> take_ownership (TValue* _pData, size_t _nData);

		ImmutableArray () = default;

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			ImmutableArray (std::initializer_list<TConvertible> _data);

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			ImmutableArray (const TConvertible* _pData, size_t _nData);

		const TValue& operator[](size_t _index) const;

		size_t size () const;

		const TValue* data () const;
		const TValue* operator*() const;

		Iterator cbegin () const;
		Iterator cend ()   const;
		Iterator begin () const;
		Iterator end ()   const;

	};

}

namespace IndexSelector
{

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
	template<typename TConvertible>
	requires std::convertible_to<TConvertible, TValue>
		ImmutableArray<TValue>::ImmutableArray (std::initializer_list<TConvertible> _data) : ImmutableArray{ _data.begin (), _data.size () }
	{}

	template<typename TValue>
	template<typename TConvertible>
	requires std::convertible_to<TConvertible, TValue>
		ImmutableArray<TValue>::ImmutableArray (const TConvertible* _pData, size_t _nData) : m_data{ ImmutableArray::copy (_pData, _nData) }, m_nData{ _nData }
	{
		if (_nData < 0)
		{
			throw std::invalid_argument{ "Negative size" };
		}
	}
	
	template<typename TValue>
	ImmutableArray<TValue> ImmutableArray<TValue>::take_ownership (TValue* _pData, size_t _nData)
	{
		ImmutableArray<TValue> a;
		a.m_data = { _pData };
		a.m_nData = _nData;
		return a;
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

}

#endif