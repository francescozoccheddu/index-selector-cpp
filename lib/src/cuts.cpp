#include <index-selector-lib/cuts.hpp>
#include <iostream>

namespace IndexSelector
{

	IloCplex::CallbackI* CutCallbackI::duplicateCallback () const
	{
		return new (getEnv ()) CutCallbackI{ *this };
	}

	CutCallbackI::CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options) : IloCplex::UserCutCallbackI (_env), m_variables (_variables), m_options (_options)
	{}

	void CutCallbackI::main ()
	{
		std::cout << "Hello from the callback" << std::endl;
	}

	IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options)
	{
		return IloCplex::Callback{ new (_env) CutCallbackI {_env, _variables, _options} };
	}

}