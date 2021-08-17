#ifndef INCLUDED_INDEX_SELECTOR_LIB_HEURISTIC_SIZE_CUTTER
#define INCLUDED_INDEX_SELECTOR_LIB_HEURISTIC_SIZE_CUTTER

#include <index-selector-lib/cutter.hpp>
#include <index-selector/immutable_array.hpp>
#include <vector>
#include <index-selector/types.hpp>

#define INDEX_SELECTOR_HEURISTIC_SIZE_CUTTER_ENABLE_HEURISTICS 1

namespace IndexSelector
{

	class HeuristicSizeCutter final : public SimpleCutter<HeuristicSizeCutter>
	{

		struct Candidate
		{
			Real size;
			IloBoolVar y;
#if INDEX_SELECTOR_HEURISTIC_SIZE_CUTTER_ENABLE_HEURISTICS
			size_t minK{};
#endif
			Real value;
		};

		std::vector<Candidate> m_candidates;
		size_t m_minK{}, m_maxK{};
		size_t m_remainingCuts;
		std::vector<Candidate*> m_valueSortedCandidates;
		std::vector<Candidate*> m_kCandidates;
		std::vector<size_t> m_chosen;

		void choose (Callback& _callback, size_t _k);
		bool pushChosen (Callback& _callback);

	protected:

		void cut (Callback& _callback) override;
		bool shouldShare () const override;

	public:

		HeuristicSizeCutter (Cutter::Manager& _manager);

	};

}

#endif