#ifndef INCLUDED_INDEX_SELECTOR_LIB_CUTS
#define INCLUDED_INDEX_SELECTOR_LIB_CUTS

#include <ilcplex/ilocplex.h>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector/options.hpp>

namespace IndexSelector
{

	class CutCallbackI final : public IloCplex::UserCutCallbackI
	{

		const VariableMatrix& m_variables;
		const Options& m_options;

	public:

		IloCplex::CallbackI* duplicateCallback () const override;

		CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options);

		void main () override;
	};

	IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options);

}

#endif