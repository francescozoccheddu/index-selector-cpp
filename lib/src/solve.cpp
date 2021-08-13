#include <index-selector/solve.hpp>
#include <index-selector-lib/variable_matrix.hpp>
#include <ilcplex/ilocplex.h>

namespace IndexSelector
{

	static Solution solve (const Problem& _problem, const Options& _options, const IloEnv& _env);
	static IloModel create_model (const Problem& _problem, const VariableMatrix& _variables, const IloEnv& _env);

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

	Solution solve (const Problem& _problem, const Options& _options, const IloEnv& _env)
	{
		Solution s{};
		VariableMatrix v{ _env, _problem, _options.prune };
		IloModel m{ create_model (_problem, v, _env) };
		IloCplex c{ _env };
		c.extract (m);
		c.setParam (IloCplex::Param::MIP::Strategy::Search, IloCplex::Traditional);
		c.solve ();
		c.exportModel ("C:/Users/franc/Desktop/ciao.lp");
		s.totalCost = static_cast<Real>(c.getObjValue ());
		return s;
	}

	IloModel create_model (const Problem& _problem, const VariableMatrix& _variables, const IloEnv& _env)
	{
		IloModel model{ _env };
		if (_problem.nIndices () > 0)
		{
			IloExpr actualSize{ _env };
			for (int i{ 0 }; i < _problem.nIndices (); i++)
			{
				const std::optional<IloBoolVar>& y{ _variables.y (i) };
				if (y)
				{
					actualSize += *y * _problem.indices[i].size;
				}
			}
			model.add (actualSize <= _problem.maxSize);
			actualSize.end ();
		}
		for (int q{ 0 }; q < _problem.nQueries (); q++)
		{
			IloExpr nX{ _variables.ux (q) };
			for (int i{ 0 }; i < _problem.nIndices (); i++)
			{
				const std::optional<IloBoolVar>& x{ _variables.x (i, q) };
				if (x)
				{
					nX += *x;
				}
			}
			model.add (nX == 1);
			nX.end ();
		}
		for (int i{ 0 }; i < _problem.nIndices (); i++)
		{
			const std::optional<IloBoolVar>& y{ _variables.y (i) };
			if (y)
			{
				IloExpr nX{ _env };
				int nActiveX = 0;
				for (int q{ 0 }; q < _problem.nQueries (); q++)
				{
					const std::optional<IloBoolVar>& x{ _variables.x (i, q) };
					if (x)
					{
						nX += *x;
					}
				}
				model.add (nX <= nActiveX * *y);
				nX.end ();
			}
		}
		{
			IloExpr cost{ _env };
			for (int i{ 0 }; i < _problem.nIndices (); i++)
			{
				const std::optional<IloBoolVar>& y{ _variables.y (i) };
				if (y)
				{
					const Index& index{ _problem.indices[i] };
					cost += *y * index.fixedCost;
					for (int q{ 0 }; q < _problem.nQueries (); q++)
					{
						const std::optional<IloBoolVar>& x{ _variables.x (i, q) };
						if (x)
						{
							cost += *x * index.queryCosts[q];
						}
					}
				}
			}
			for (int q{ 0 }; q < _problem.nQueries (); q++)
			{
				cost += _variables.ux (q) * _problem.unindexedQueryCosts[q];
			}
			model.add (IloMinimize (_env, cost));
			cost.end ();
		}
		return model;
	}

}