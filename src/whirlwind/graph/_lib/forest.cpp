#include <utility>

#include <nanobind/nanobind.h>

#include <whirlwind/common/type_traits.hpp>
#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/forest.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

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
            nb::keep_alive<1, 2>());

    // Attributes.
    cls.def_prop_ro("graph", &Class::graph, nb::rv_policy::reference_internal);
    cls.def_prop_ro("edge_fill_value", &Class::edge_fill_value);

    // Methods.
    cls.def("depth", &Class::depth, "vertex"_a);
    cls.def("predecessor_vertex", &Class::predecessor_vertex, "vertex"_a);
    cls.def("predecessor_edge", &Class::predecessor_edge, "vertex"_a);
    cls.def("predecessor", &Class::predecessor, "vertex"_a);
    cls.def("predecessors", &Class::predecessors, "vertex"_a, nb::keep_alive<0, 1>());
    cls.def("set_predecessor",
            nb::overload_cast<const Vertex&, Vertex, Edge>(&Class::set_predecessor),
            "vertex"_a, "pred_vertex"_a, "pred_edge"_a);
    cls.def("make_root_vertex", &Class::make_root_vertex, "vertex"_a);
    cls.def("is_root_vertex", &Class::is_root_vertex, "vertex"_a);
    cls.def("reset", &Class::reset);
}

template<class Graph>
void
forest(nb::module_& m, const char* name)
{
    using Class = Forest<Graph>;
    using Vertex = Class::vertex_type;
    using Predecessors = remove_cvref_t<decltype(std::declval<Class>().predecessors(
            std::declval<Vertex>()))>;

    auto forest = nb::class_<Class>(m, name);
    auto predecessors = nb::class_<Predecessors>(forest, "_Predecessors");

    forest_attrs_and_methods(forest);
    common_iterable_attrs_and_methods(predecessors);

    m.def("Forest", [](const Graph& graph) { return Class(graph); });
}

void
forest(nb::module_& m)
{
    forest<CSRGraph<>>(m, "Forest__CSRGraph");
    forest<RectangularGridGraph<>>(m, "Forest__RectangularGridGraph");
}

} // namespace whirlwind::bindings
