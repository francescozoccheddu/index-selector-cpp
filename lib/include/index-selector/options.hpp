#ifndef INCLUDED_INDEX_SELECTOR_OPTIONS
#define INCLUDED_INDEX_SELECTOR_OPTIONS

namespace IndexSelector
{

	struct Options final
	{

		bool prune{ true };
		bool enableSelectionCuts{ true };
		bool enableSizeCuts{ false };
		bool enableSizeOptimalCuts{ false };
		int heuristicSizeCutsMaxSize{ 10 };

		void validate () const;

	};

}

#endif