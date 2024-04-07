#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/type_traits.hpp>
#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/forest.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

inline constexpr auto forest_ctor_doc = R"(
    Create a new `Forest`.

    The forest is initialized such that each vertex in the graph is the root of its own
    singleton tree (by setting its predecessor vertex to itself).

    Parameters
    ----------
    graph : Graph
        The forest's underlying graph.
    edge_fill_value : Edge
        The default predecessor edge value for vertices that have no predecessor.
)";

inline constexpr auto forest_predecessor_vertex_doc = R"(
    Get the predecessor (parent) vertex of a vertex.

    If `vertex` is a root vertex, its predecessor is itself.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid vertex in the graph.

    Returns
    -------
    Vertex
        The vertex's predecessor vertex.
)";

inline constexpr auto forest_predecessor_edge_doc = R"(
    Get the predecessor edge of a vertex.

    The predecessor edge is only valid if `vertex` is not a root vertex of a tree in the
    forest.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid, non-root vertex in the graph.

    Returns
    -------
    Edge
        The vertex's predecessor edge.
)";

inline constexpr auto forest_predecessor_doc = R"(
    Get the predecessor vertex and edge of a vertex.

    The predecessor edge is only valid if `vertex` is not a root vertex of a tree in the
    forest.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid, non-root vertex in the graph.

    Returns
    -------
    pred_vertex : Vertex
        The vertex's predecessor vertex.
    pred_edge : Edge
        The vertex's predecessor edge.
)";

inline constexpr auto forest_predecessors_doc = R"(
    Iterate over a vertex's predecessor vertices and edges.

    Returns a view of the input vertex's predecessors. Iterating over the result
    traverses a path in the graph from `vertex` to the root of the tree on which it
    resides, while yielding (tail,edge) pairs of the predecessors of each visited vertex
    (excluding the root vertex).

    The view is empty if `vertex` is a root vertex.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid vertex in the graph.

    Returns
    -------
    iterable of (Vertex, Edge)
        A view of the vertex's predecessors.
)";

inline constexpr auto forest_set_predecessor_doc = R"(
    Assign a vertex's predecessor.

    Parameters
    ----------
    vertex : Vertex
        The successor (child) vertex. Must be a valid vertex in the graph.
    pred_vertex : Vertex
        The predecessor (parent) vertex. Must be a valid vertex in the graph.
    pred_edge : Edge
        The predecessor edge of `vertex`. If `vertex` is not a root vertex, it must be a
        valid edge in the graph whose head is `vertex` and whose tail is `pred_vertex`.
)";

inline constexpr auto forest_make_root_vertex_doc = R"(
    Make the specified vertex into a root vertex.

    Make the specified vertex the root of a new tree by setting its predecessor vertex
    to itself and setting its predecessor edge to `edge_fill_value`.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid vertex in the graph.
)";

inline constexpr auto forest_is_root_vertex_doc = R"(
    Check if a vertex is the root of a tree in the forest.

    Parameters
    ----------
    vertex : Vertex
        The input vertex. Must be a valid vertex in the graph.

    Returns
    -------
    bool
        True if the vertex is a root vertex; otherwise false.
)";

inline constexpr auto forest_reset_doc = R"(
    Reset the forest to its initial state.

    Re-initializes the forest such that each vertex in the graph is the root of its own
    singleton tree (by setting its predecessor vertex to itself). Each predecessor edge
    is set to the value of `edge_fill_value`.
)";

template<class Graph, template<class> class Container>
void
forest_attrs_and_methods(nb::class_<Forest<Graph, Container>>& cls)
{
    using Class = Forest<Graph, Container>;

    using Vertex = Class::vertex_type;
    using Edge = Class::edge_type;
    using Pred = Class::pred_type;

    // Constructors.
    cls.def(nb::init<const Graph&, Edge>(), "graph"_a, "edge_fill_value"_a = Edge{},
            forest_ctor_doc);

    // Attributes.
    cls.def_prop_ro("graph", &Class::graph, "The underlying graph.");
    cls.def_prop_ro("edge_fill_value", &Class::edge_fill_value,
                    "The default predecessor edge value for vertices that have no"
                    " predecessor.");

    // Methods.
    cls.def("predecessor_vertex", &Class::predecessor_vertex, "vertex"_a,
            forest_predecessor_vertex_doc);
    cls.def("predecessor_edge", &Class::predecessor_edge, "vertex"_a,
            forest_predecessor_edge_doc);
    cls.def("predecessor", &Class::predecessor, "vertex"_a, forest_predecessor_doc);
    cls.def("predecessors", &Class::predecessors, "vertex"_a, forest_predecessors_doc);
    cls.def("set_predecessor",
            nb::overload_cast<const Vertex&, Vertex, Edge>(&Class::set_predecessor),
            "vertex"_a, "pred_vertex"_a, "pred_edge"_a, forest_set_predecessor_doc);
    cls.def("make_root_vertex", &Class::make_root_vertex, "vertex"_a,
            forest_make_root_vertex_doc);
    cls.def("is_root_vertex", &Class::is_root_vertex, "vertex"_a,
            forest_is_root_vertex_doc);
    cls.def("reset", &Class::reset, forest_reset_doc);
}

template<class Graph>
void
forest_inst(nb::module_& m, const char* name)
{
    using Class = Forest<Graph>;
    using Vertex = Class::vertex_type;
    using Predecessors = remove_cvref_t<decltype(std::declval<Class>().predecessors(
            std::declval<Vertex>()))>;

    auto forest = nb::class_<Class>(m, name);
    auto predecessors = nb::class_<Predecessors>(forest, "_Predecessors");

    forest_attrs_and_methods(forest);
    common_iterable_attrs_and_methods(predecessors);
}

void
forest(nb::module_& m)
{
    forest_inst<CSRGraph<>>(m, "Forest__CSRGraph");
    forest_inst<RectangularGridGraph<>>(m, "Forest__RectangularGridGraph");
}

} // namespace whirlwind::bindings
