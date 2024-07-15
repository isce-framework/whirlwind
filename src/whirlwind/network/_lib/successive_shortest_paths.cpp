#include <cstdint>

#include <nanobind/nanobind.h>

#include <whirlwind/common/vector.hpp>
#include <whirlwind/graph/dial.hpp>
#include <whirlwind/graph/dijkstra.hpp>
#include <whirlwind/logging/null_logger.hpp>
#include <whirlwind/network/network.hpp>
#include <whirlwind/network/residual_graph_traits.hpp>
#include <whirlwind/network/successive_shortest_paths.hpp>
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
successive_shortest_paths(nb::module_& m)
{
    using Network = Network<Graph, Cost, Flow, Container, Mixin>;

    m.def("successive_shortest_paths",
          &whirlwind::successive_shortest_paths<Dijkstra, Logger, Network>, "network"_a,
          nb::call_guard<nb::gil_scoped_release>());
}

template<class Graph,
         class Cost,
         class Dijkstra,
         class Logger,
         class Flow, // clang-format off
         template<class> class Container> // clang-format on
void
successive_shortest_paths(nb::module_& m)
{
    using Uncapacitated = UncapacitatedMixin<Graph, Flow, Container>;
    successive_shortest_paths<Graph, Cost, Dijkstra, Logger, Flow, Container,
                              Uncapacitated>(m);

    using UnitCapacity = UnitCapacityMixin<Graph, Flow, Container>;
    successive_shortest_paths<Graph, Cost, Dijkstra, Logger, Flow, Container,
                              UnitCapacity>(m);
}

template<class Graph, class Cost, class Dijkstra, class Logger, class Flow>
void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<Graph, Cost, Dijkstra, Logger, Flow, Vector>(m);
}

template<class Graph, class Cost, class Dijkstra, class Logger>
void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<Graph, Cost, Dijkstra, Logger, std::int32_t>(m);
}

template<class Graph, class Cost, class Dijkstra>
void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<Graph, Cost, Dijkstra, NullLogger>(m);
}

template<class Graph, class Cost>
void
successive_shortest_paths(nb::module_& m)
{
    using ResidualGraph = ResidualGraphTraits<Graph>::type;

    successive_shortest_paths<Graph, Cost, Dial<Cost, ResidualGraph>>(m);
    successive_shortest_paths<Graph, Cost, Dijkstra<Cost, ResidualGraph>>(m);
}

template<class Graph>
void
successive_shortest_paths(nb::module_& m)
{
    // successive_shortest_paths<Graph, float>(m);
    successive_shortest_paths<Graph, std::int32_t>(m);
}

void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<RectangularGridGraph<>>(m);
}

} // namespace whirlwind::bindings
