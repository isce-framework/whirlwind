#include <nanobind/nanobind.h>

#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/edge_list.hpp>
#include <whirlwind/graph/graph_traits.hpp>

#include "graph.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<template<class> class Container>
void
csr_graph_attrs_and_methods(nb::class_<CSRGraph<Container>>& cls)
{
    using Vertex = typename GraphTraits<CSRGraph<Container>>::vertex_type;

    // Constructors.
    cls.def(nb::init<>(), "Default constructor. Creates an empty `CSRGraph` with no"
                          " vertices or edges.");
    cls.def(nb::init<EdgeList<Vertex>>(), nb::call_guard<nb::gil_scoped_release>(),
            "edge_list"_a,
            "Create a new `CSRGraph` from a sequence of (tail,head) pairs.");

    common_graph_attrs_and_methods(cls);
}

void
csr_graph(nb::module_& m)
{
    auto graph = nb::class_<CSRGraph<>>(m, "CSRGraph");
    csr_graph_attrs_and_methods(graph);
    graph_member_types(graph);
}

} // namespace whirlwind::bindings
