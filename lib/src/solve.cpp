#include <index-selector/solve.hpp>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector-lib/model.hpp>
#include <index-selector-lib/selection_cutter.hpp>
#include <index-selector-lib/optimal_size_cutter.hpp>
#include <index-selector-lib/heuristic_size_cutter.hpp>
#include <ilcplex/ilocplex.h>
#include <format>
#include <chrono>

#define INDEX_SELECTOR_DISABLE_CUTS 0
#define INDEX_SELECTOR_DISABLE_PRESOLVE 0
#define INDEX_SELECTOR_DISABLE_HEURISTICS 0
#define INDEX_SELECTOR_DISABLE_NON_PRIMAL_LP 0
#define INDEX_SELECTOR_DISABLE_OUT 1

namespace IndexSelector
{

	static const Solution solve (const IloEnv _env, const Problem& _problem, const Options& _options);

	const Solution solve (const Problem& _problem, const Options& _options)
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

	const Solution solve (const IloEnv _env, const Problem& _problem, const Options& _options)
	{
		Solution s{};
		VariableMatrix v{ _env, _problem, _options.reduceVariables };
		IloModel m{ create_model (_env, v, s.statistics) };
		IloCplex c{ _env };
#if INDEX_SELECTOR_DISABLE_OUT
		c.setOut (_env.getNullStream ());
		c.setError (_env.getNullStream ());
#endif
		c.setParam (IloCplex::Param::MIP::Strategy::Search, IloCplex::Traditional);
		c.setParam (IloCplex::Param::TimeLimit, _options.timeLimit);
		if (_options.additionalCuts != Options::EAdditionalCuts::All)
		{
			c.setParam (IloCplex::Param::MIP::Cuts::BQP, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::Cliques, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::Covers, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::Disjunctive, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::FlowCovers, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::GUBCovers, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::Implied, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::LiftProj, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::LocalImplied, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::MCFCut, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::MIRCut, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::Nodecuts, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::PathCut, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::RLT, -1);
			c.setParam (IloCplex::Param::MIP::Cuts::ZeroHalfCut, -1);
		}
		if (_options.additionalCuts == Options::EAdditionalCuts::None)
		{
			c.setParam (IloCplex::Param::MIP::Cuts::Gomory, -1);
		}
#if INDEX_SELECTOR_DISABLE_CUTS
		c.setParam (IloCplex::Param::MIP::Limits::CutsFactor, 0.0);
		c.setParam (IloCplex::Param::MIP::Limits::CutPasses, -1);
#endif
#if INDEX_SELECTOR_DISABLE_HEURISTICS
		c.setParam (IloCplex::Param::MIP::Strategy::HeuristicEffort, 0);
#endif
#if INDEX_SELECTOR_DISABLE_NON_PRIMAL_LP
		c.setParam (IloCplex::Param::RootAlgorithm, IloCplex::Algorithm::Primal);
		c.setParam (IloCplex::Param::MIP::SubMIP::StartAlg, IloCplex::Algorithm::Primal);
		c.setParam (IloCplex::Param::MIP::SubMIP::SubAlg, IloCplex::Algorithm::Primal);
		c.setParam (IloCplex::Param::NodeAlgorithm, IloCplex::Algorithm::Primal);
#endif
#if INDEX_SELECTOR_DISABLE_PRESOLVE
		c.setParam (IloCplex::Param::Preprocessing::Presolve, false);
#endif
		c.extract (m);
		Cutter::Manager selectionCutManager{ _env, v, _options };
		Cutter::Manager sizeCutManager{ _env, v, _options };
		std::vector<IloCplex::Callback> callbacks;
		if (_options.enableSelectionCuts)
		{
			callbacks.push_back (c.add (SelectionCutter::createAndGetCallback (selectionCutManager)));
		}
		switch (_options.sizeCutMode)
		{
			case Options::ESizeCutMode::Optimal:
				callbacks.push_back (c.add (OptimalSizeCutter::createAndGetCallback (sizeCutManager)));
				break;
			case Options::ESizeCutMode::Heuristic:
				callbacks.push_back (c.add (HeuristicSizeCutter::createAndGetCallback (sizeCutManager)));
				break;
		}
		std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now ();
		s.succeeded = c.solve ();
		for (IloCplex::Callback cb : callbacks)
		{
			cb.end ();
		}
		std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now ();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		s.statistics.nSelectionCuts = selectionCutManager.nCuts ();
		s.statistics.nSizeCuts = sizeCutManager.nCuts ();
		s.statistics.selectionCutElapsedTime = selectionCutManager.elapsedTime ();
		s.statistics.sizeCutElapsedTime = sizeCutManager.elapsedTime ();
		s.statistics.totalElapsedTime = elapsedTime.count ();
		s.statistics.nNodes = static_cast<size_t>(c.getNnodes ());
		s.statistics.nVariables = static_cast<size_t>(c.getNbinVars ());
		if (s.succeeded)
		{
			const size_t ni{ _problem.nIndices () }, nq{ _problem.nQueries () };
			s.cost = static_cast<Real>(c.getObjValue ());
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
				s.bestIndexPerQuery = ImmutableArray<size_t>::takeOwnership (pIXs, nq);
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