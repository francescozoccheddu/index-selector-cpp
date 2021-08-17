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

		enum class ESizeCutManagement
		{
			CannotPurge, CanPurgeLater, CanFilter
		};

		double timeLimit{ 60.0 * 60.0 * 24.0 * 365.0 };
		bool reduceVariables{ true };
		bool shareSelectionCutter{ true };
		bool shareSizeCutter{ true };
		bool enableSelectionCuts{ true };
		ESizeCutMode sizeCutMode{ ESizeCutMode::None };
		int nMaxHeuristicSizeCutsVars{ 10 };
		double sizeCutTimeLimit{ 0.5 };
		int nMaxSizeCuts{ 20 };
		ESizeCutManagement sizeCutManagement{ ESizeCutManagement::CannotPurge };

		void validate () const;

	};

}

#endif