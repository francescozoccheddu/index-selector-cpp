#ifndef INCLUDED_INDEX_SELECTOR_LIB_OPTIMAL_SIZE_CUTTER
#define INCLUDED_INDEX_SELECTOR_LIB_OPTIMAL_SIZE_CUTTER

#include <index-selector-lib/cutter.hpp>

namespace IndexSelector
{

	class OptimalSizeCutter final : public SimpleCutter<OptimalSizeCutter>
	{

		IloNumVarArray m_zs;
		IloNumArray m_vys;
		IloCplex m_cplex;

	protected:

		void cut (Callback& _callback) override;
		bool shouldShare () const override;

	public:

		OptimalSizeCutter (Cutter::Manager& _manager);

	};

}

#endif