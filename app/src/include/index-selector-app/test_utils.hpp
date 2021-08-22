#ifndef INCLUDED_INDEX_SELECTOR_APP_TEST_UTILS
#define INCLUDED_INDEX_SELECTOR_APP_TEST_UTILS

#include <index-selector-app/test.hpp>
#include <index-selector/immutable_array.hpp>
#include <concepts>

namespace IndexSelector::App
{

	template<typename TValue>
	requires std::floating_point<TValue>
	ImmutableArray<TValue> range (TValue _min, TValue _max, size_t _count)
	{
		if (_count < 0)
		{
			throw new std::invalid_argument ("Negative count");
		}
		TValue* const r{ new TValue[_count] };
		for (size_t i{}; i < _count; i++)
		{
			r[i] = (_max - _min) * i / std::max<TValue> (_count - 1, 0) + _min;
		}
		return ImmutableArray<TValue>::takeOwnership (r, _count);
	}

	template<typename TValue>
	requires std::integral<TValue>
	ImmutableArray<TValue> range (TValue _min, TValue _max, size_t _count)
	{
		if (_count < 0)
		{
			throw new std::invalid_argument ("Negative count");
		}
		TValue* const r{ new TValue[_count] };
		for (size_t i{}; i < _count; i++)
		{
			r[i] = static_cast<TValue>(std::round ((static_cast<double>(_max) - _min)) * i / std::max<TValue> (_count - 1, 0) + _min);
		}
		return ImmutableArray<TValue>::takeOwnership (r, _count);
	}


	template<typename TObject, typename TMapper>
	ImmutableArray<TObject> map (ImmutableArray<TObject> _options, TMapper _setter)
	{
		TObject* const r{ new TObject[_options.size ()] };
		for (int c{}; c < _options.size (); c++)
		{
			r[c] = _setter (_options[c]);
		}
		return ImmutableArray<TObject>::takeOwnership (r, _options.size ());
	}

	template<typename TObject, typename TValue>
	ImmutableArray<TObject> explode (ImmutableArray<TObject> _options, ImmutableArray<TValue> _values, void (*_setter)(TObject&, TValue))
	{
		TObject* const r{ new TObject[_values.size () * _options.size ()] };
		int fi{ };
		for (TObject o : _options)
		{
			for (int c{}; c < _values.size(); c++)
			{
				_setter (o, _values[c]);
				r[fi++] = o;
			}
		}
		return ImmutableArray<TObject>::takeOwnership (r, _values.size () * _options.size ());
	}

}

#endif