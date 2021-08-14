#ifndef INCLUDED_INDEX_SELECTOR_OPTIONS
#define INCLUDED_INDEX_SELECTOR_OPTIONS

#include <limits>

namespace IndexSelector
{

	struct Options final
	{

		enum class ESizeCutMode
		{
			None, Heuristic, Optimal
		};

		double timeLimit{ 1e+75 };
		double optimalSizeCutTimeLimit{ 2.0 };
		bool reduceVariables{ true };
		bool shareCutters{ true };
		bool enableSelectionCuts{ true };
		ESizeCutMode sizeCutMode{ ESizeCutMode::None };
		int heuristicSizeCutsMaxSize{ 10 };

		void validate () const;

	};

}

#endif