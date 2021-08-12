#ifndef INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY
#define INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY

#include <vector>
#include <concepts>
#include <memory>
#include <initializer_list>
#include <iterator>

namespace IndexSelector
{

	template<typename TValue>
	class ImmutableArray final
	{

	private:

		std::shared_ptr<TValue> m_data{};
		int m_nData{};

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			static  TValue* copy (const TConvertible* _pData, int _nData);

		ImmutableArray (TValue* _pData, int _nData);

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

		static ImmutableArray<TValue> from_immutable_data (TValue* _pData, int _nData);

		ImmutableArray () = default;

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			ImmutableArray (std::initializer_list<TConvertible> _data);

		template<typename TConvertible>
		requires std::convertible_to<TConvertible, TValue>
			ImmutableArray (const TConvertible* _pData, int _nData);

		ImmutableArray (const ImmutableArray& _copy) = default;
		ImmutableArray (ImmutableArray&& _moved) = default;

		TValue operator[](int _index) const;

		int size () const;

		const TValue* data () const;
		const TValue* operator*() const;

		ImmutableArray& operator=(const ImmutableArray& _copy) = default;
		ImmutableArray& operator=(ImmutableArray&& _moved) = default;

		Iterator cbegin () const;
		Iterator cend ()   const;
		Iterator begin () const;
		Iterator end ()   const;

	};

}

#include "../../src/include/index-selector-lib/immutable_array.tpp"
#endif