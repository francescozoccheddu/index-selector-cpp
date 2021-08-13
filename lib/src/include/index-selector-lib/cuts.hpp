#ifndef INCLUDED_INDEX_SELECTOR_LIB_CUTS
#define INCLUDED_INDEX_SELECTOR_LIB_CUTS

#include <ilcplex/ilocplex.h>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector/options.hpp>

namespace IndexSelector
{

	IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options);

}

#endif