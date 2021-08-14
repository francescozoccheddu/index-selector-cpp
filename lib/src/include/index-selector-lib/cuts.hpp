#ifndef INCLUDED_INDEX_SELECTOR_LIB_CUTS
#define INCLUDED_INDEX_SELECTOR_LIB_CUTS

#include <ilcplex/ilocplex.h>
#include <forward_list>
#include <mutex>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector/options.hpp>
#include <index-selector/solution.hpp>

namespace IndexSelector
{

	class CutCallbackI final : public IloCplex::UserCutCallbackI
	{

		const VariableMatrix* m_pVariables;
		const Options* m_pOptions;
		Solution::Statistics* m_pStatistics;

		struct SelectionIndex
		{
			IloBoolVar y;
			std::forward_list<IloBoolVar> xs;
		};

		std::forward_list<SelectionIndex> m_selectionIndices;
		std::mutex m_selectionMutex;

	public:

		IloCplex::CallbackI* duplicateCallback () const override;

		CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics);
		CutCallbackI (const CutCallbackI&);

		void main () override;

		void add_selection_cuts ();

		void add_size_cuts ();

	};

	IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics);

}

#endif