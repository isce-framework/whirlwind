#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <range/v3/view/span.hpp>

#include <whirlwind/common/type_traits.hpp>
#include <whirlwind/network/network.hpp>
#include <whirlwind/network/uncapacitated.hpp>
#include <whirlwind/network/unit_capacity.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class T>
using PyArray1D = nb::ndarray<T, nb::ndim<1>, nb::c_contig, nb::device::cpu>;

template<class Class, class... Extra>
void
network_attrs_and_methods(nb::class_<Class, Extra...>& cls)
{
    using Graph = typename Class::graph_type;
    using Flow = typename Class::flow_type;
    using Cost = typename Class::cost_type;

    // Constructors
    cls.def(
            "__init__",
            [](Class* self, const Graph& graph, const PyArray1D<const Flow>& surplus,
               const PyArray1D<const Cost>& cost) {
                auto surplus_span = ranges::span(surplus.data(), surplus.size());
                auto cost_span = ranges::span(cost.data(), cost.size());

                new (self) Class(graph, std::move(surplus_span), std::move(cost_span));
            },
            "graph"_a, "surplus"_a, "cost"_a, nb::call_guard<nb::gil_scoped_release>());

    // Methods.
    cls.def("node_excess", &Class::node_excess, "node"_a);
    cls.def("increase_node_excess", &Class::increase_node_excess, "node"_a, "delta"_a);
    cls.def("decrease_node_excess", &Class::decrease_node_excess, "node"_a, "delta"_a);
    cls.def("is_excess_node", &Class::is_excess_node, "node"_a);
    cls.def("is_deficit_node", &Class::is_deficit_node, "node"_a);
    cls.def("excess_nodes", &Class::excess_nodes, nb::keep_alive<0, 1>());
    cls.def("deficit_nodes", &Class::deficit_nodes, nb::keep_alive<0, 1>());
    cls.def("total_excess", &Class::total_excess,
            nb::call_guard<nb::gil_scoped_release>());
    cls.def("total_deficit", &Class::total_deficit,
            nb::call_guard<nb::gil_scoped_release>());
    cls.def("is_balanced", &Class::is_balanced,
            nb::call_guard<nb::gil_scoped_release>());
    cls.def("node_potential", &Class::node_potential, "node"_a);
    cls.def("increase_node_potential", &Class::increase_node_potential, "node"_a,
            "delta"_a);
    cls.def("decrease_node_potential", &Class::decrease_node_potential, "node"_a,
            "delta"_a);
    cls.def("arc_cost", &Class::arc_cost, "arc"_a);
    cls.def("arc_reduced_cost", &Class::arc_reduced_cost, "arc"_a, "tail"_a, "head"_a);
    cls.def("total_cost", &Class::total_cost, nb::call_guard<nb::gil_scoped_release>());

    using ExcessNodes = remove_cvref_t<decltype(std::declval<Class>().excess_nodes())>;
    using DeficitNodes =
            remove_cvref_t<decltype(std::declval<Class>().deficit_nodes())>;

    if (!nb::type<ExcessNodes>().is_valid()) {
        auto excess_nodes = nb::class_<ExcessNodes>(cls, "_ExcessNodes");
        common_iterable_attrs_and_methods(excess_nodes);
    }

    if (!nb::type<DeficitNodes>().is_valid()) {
        auto deficit_nodes = nb::class_<DeficitNodes>(cls, "_DeficitNodes");
        common_iterable_attrs_and_methods(deficit_nodes);
    }
}

template<class Graph,
         class Cost,
         class Flow,
         // clang-format off
         template<class> class Container,
         // clang-format on
         class Mixin>
void
network(nb::module_& m, const std::string& name)
{
    using Class = Network<Graph, Cost, Flow, Container, Mixin>;
    auto cls = nb::class_<Class, Mixin>(m, name.c_str());
    network_attrs_and_methods(cls);
}

template<class Graph, class Cost, class Flow, template<class> class Container>
void
network(nb::module_& m, const std::string& name)
{
    using Uncapacitated = UncapacitatedMixin<Graph, Flow, Container>;
    network<Graph, Cost, Flow, Container, Uncapacitated>(m, name + "_Uncapacitated");

    using UnitCapacity = UnitCapacityMixin<Graph, Flow, Container>;
    network<Graph, Cost, Flow, Container, UnitCapacity>(m, name + "_UnitCapacity");
}

template<class Graph, class Cost, class Flow>
void
network(nb::module_& m, const std::string& name)
{
    network<Graph, Cost, Flow, std::vector>(m, name + "_vector");
}

template<class Graph, class Cost>
void
network(nb::module_& m, const std::string& name)
{
    network<Graph, Cost, std::int32_t>(m, name + "_i32");
}

template<class Graph>
void
network(nb::module_& m, const std::string& name)
{
    network<Graph, float>(m, name + "_f32");
    network<Graph, std::int32_t>(m, name + "_i32");
}

void
network(nb::module_& m)
{
    network<RectangularGridGraph<>>(m, "Network__RectangularGridGraph");
}

} // namespace whirlwind::bindings
