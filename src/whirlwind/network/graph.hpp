#pragma once

#include <tuple>
#include <utility>

#include <nanobind/nanobind.h>

#include <whirlwind/graph/graph_traits.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

inline constexpr auto graph_get_vertex_id_doc = R"(
    Get the unique array index of a vertex.

    Given a vertex in the graph, get the associated vertex index in the range [0, V),
    where V is the total number of vertices.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid vertex in the graph.

    Returns
    -------
    int
        The vertex index.
)";

inline constexpr auto graph_get_edge_id_doc = R"(
    Get the unique array index of an edge.

    Given an edge in the graph, get the associated edge index in the range [0, E), where
    E is the total number of edges.

    Parameters
    ----------
    edge : Edge
        The input edge. Must be a valid edge in the graph.

    Returns
    -------
    int
        The edge index.
)";

inline constexpr auto graph_vertices_doc = R"(
    Iterate over vertices in the graph.

    Returns a view of all vertices in the graph in order from smallest index to
    largest.
)";

inline constexpr auto graph_edges_doc = R"(
    Iterate over edges in the graph.

    Returns a view of all edges in the graph in order from smallest index to largest.
)";

inline constexpr auto graph_outdegree_doc = R"(
    Get the number of outgoing edges of a vertex.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid vertex in the graph.

    Returns
    -------
    int
        The outdegree of the vertex.
)";

inline constexpr auto graph_outgoing_edges_doc = R"(
    Iterate over outgoing edges (and corresponding head vertices) of a vertex.

    Returns a view of ordered (edge,head) pairs over all edges emanating from the
    specified vertex in the graph.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid vertex in the graph.

    Returns
    -------
    iterable of (Edge, Vertex)
        A view of the vertex's outgoing incident edges and successor vertices.
)";

template<class Graph>
void
common_graph_attrs_and_methods(nb::class_<Graph>& cls)
{
    // Attributes.
    cls.def_prop_ro("num_vertices", &Graph::num_vertices,
                    "The total number of vertices in the graph.");
    cls.def_prop_ro("num_edges", &Graph::num_edges,
                    "The total number of edges in the graph.");

    // Methods.
    cls.def("get_vertex_id", &Graph::get_vertex_id, "vertex"_a,
            graph_get_vertex_id_doc);
    cls.def("get_edge_id", &Graph::get_edge_id, "edge"_a, graph_get_edge_id_doc);
    cls.def("vertices", &Graph::vertices, nb::keep_alive<0, 1>(), graph_vertices_doc);
    cls.def("edges", &Graph::edges, nb::keep_alive<0, 1>(), graph_edges_doc);
    cls.def("contains_vertex", &Graph::contains_vertex, "vertex"_a,
            "Check whether the graph contains the specified vertex.");
    cls.def("contains_edge", &Graph::contains_edge, "edge"_a,
            "Check whether the graph contains the specified edge.");
    cls.def("outdegree", &Graph::outdegree, "vertex"_a, graph_outdegree_doc);
    cls.def("outgoing_edges", &Graph::outgoing_edges, nb::keep_alive<0, 1>(),
            "vertex"_a, graph_outgoing_edges_doc);
}

template<class Graph>
auto
graph_member_types(nb::class_<Graph>& cls)
{
    using Traits = GraphTraits<Graph>;
    auto vertices = nb::class_<typename Traits::vertices_type>(cls, "_Vertices");
    auto edges = nb::class_<typename Traits::edges_type>(cls, "_Edges");
    auto outgoing_edges =
            nb::class_<typename Traits::outgoing_edges_type>(cls, "_OutgoingEdges");

    return std::tuple(std::move(vertices), std::move(edges), std::move(outgoing_edges));
}

} // namespace whirlwind::bindings
