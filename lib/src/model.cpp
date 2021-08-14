#include <index-selector-lib/model.hpp>
#include <format>

namespace IndexSelector
{

	IloModel create_model (const IloEnv _env, const VariableMatrix& _variables)
	{
		const Problem problem{ _variables.problem () };
		const size_t ni{ problem.nIndices () }, nq{ problem.nQueries () };
		IloModel model{ _env , "Index selection" };
		std::string name{ 9 };
		if (ni > 0)
		{
			IloExpr actualSize{ _env };
			for (size_t i{ 0 }; i < ni; i++)
			{
				const std::optional<IloBoolVar>& y{ _variables.y (i) };
				if (y)
				{
					actualSize += *y * problem.indices[i].size;
				}
			}
			model.add (actualSize <= problem.maxSize).setName ("ms");
			actualSize.end ();
		}
		for (size_t q{ 0 }; q < nq; q++)
		{
			IloExpr nX{ _variables.ux (q) };
			for (size_t i{ 0 }; i < ni; i++)
			{
				const std::optional<IloBoolVar>& x{ _variables.x (i, q) };
				if (x)
				{
					nX += *x;
				}
			}
			name.clear ();
			std::format_to (std::back_inserter (name), "squ_q{}", q);
			model.add (nX == 1).setName (name.c_str ());
			nX.end ();
		}
		for (size_t i{ 0 }; i < ni; i++)
		{
			const std::optional<IloBoolVar>& y{ _variables.y (i) };
			if (y)
			{
				IloExpr nX{ _env };
				int nActiveX = 0;
				for (size_t q{ 0 }; q < nq; q++)
				{
					const std::optional<IloBoolVar>& x{ _variables.x (i, q) };
					if (x)
					{
						nActiveX++;
						nX += *x;
					}
				}
				name.clear ();
				std::format_to (std::back_inserter (name), "miu_i{}", i);
				model.add (nX <= *y * nActiveX).setName (name.c_str ());
				nX.end ();
			}
		}
		{
			IloExpr cost{ _env };
			for (size_t i{ 0 }; i < ni; i++)
			{
				const std::optional<IloBoolVar>& y{ _variables.y (i) };
				if (y)
				{
					const Index& index{ problem.indices[i] };
					cost += *y * index.fixedCost;
					for (size_t q{ 0 }; q < nq; q++)
					{
						const std::optional<IloBoolVar>& x{ _variables.x (i, q) };
						if (x)
						{
							cost += *x * index.queryCosts[q];
						}
					}
				}
			}
			for (size_t q{ 0 }; q < nq; q++)
			{
				cost += _variables.ux (q) * problem.unindexedQueryCosts[q];
			}
			model.add (IloMinimize (_env, cost)).setName ("c");
			cost.end ();
		}
		return model;
	}

}