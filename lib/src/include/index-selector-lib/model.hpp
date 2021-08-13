#ifndef INCLUDED_INDEX_SELECTOR_LIB_MODEL
#define INCLUDED_INDEX_SELECTOR_LIB_MODEL

#include <ilcplex/ilocplex.h>
#include <index-selector-lib/variable_matrix.hpp>

namespace IndexSelector
{

	IloModel create_model (const IloEnv _env, const VariableMatrix& _variables);

}

#endif