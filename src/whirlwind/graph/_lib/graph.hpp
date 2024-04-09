#pragma once

#include <nanobind/nanobind.h>

#include <whirlwind/graph/graph_traits.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Graph>
void
common_graph_attrs_and_methods(nb::class_<Graph>& cls)
{
    // Attributes.
    cls.def_prop_ro("num_vertices", &Graph::num_vertices);
    cls.def_prop_ro("num_edges", &Graph::num_edges);

    // Methods.
    cls.def("get_vertex_id", &Graph::get_vertex_id, "vertex"_a);
    cls.def("get_edge_id", &Graph::get_edge_id, "edge"_a);
    cls.def("vertices", &Graph::vertices, nb::keep_alive<0, 1>());
    cls.def("edges", &Graph::edges, nb::keep_alive<0, 1>());
    cls.def("contains_vertex", &Graph::contains_vertex, "vertex"_a);
    cls.def("contains_edge", &Graph::contains_edge, "edge"_a);
    cls.def("outdegree", &Graph::outdegree, "vertex"_a);
    cls.def("outgoing_edges", &Graph::outgoing_edges, "vertex"_a,
            nb::keep_alive<0, 1>());
}

template<class Graph>
auto
graph_member_types(nb::class_<Graph>& cls)
{
    using Traits = GraphTraits<Graph>;
    using Vertices = Traits::vertices_type;
    using Edges = Traits::edges_type;
    using OutgoingEdges = Traits::outgoing_edges_type;

    if (!nb::type<Vertices>().is_valid()) {
        auto vertices = nb::class_<Vertices>(cls, "_Vertices");
        common_iterable_attrs_and_methods(vertices);
    }

    if (!nb::type<Edges>().is_valid()) {
        auto edges = nb::class_<Edges>(cls, "_Edges");
        common_iterable_attrs_and_methods(edges);
    }

    if (!nb::type<OutgoingEdges>().is_valid()) {
        auto outgoing_edges = nb::class_<OutgoingEdges>(cls, "_OutgoingEdges");
        common_iterable_attrs_and_methods(outgoing_edges);
    }
}

} // namespace whirlwind::bindings
