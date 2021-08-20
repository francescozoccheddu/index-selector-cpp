#ifndef INCLUDED_INDEX_SELECTOR_APP_TEST
#define INCLUDED_INDEX_SELECTOR_APP_TEST

#include <index-selector-app/random_problem.hpp>
#include <index-selector/immutable_array.hpp>
#include <index-selector/options.hpp>
#include <ostream>
#include <iostream>
#include <type_traits>

namespace IndexSelector::App
{

	struct TestOptions final
	{
		enum class EStatistics
		{
			TotalTime = 1 << 0,
			SelectionCutTime = 1 << 1,
			SizeCutTime = 1 << 2,
			Nodes = 1 << 3,
			Variables = 1 << 4,
			Constraints = 1 << 5,
			SelectionCutCount = 1 << 6,
			SizeCutCount = 1 << 7,
			Success = 1 << 8
		};

		static const EStatistics defaultStatistics;

		RandomProblemOptions problem{};
		int nMinIndices{ 10 }, nMaxIndices{ 100 };
		int nTests{ 10 };
		double queriesRatio{ 1.5 };
		std::ostream* progressOut{ &std::cout }, * csvOut{ nullptr };
		ImmutableArray<Options> configs{ Options{} };
		EStatistics statistics{ defaultStatistics };

		void validate () const;

	};

	inline TestOptions::EStatistics operator | (TestOptions::EStatistics _lhs, TestOptions::EStatistics _rhs)
	{
		using T = std::underlying_type_t <TestOptions::EStatistics>;
		return static_cast<TestOptions::EStatistics>(static_cast<T>(_lhs) | static_cast<T>(_rhs));
	}

	inline TestOptions::EStatistics& operator |= (TestOptions::EStatistics& _lhs, TestOptions::EStatistics _rhs)
	{
		_lhs = _lhs | _rhs;
		return _lhs;
	}

	inline TestOptions::EStatistics operator & (TestOptions::EStatistics _lhs, TestOptions::EStatistics _rhs)
	{
		using T = std::underlying_type_t <TestOptions::EStatistics>;
		return static_cast<TestOptions::EStatistics>(static_cast<T>(_lhs) & static_cast<T>(_rhs));
	}

	inline TestOptions::EStatistics& operator &= (TestOptions::EStatistics& _lhs, TestOptions::EStatistics _rhs)
	{
		_lhs = _lhs & _rhs;
		return _lhs;
	}

	inline TestOptions::EStatistics operator ~ (TestOptions::EStatistics _x)
	{
		using T = std::underlying_type_t <TestOptions::EStatistics>;
		return static_cast<TestOptions::EStatistics>(~static_cast<T>(_x));
	}

	inline bool has (TestOptions::EStatistics _src, TestOptions::EStatistics _what)
	{
		return (_src & _what) == _what;
	}

	void test (const TestOptions& _options);

}

#endif