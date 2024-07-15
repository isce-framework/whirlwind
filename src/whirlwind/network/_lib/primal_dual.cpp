#include <cstdint>

#include <nanobind/nanobind.h>

#include <whirlwind/common/vector.hpp>
#include <whirlwind/graph/dial.hpp>
#include <whirlwind/graph/dijkstra.hpp>
#include <whirlwind/logging/null_logger.hpp>
#include <whirlwind/network/network.hpp>
#include <whirlwind/network/primal_dual.hpp>
#include <whirlwind/network/residual_graph_traits.hpp>
#include <whirlwind/network/uncapacitated.hpp>
#include <whirlwind/network/unit_capacity.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Graph,
         class Cost,
         class Dijkstra,
         class Logger,
         class Flow, // clang-format off
         template<class> class Container, // clang-format on
         class Mixin>
void
primal_dual(nb::module_& m)
{
    using ResidualGraph = ResidualGraphTraits<Graph>::type;
    using Network = Network<Graph, Cost, Flow, Container, Mixin>;

    m.def("primal_dual", &whirlwind::primal_dual<Dijkstra, Logger, Network>,
          "network"_a, "maxiter"_a = 0, nb::call_guard<nb::gil_scoped_release>());
}

template<class Graph,
         class Cost,
         class Dijkstra,
         class Logger,
         class Flow, // clang-format off
         template<class> class Container> // clang-format on
void
primal_dual(nb::module_& m)
{
    using Uncapacitated = UncapacitatedMixin<Graph, Flow, Container>;
    primal_dual<Graph, Cost, Dijkstra, Logger, Flow, Container, Uncapacitated>(m);

    using UnitCapacity = UnitCapacityMixin<Graph, Flow, Container>;
    primal_dual<Graph, Cost, Dijkstra, Logger, Flow, Container, UnitCapacity>(m);
}

template<class Graph, class Cost, class Dijkstra, class Logger, class Flow>
void
primal_dual(nb::module_& m)
{
    primal_dual<Graph, Cost, Dijkstra, Logger, Flow, Vector>(m);
}

template<class Graph, class Cost, class Dijkstra, class Logger>
void
primal_dual(nb::module_& m)
{
    primal_dual<Graph, Cost, Dijkstra, Logger, std::int32_t>(m);
}

template<class Graph, class Cost, class Dijkstra>
void
primal_dual(nb::module_& m)
{
    primal_dual<Graph, Cost, Dijkstra, NullLogger>(m);
}

template<class Graph, class Cost>
void
primal_dual(nb::module_& m)
{
    using ResidualGraph = ResidualGraphTraits<Graph>::type;

    primal_dual<Graph, Cost, Dial<Cost, ResidualGraph>>(m);
    primal_dual<Graph, Cost, Dijkstra<Cost, ResidualGraph>>(m);
}

template<class Graph>
void
primal_dual(nb::module_& m)
{
    // primal_dual<Graph, float>(m);
    primal_dual<Graph, std::int32_t>(m);
}

void
primal_dual(nb::module_& m)
{
    primal_dual<RectangularGridGraph<>>(m);
}

} // namespace whirlwind::bindings
