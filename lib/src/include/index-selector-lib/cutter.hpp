#ifndef INCLUDED_INDEX_SELECTOR_LIB_CUTTER
#define INCLUDED_INDEX_SELECTOR_LIB_CUTTER

#include <ilcplex/ilocplex.h>
#include <ilconcert/ilothread.h>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector/solution.hpp>
#include <index-selector/options.hpp>
#include <atomic>
#include <chrono>
#include <limits>


namespace IndexSelector
{

	class Cutter
	{

	protected:

		class Callback : private IloCplex::UserCutCallbackI
		{

			friend class Cutter;

			Callback (Cutter& _cutter, bool _owner);

			Cutter& m_cutter;
			const bool m_owner;

		protected:

			void main () override;
			IloCplex::CallbackI* duplicateCallback () const override;

		public:

			Real getValue (IloBoolVar _var);
			IloConstraint add (IloConstraint _constraint, IloCplex::CutManagement _management = IloCplex::CutManagement::UseCutForce);
			IloConstraint addLocal (IloConstraint _constraint);
			void lock ();
			void lockIfShared ();
			void unlock ();
			void unlockIfShared ();

			~Callback ();

		};

		virtual void cut (Callback& _callback) = 0;
		virtual Cutter* clone () const;

	public:

		class Manager final
		{

#ifdef INDEX_SELECTOR_MEASURE_TIME
			IloFastMutex m_timeMutex{};
			std::chrono::steady_clock::time_point m_startTime;
			size_t m_nRunningCutters{};
			std::chrono::nanoseconds m_elapsedTime{};
#endif
			std::atomic_size_t m_nCuts{};
			IloFastMutex m_dataMutex{};

			void startCutter ();
			void endCutter ();
			void addCut ();
			void lock ();
			void unlock ();

			friend class Cutter;

		public:

			Manager (const Manager&) = delete;
			Manager (Manager&&) = delete;

			Manager (IloEnv _env, const VariableMatrix& _variables, const Options& _options);

			const IloEnv env;
			const VariableMatrix& variables;
			const Options& options;

			size_t nCuts () const;
			double elapsedTime () const;

		};

		Manager& manager;

		Cutter (Manager& _manager);
		virtual ~Cutter () = default;

		IloCplex::Callback createCallback (bool _own = false);

	};

}

#endif