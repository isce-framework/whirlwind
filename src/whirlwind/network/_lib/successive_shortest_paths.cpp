#include <cstdint>
#include <vector>

#include <nanobind/nanobind.h>

#include <whirlwind/graph/dijkstra.hpp>
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
         class Flow,
         // clang-format off
         template<class> class Container,
         // clang-format on
         class Mixin>
void
successive_shortest_paths(nb::module_& m)
{
    using ResidualGraph = ResidualGraphTraits<Graph>::type;
    using Dijkstra = Dijkstra<Cost, ResidualGraph>;
    using Network = Network<Graph, Cost, Flow, Container, Mixin>;

    m.def("dijkstra_ssp", &dijkstra_ssp<Dijkstra, Network>, "dijkstra"_a, "network"_a,
          "source"_a, nb::call_guard<nb::gil_scoped_release>());
    m.def("augment_flow_ssp", &augment_flow_ssp<Network, Dijkstra>, "network"_a,
          "dijkstra"_a, "sink"_a, nb::call_guard<nb::gil_scoped_release>());
    m.def("update_potential_ssp", &update_potential_ssp<Network, Dijkstra>, "network"_a,
          "dijkstra"_a, "sink"_a, nb::call_guard<nb::gil_scoped_release>());
    m.def("successive_shortest_paths",
          &whirlwind::successive_shortest_paths<Dijkstra, Network>, "network"_a,
          nb::call_guard<nb::gil_scoped_release>());
}

template<class Graph, class Cost, class Flow, template<class> class Container>
void
successive_shortest_paths(nb::module_& m)
{
    using Uncapacitated = UncapacitatedMixin<Graph, Flow, Container>;
    successive_shortest_paths<Graph, Cost, Flow, Container, Uncapacitated>(m);

    using UnitCapacity = UnitCapacityMixin<Graph, Flow, Container>;
    successive_shortest_paths<Graph, Cost, Flow, Container, UnitCapacity>(m);
}

template<class Graph, class Cost, class Flow>
void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<Graph, Cost, Flow, std::vector>(m);
}

template<class Graph, class Cost>
void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<Graph, Cost, std::int32_t>(m);
}

template<class Graph>
void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<Graph, float>(m);
    successive_shortest_paths<Graph, std::int32_t>(m);
}

void
successive_shortest_paths(nb::module_& m)
{
    successive_shortest_paths<RectangularGridGraph<>>(m);
}

} // namespace whirlwind::bindings
