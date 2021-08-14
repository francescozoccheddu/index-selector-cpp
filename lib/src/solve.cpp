#include <index-selector/solve.hpp>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector-lib/model.hpp>
#include <index-selector-lib/cuts.hpp>
#include <ilcplex/ilocplex.h>
#include <format>
#include <chrono>

namespace IndexSelector
{

	static Solution solve (const IloEnv _env, const Problem& _problem, const Options& _options);

	Solution solve (const Problem& _problem, const Options& _options)
	{
		_problem.validate ();
		_options.validate ();
		IloEnv env{};
		Solution solution;
		try
		{
			solution = solve (env, _problem, _options);
		}
		catch (...)
		{
			env.end ();
			throw;
		}
		env.end ();
		return solution;
	}

	Solution solve (const IloEnv _env, const Problem& _problem, const Options& _options)
	{
		Solution s{};
		VariableMatrix v{ _env, _problem, _options.prune };
		IloModel m{ create_model (_env, v) };
		IloCplex c{ _env };
		c.extract (m);
#ifdef NDEBUG 
		c.setOut (_env.getNullStream ());
#endif 
		c.setParam (IloCplex::Param::MIP::Strategy::Search, IloCplex::Traditional);
		c.use (create_cut_callback (_env, v, _options, s.statistics));
		std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now ();
		c.solve ();
		std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now ();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		s.statistics.totalElapsedTime = elapsedTime.count ();
		s.statistics.nNodes = static_cast<size_t>(c.getNnodes ());
		s.statistics.nVariables = static_cast<size_t>(c.getNbinVars ());
		s.totalCost = static_cast<Real>(c.getObjValue ());
		return s;
	}

}