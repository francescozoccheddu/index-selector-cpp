#include <index-selector-lib/optimal_size_cutter.hpp>
#include <limits>
#include <cmath>
#include <stdexcept>

namespace IndexSelector
{

	OptimalSizeCutter::OptimalSizeCutter (Cutter::Manager& _manager) : SimpleCutter{ _manager }
	{
		if (manager.options.sizeCutMode != Options::ESizeCutMode::Optimal)
		{
			throw std::logic_error{ "optimal size cuts are disabled in options" };
		}
		IloModel m{ _manager.env, "Optimal size cut submodel" };
		m_cplex = IloCplex{ _manager.env };
		m_cplex.setOut (_manager.env.getNullStream ());
		m_cplex.setError (_manager.env.getNullStream ());
		const int nZs{ static_cast<int>(_manager.variables.nActiveYs ()) };
		IloBoolVarArray zs{ _manager.env, nZs };
		m_vys = IloNumArray{ _manager.env, nZs };
		size_t ai{ 0 }, i{ 0 };
		while (ai < manager.variables.nActiveYs ())
		{
			const std::optional<IloBoolVar>& y{ _manager.variables.y (i) };
			if (y)
			{
				m_vys[ai++] = _manager.variables.problem ().indices[i].size;
			}
			i++;
		}
		m.add (IloScalProd (zs, m_vys) >= std::nextafter (_manager.variables.problem ().maxSize, _manager.variables.problem ().maxSize + 1));
		m.add (IloMinimize (manager.env, IloScalProd (zs, m_vys)));
		m_cplex.extract (m);
		m_cplex.setParam (IloCplex::Param::TimeLimit, _manager.options.sizeCutTimeLimit);
		m_zs = zs.toNumVarArray ();
		zs.end ();
	}

	void OptimalSizeCutter::cut (Callback& _callback)
	{
		_callback.lockIfShared ();
		{
			size_t ai{ 0 }, i{ 0 };
			while (ai < manager.variables.nActiveYs ())
			{
				const std::optional<IloBoolVar>& y{ manager.variables.y (i++) };
				if (y)
				{
					m_vys[ai++] = 1.0 - _callback.getValue (*y);
				}
			}
		}
		m_cplex.getObjective ().setLinearCoefs (m_zs, m_vys);
		if (m_cplex.solve () && m_cplex.getObjValue () < 1)
		{
			IloExpr sum{ manager.env };
			int count{};
			size_t ai{ 0 }, i{ 0 };
			while (ai < manager.variables.nActiveYs ())
			{
				const std::optional<IloBoolVar>& y{ manager.variables.y (i++) };
				if (y)
				{
					if (m_cplex.getValue (m_zs[ai]))
					{
						sum += *y;
						count++;
					}
					ai++;
				}
			}
			_callback.add (sum <= count - 1, manager.options.sizeCutManagement);
			sum.end ();
		}
		_callback.unlockIfShared ();
	}

	bool OptimalSizeCutter::shouldShare () const
	{
		return manager.options.shareSizeCutter;
	}

}