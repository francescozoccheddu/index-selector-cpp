#include <index-selector-lib/heuristic_size_cutter.hpp>
#include <limits>
#include <cmath>
#include <algorithm>
#include <chrono>


namespace IndexSelector
{

	HeuristicSizeCutter::HeuristicSizeCutter (Cutter::Manager& _manager) : SimpleCutter{ _manager }
	{
		if (manager.options.sizeCutMode != Options::ESizeCutMode::Heuristic)
		{
			throw std::logic_error{ "heuristic size cuts are disabled in options" };
		}
		const size_t nAy{ _manager.variables.nActiveYs () };
		m_candidates = std::vector<Candidate>{ nAy };
		{
			size_t ai{ 0 }, i{ 0 };
			while (ai < nAy)
			{
				const std::optional<IloBoolVar>& y{ manager.variables.y (i) };
				if (y)
				{
					m_candidates[ai++] = Candidate{ .size {_manager.variables.problem ().indices[i].size }, .y{ *y } };
				}
				i++;
			}
		}
		m_maxK = std::min<size_t> (_manager.options.nMaxHeuristicSizeCutVars, nAy);
		if (_manager.options.enableHeuristicSizeCutHeuristics)
		{
			std::sort (m_candidates.begin (), m_candidates.end (), [] (const Candidate& _a, const Candidate& _b)
			{
				return _a.size > _b.size;
			});
			Real topSum{}, kSum{};
			size_t ai{}, sai{};
			const Real maxSize{ _manager.variables.problem ().maxSize };
			for (size_t k{ 2 }; k <= m_maxK; k++)
			{
				topSum += m_candidates[k - 2].size;
				Real minSize{ maxSize - topSum };
				while (sai < k)
				{
					kSum += m_candidates[sai].size;
					sai++;
				}
				ai = std::max<size_t> (ai, sai);
				while (ai < nAy and m_candidates[ai].size >= minSize)
				{
					ai++;
				}
				if (kSum >= maxSize)
				{
					if (!m_minK)
					{
						m_minK = k;
					}
					size_t mm{ ai };
					while (mm > 0 and m_candidates[--mm].minK == 0)
					{
						m_candidates[mm].minK = k;
					}
				}
			}
		}
		else
		{
			m_minK = 2;
		}
		if (m_minK)
		{
			m_valueSortedCandidates = std::vector<Candidate*> (m_candidates.size ());
			m_kCandidates = std::vector<Candidate*> (m_candidates.size ());
			m_chosen = std::vector<size_t> (m_maxK);
		}
		else
		{
			m_maxK = 0;
			m_candidates.resize (0);
		}
	}

	void HeuristicSizeCutter::choose (Callback& _callback, size_t _k)
	{
		const size_t nc{ m_kCandidates.size () };
		m_chosen.clear ();
		for (size_t i{ _k }; i > 0; i--)
		{
			m_chosen.push_back (i - 1);
		}
		while (true)
		{
			if (!pushChosen (_callback))
			{
				if (manager.options.enableHeuristicSizeCutHeuristics)
				{
					for (size_t i{ 0 }; i < _k; i++)
					{
						const size_t mi = (i == _k - 1) ? 0 : (m_chosen[i + 1] + 1);
						bool stop{ m_chosen[i] <= mi + 1 };
						m_chosen[i] = nc;
						if (stop)
						{
							break;
						}
					}
				}
			}
			else if (m_remainingCuts == 0)
			{
				return;
			}
			size_t i;
			for (i = 0; i < _k; i++)
			{
				if (m_chosen[i] < nc - i - 1)
				{
					m_chosen[i] ++;
					while (i-- > 0)
					{
						m_chosen[i] = m_chosen[i + 1] + 1;
					}
					break;
				}
			}
			if (i == _k)
			{
				break;
			}
		}
	}

	bool HeuristicSizeCutter::pushChosen (Callback& _callback)
	{
		Real size{}, value{};
		for (size_t c{ 0 }; c < m_chosen.size (); c++)
		{
			value += m_kCandidates[m_chosen[c]]->value;
			size += m_kCandidates[m_chosen[c]]->size;
		}
		if (value <= m_chosen.size () - 1)
		{
			return false;
		}
		if (size > manager.variables.problem ().maxSize)
		{
			IloExpr sum{ manager.env };
			for (size_t c{ 0 }; c < m_chosen.size (); c++)
			{
				sum += m_kCandidates[m_chosen[c]]->y;
			}
			_callback.add (sum <= static_cast<int>(m_chosen.size () - 1), manager.options.sizeCutManagement);
			sum.end ();
			m_remainingCuts--;
		}
		return true;
	}

	void HeuristicSizeCutter::cut (Callback& _callback)
	{
		if (!m_maxK)
		{
			return;
		}
		_callback.lockIfShared ();
		std::chrono::steady_clock::time_point startTime{ std::chrono::high_resolution_clock::now () };
		m_remainingCuts = manager.options.nMaxSizeCuts;
		m_valueSortedCandidates.clear ();
		for (Candidate& c : m_candidates)
		{
			c.value = _callback.getValue (c.y);
			if (c.value > 0)
			{
				m_valueSortedCandidates.push_back (&c);
			}
		}
		if (!m_valueSortedCandidates.empty ())
		{
			if (manager.options.enableHeuristicSizeCutHeuristics)
			{
				std::sort (m_valueSortedCandidates.begin (), m_valueSortedCandidates.end (), [] (const Candidate* _a, const Candidate* _b)
				{
					return _a->value > _b->value;
				});
			}
			for (size_t k{ m_minK }; k <= m_maxK; k++)
			{
				m_kCandidates.clear ();
				Real topSum{}, value{}, size{};
				for (Candidate* c : m_valueSortedCandidates)
				{
					if (manager.options.enableHeuristicSizeCutHeuristics)
					{
						if (m_kCandidates.size () >= k - 1 and c->value < (topSum - k + 1))
						{
							break;
						}
						if (k >= c->minK)
						{
							if (m_kCandidates.size () < k - 1)
							{
								topSum += c->value;
							}
							if (m_kCandidates.size () < k)
							{
								value += c->value;
								if (m_candidates.size () == k - 1 and value <= k - 1)
								{
									break;
								}
							}
							size += c->size;
						}
					}
					m_kCandidates.push_back (c);
				}
				if (m_kCandidates.size () >= k and (!manager.options.enableHeuristicSizeCutHeuristics or (value > k - 1 and size > manager.variables.problem ().maxSize)))
				{
					choose (_callback, k);
					if (m_remainingCuts == 0 or std::chrono::duration<double>{std::chrono::high_resolution_clock::now () - startTime}.count () > manager.options.sizeCutTimeLimit)
					{
						break;
					}
				}
			}
		}
		_callback.unlockIfShared ();
	}

	bool HeuristicSizeCutter::shouldShare () const
	{
		return false;
	}

}