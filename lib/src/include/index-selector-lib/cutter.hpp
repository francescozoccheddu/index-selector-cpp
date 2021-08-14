#ifndef INCLUDED_INDEX_SELECTOR_LIB_CUTTER
#define INCLUDED_INDEX_SELECTOR_LIB_CUTTER

#include <ilcplex/ilocplex.h>
#include <ilconcert/ilothread.h>
#include <index-selector-lib/variable_matrix.hpp>
#include <index-selector/solution.hpp>
#include <index-selector/options.hpp>

namespace IndexSelector
{

	class Cutter
	{

		IloFastMutex m_mutex{};

	protected:

		class Callback : private IloCplex::UserCutCallbackI
		{

			friend class Cutter;

			Callback (const IloEnv _env, Cutter* _cutter, bool _shared);

			std::shared_ptr<Cutter> m_cutter;
			bool m_shared;

		protected:

			void main () override;
			IloCplex::CallbackI* duplicateCallback () const override;

		public:

			Real getValue (IloBoolVar _var);
			using IloCplex::UserCutCallbackI::add;
			using IloCplex::UserCutCallbackI::addLocal;

		};

		Cutter (const Cutter&);

		virtual void reportElapsedTime (double _elapsedTime) const;
		virtual void cut (Callback& _callback) = 0;
		virtual Cutter* clone () const;

	public:

		Cutter (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics);

		const IloEnv env;
		const VariableMatrix& variables;
		const Options& options;
		Solution::Statistics& statistics;

		IloCplex::Callback createCallback ();

	};

}

#endif