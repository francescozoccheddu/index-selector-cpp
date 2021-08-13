#include <index-selector/solve.hpp>
#include <index-selector-lib/variable_matrix.hpp>
#include <ilcplex/ilocplex.h>

namespace IndexSelector
{

	static Solution solve (const Problem& _problem, const Options& _options, IloEnv& _env);

	constexpr const char* qKey{ "q" }, *iKey{ "i" };

	Solution solve (const Problem& _problem, const Options& _options)
	{
		_problem.validate ();
		_options.validate ();
		IloEnv env{};
		Solution solution;
		try
		{
			solution = solve (_problem, _options, env);
		}
		catch (...)
		{
			env.end ();
			throw;
		}
		env.end ();
		return solution;
	}

	Solution solve (const Problem& _problem, const Options& _options, IloEnv& _env)
	{
		VariableMatrix v{ _env, _problem, _options.prune };
		
		return Solution ();
	}

}