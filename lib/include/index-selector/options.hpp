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

		enum class ECutManagement
		{
			CannotPurge, CanPurgeLater, CanFilter, Local
		};

		enum class EAdditionalCuts
		{
			All, GomoryOnly, None
		};

		double timeLimit{ 60.0 * 60.0 * 24.0 * 365.0 };
		bool reduceVariables{ true };
		bool shareSelectionCutter{ false };
		bool shareSizeCutter{ false };
		bool enableSelectionCuts{ true };
		ESizeCutMode sizeCutMode{ ESizeCutMode::None };
		int nMaxHeuristicSizeCutVars{ 10 };
		double sizeCutTimeLimit{ 0.5 };
		int nMaxSizeCuts{ 20 };
		bool enableHeuristicSizeCutHeuristics{ true };
		ECutManagement sizeCutManagement{ ECutManagement::CanFilter };
		ECutManagement selectionCutManagement{ ECutManagement::CannotPurge };
		EAdditionalCuts additionalCuts{ EAdditionalCuts::None };
		bool presolve{ false };
		bool enableHeuristics{ false };
		bool disableNonPrimalLP{ false };

		void validate () const;

	};

}

#endif