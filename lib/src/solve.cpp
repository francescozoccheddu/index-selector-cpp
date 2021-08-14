#include <index-selector/solve.hpp>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector-lib/model.hpp>
#include <index-selector-lib/selection_cutter.hpp>
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
		VariableMatrix v{ _env, _problem, _options.reduceVariables };
		IloModel m{ create_model (_env, v, s.statistics) };
		IloCplex c{ _env };
		c.extract (m);
#ifdef NDEBUG 
		c.setOut (_env.getNullStream ());
#endif 
		c.setParam (IloCplex::Param::MIP::Strategy::Search, IloCplex::Traditional);
		c.setParam (IloCplex::Param::TimeLimit, _options.timeLimit);
		//c.use (create_cut_callback (_env, v, _options, s.statistics));
		std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now ();
		s.succeeded = c.solve ();
		std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now ();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		s.statistics.totalElapsedTime = elapsedTime.count ();
		s.statistics.nNodes = static_cast<size_t>(c.getNnodes ());
		s.statistics.nVariables = static_cast<size_t>(c.getNbinVars ());
		if (s.succeeded)
		{
			const size_t ni{ _problem.nIndices () }, nq{ _problem.nQueries () };
			s.totalCost = static_cast<Real>(c.getObjValue ());
			{
				size_t* const pIXs = new size_t[nq];
				for (size_t q{ 0 }; q < nq; q++)
				{
					const std::optional<IloBoolVar>& ux{ v.ux (q) };
					if (ux and c.getValue (*ux))
					{
						pIXs[q] = -1;
						continue;
					}
					for (size_t i{ 0 }; i < ni; i++)
					{
						const std::optional<IloBoolVar>& x{ v.x (i, q) };
						if (x and c.getValue (*x))
						{
							pIXs[q] = i;
							break;
						}
					}
				}
				s.bestIndexPerQuery = ImmutableArray<size_t>::take_ownership (pIXs, nq);
			}
			{
				std::vector<size_t> iYs{};
				for (size_t i{ 0 }; i < ni; i++)
				{
					const std::optional<IloBoolVar>& y{ v.y (i) };
					if (y and c.getValue (*y))
					{
						iYs.push_back (i);
					}
				}
				s.enabledIndices = ImmutableArray<size_t>{ iYs.data (), iYs.size () };
			}
		}
		return s;
	}

}