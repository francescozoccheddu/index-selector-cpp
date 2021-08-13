#include <index-selector-lib/cuts.hpp>
#include <iostream>

namespace IndexSelector
{

	ILOUSERCUTCALLBACK2 (create_cb, const VariableMatrix&, _variables, const Options&, _options)
	{
		std::cout << "Hello from the callback!" << std::endl;
	}

	IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options)
	{
		return create_cb (_env, _variables, _options);
	}

}