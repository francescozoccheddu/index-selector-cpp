#ifndef INCLUDED_INDEX_SELECTOR_OPTIONS
#define INCLUDED_INDEX_SELECTOR_OPTIONS

namespace IndexSelector
{

	struct Options final
	{

		bool prune;
		bool enableSelectionCuts;
		bool enableSizeCuts;
		bool enableSizeOptimalCuts;
		int heuristicSizeCutsMaxSize;

		void validate ();

	};

}

#endif