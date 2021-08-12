#ifndef INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY
#define INCLUDED_INDEX_SELECTOR_IMMUTABLE_ARRAY

#include <vector>
#include <concepts>
#include <memory>
#include <initializer_list>
#include <iterator>

namespace IndexSelector
{

	template<typename Type>
	class ImmutableArray final
	{

	private:

		const std::shared_ptr<Type> m_data;
		const int m_nData;

		template<typename T>
		requires std::convertible_to<T, Type>
			static Type* copy (const T* _pData, int _nData);

		ImmutableArray (Type* _pData, int _nData);

	public:

		class Iterator
		{

		private:

			friend class ImmutableArray;

			const Type* m_p;

			Iterator (const Type* _p) : m_p{ _p }
			{}

		public:


			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Type;
			using pointer = Type*;
			using reference = Type&;

			const Type& operator*() const;
			const Type* operator->();
			Iterator& operator++();
			Iterator operator++(int);
			friend bool operator== (const Iterator& _a, const Iterator& _b) = default;

		};

		static ImmutableArray<Type> from_immutable_data (Type* _pData, int _nData);

		template<typename T>
		requires std::convertible_to<T, Type>
			ImmutableArray (const std::vector<T>& _data);

		template<typename T, size_t N>
		requires std::convertible_to<T, Type>
			ImmutableArray (const std::array<T, N>& _data);

		template<typename T>
		requires std::convertible_to<T, Type>
			ImmutableArray (std::initializer_list<T> _data);

		template<typename T>
		requires std::convertible_to<T, Type>
			ImmutableArray (const T* _pData, int _nData);

		ImmutableArray (const ImmutableArray& _copy) = default;
		ImmutableArray (ImmutableArray&& _moved) = default;

		Type operator[](int _index) const;

		int size () const;

		const Type* data () const;
		const Type* operator*() const;

		Iterator cbegin () const;
		Iterator cend ()   const;
		Iterator begin () const;
		Iterator end ()   const;

	};

}

#include <index-selector-lib/immutable_array.tpp>
#endif