#include <string>
#include <utility>

#include <nanobind/nanobind.h>

#include <whirlwind/common/type_traits.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>
#include <whirlwind/network/residual_graph.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class>
void
basic_residual_graph_mixin_attrs_and_methods(nb::class_<Class>& cls)
{
    // Attributes.
    cls.def_prop_ro("residual_graph", &Class::residual_graph,
                    nb::rv_policy::reference_internal);
    cls.def_prop_ro("num_nodes", &Class::num_nodes);
    cls.def_prop_ro("num_arcs", &Class::num_arcs);

    // Methods.
    cls.def("contains_node", &Class::contains_node, "node"_a);
    cls.def("contains_arc", &Class::contains_arc, "arc"_a);
    cls.def("get_node_id", &Class::get_node_id, "node"_a);
    cls.def("get_arc_id", &Class::get_arc_id, "arc"_a);
    cls.def("nodes", &Class::nodes, nb::keep_alive<0, 1>());
    cls.def("arcs", &Class::arcs, nb::keep_alive<0, 1>());
    cls.def("outgoing_arcs", &Class::outgoing_arcs, "node"_a, nb::keep_alive<0, 1>());

    using Node = typename Class::node_type;
    using Nodes = remove_cvref_t<decltype(std::declval<Class>().nodes())>;
    using Arcs = remove_cvref_t<decltype(std::declval<Class>().arcs())>;
    using OutgoingArcs = remove_cvref_t<decltype(std::declval<Class>().outgoing_arcs(
            std::declval<Node>()))>;

    if (!nb::type<Nodes>().is_valid()) {
        auto nodes = nb::class_<Nodes>(cls, "_Nodes");
        common_iterable_attrs_and_methods(nodes);
    }

    if (!nb::type<Arcs>().is_valid()) {
        auto arcs = nb::class_<Arcs>(cls, "_Arcs");
        common_iterable_attrs_and_methods(arcs);
    }

    if (!nb::type<OutgoingArcs>().is_valid()) {
        auto outgoing_arcs = nb::class_<OutgoingArcs>(cls, "_OutgoingArcs");
        common_iterable_attrs_and_methods(outgoing_arcs);
    }
}

template<class Class, class... Extra>
void
residual_graph_mixin_attrs_and_methods(nb::class_<Class, Extra...>& cls)
{
    // Methods.
    cls.def("is_forward_arc", &Class::is_forward_arc, "arc"_a);
    cls.def("get_residual_graph_arc_id", &Class::get_residual_graph_arc_id,
            "edge_id"_a);
    cls.def("get_transpose_arc_id", &Class::get_transpose_arc_id, "arc"_a);
}

template<class Graph>
void
residual_graph(nb::module_& m, const std::string& name)
{
    using Class = ResidualGraphMixin<Graph>;
    using Parent = detail::BasicResidualGraphMixin<Graph>;

    if (!nb::type<Parent>().is_valid()) {
        auto parent = nb::class_<Parent>(m, ("Basic" + name).c_str());
        basic_residual_graph_mixin_attrs_and_methods(parent);
    }

    auto cls = nb::class_<Class, Parent>(m, name.c_str());
    residual_graph_mixin_attrs_and_methods(cls);
}

void
residual_graph(nb::module_& m)
{
    residual_graph<RectangularGridGraph<>>(m,
                                           "ResidualGraphMixin__RectangularGridGraph");
}

} // namespace whirlwind::bindings
