#ifndef INCLUDED_INDEX_SELECTOR_LIB_VARIABLE_MATRIX
#define INCLUDED_INDEX_SELECTOR_LIB_VARIABLE_MATRIX

#include <ilcplex/ilocplex.h>
#include <optional>
#include <index-selector/immutable_array.hpp>
#include <index-selector/problem.hpp>

namespace IndexSelector
{

	class VariableMatrix final
	{

		ImmutableArray<std::optional<IloBoolVar>> m_xs{}, m_ys{};
		ImmutableArray<IloBoolVar> m_uxs{};
		size_t m_nActiveXs{}, m_nActiveYs{};
		const Problem* m_pProblem{};

	public:

		VariableMatrix (const IloEnv _env, const Problem& _problem, bool _reduceVariables = true);

		const std::optional<IloBoolVar>& x (size_t _i, size_t _q) const;
		const std::optional<IloBoolVar>& y (size_t _i) const;
		const IloBoolVar& ux (size_t _i) const;

		const Problem& problem () const;
		size_t nActiveXs () const;
		size_t nActiveYs () const;

	};

}

#endif