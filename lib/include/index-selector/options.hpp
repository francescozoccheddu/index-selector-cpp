#ifndef INCLUDED_INDEX_SELECTOR_OPTIONS
#define INCLUDED_INDEX_SELECTOR_OPTIONS

namespace IndexSelector
{

	struct Options final
	{

		enum class ESizeCutMode
		{
			None, Heuristic, Optimal
		};

		bool reduceVariables{ true };
		bool enableSelectionCuts{ true };
		ESizeCutMode sizeCutMode{ ESizeCutMode::None };
		int heuristicSizeCutsMaxSize{ 10 };

		void validate () const;

	};

}

#endif