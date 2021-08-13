#ifndef INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY
#define INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY

#include <vector>
#include <concepts>
#include <memory>
#include <initializer_list>
#include <iterator>
#include <cstddef>

namespace IndexSelector
{

	template<typename TValue>
	class ImmutableArray final
	{

		std::shared_ptr<TValue> m_data{};
		size_t m_nData{};

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			static  TValue* copy (const TConvertible* _pData, size_t _nData);

		ImmutableArray (TValue* _pData, size_t _nData);

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

		static ImmutableArray<TValue> from_immutable_data (TValue* _pData, size_t _nData);

		ImmutableArray () = default;

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			ImmutableArray (std::initializer_list<TConvertible> _data);

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			ImmutableArray (const TConvertible* _pData, size_t _nData);

		TValue operator[](size_t _index) const;

		size_t size () const;

		const TValue* data () const;
		const TValue* operator*() const;

		Iterator cbegin () const;
		Iterator cend ()   const;
		Iterator begin () const;
		Iterator end ()   const;

	};

}

#include "../../src/include/index-selector-lib/immutable_array.tpp"
#endif