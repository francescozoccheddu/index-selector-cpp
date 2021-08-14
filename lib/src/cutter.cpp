#include <index-selector-lib/cutter.hpp>
#include <chrono>
#include <stdexcept>

namespace IndexSelector
{

	Cutter::Callback::Callback (const IloEnv _env, Cutter* _cutter, bool _shared) : IloCplex::UserCutCallbackI{ _env }, m_cutter{ _cutter }, m_shared{ _shared }
	{
		if (!_cutter)
		{
			throw std::invalid_argument{ "Null cutter" };
		}
	}

	void Cutter::Callback::main ()
	{
		if (m_shared)
		{
			m_cutter->m_mutex.lock ();
		}
		using namespace std::chrono;
		steady_clock::time_point startTime = high_resolution_clock::now ();
		{
			m_cutter->cut (*this);
		}
		steady_clock::time_point endTime = high_resolution_clock::now ();
		duration<double> elapsedTime = endTime - startTime;
		m_cutter->reportElapsedTime (elapsedTime.count ());
		if (m_shared)
		{
			m_cutter->m_mutex.unlock ();
		}
	}

	IloCplex::CallbackI* Cutter::Callback::duplicateCallback () const
	{
		if (m_shared)
		{
			return new (getEnv ()) Callback{ *this };
		}
		else
		{
			return new (getEnv ()) Callback{ getEnv (), m_cutter->clone (), m_shared };
		}
	}

	bool Cutter::Callback::getValue (IloBoolVar _var)
	{
		return IloCplex::UserCutCallbackI::getValue (_var);
	}

	Cutter::Cutter (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics) : env{ _env }, variables{ _variables }, options{ _options }, statistics{ _statistics }
	{}

	void Cutter::reportElapsedTime (double _elapsedTime) const
	{}

	Cutter* Cutter::clone ()
	{
		throw std::logic_error ("Cutter must be shared since it does not implement clone()");
	}

	IloCplex::Callback Cutter::createCallback (bool _shared)
	{
		return IloCplex::Callback{ new (env) Callback {env, this, _shared} };
	}

}