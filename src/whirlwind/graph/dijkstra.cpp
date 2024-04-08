#include <cstdint>

#include <nanobind/nanobind.h>

#include <whirlwind/common/type_traits.hpp>
#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/dijkstra.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class, class... Extra>
void
dijkstra_attrs_and_methods(nb::class_<Class, Extra...>& cls)
{
    using Graph = typename Class::graph_type;

    // Constructors.
    cls.def(nb::init<const Graph&>(), "graph"_a);

    // Methods.
    cls.def("has_reached_vertex", &Class::has_reached_vertex, "vertex"_a);
    cls.def("has_visited_vertex", &Class::has_visited_vertex, "vertex"_a);
    cls.def("label_vertex_reached", &Class::label_vertex_reached, "vertex"_a);
    cls.def("label_vertex_visited", &Class::label_vertex_visited, "vertex"_a);
    cls.def("reached_vertices", &Class::reached_vertices);
    cls.def("visited_vertices", &Class::reached_vertices);
    cls.def("distance_to_vertex", &Class::distance_to_vertex, "vertex"_a);
    cls.def("set_distance_to_vertex", &Class::set_distance_to_vertex, "vertex"_a,
            "distance"_a);
    cls.def("add_source", &Class::add_source, "source"_a);
    cls.def("pop_next_unvisited_vertex", &Class::pop_next_unvisited_vertex);
    cls.def("visit_vertex", &Class::visit_vertex, "vertex"_a, "distance"_a);
    cls.def("relax_edge", &Class::relax_edge, "edge"_a, "tail"_a, "head"_a,
            "distance"_a);
    cls.def("done", &Class::done);
    cls.def("reset", &Class::reset);
}

template<class Distance, class Graph>
void
dijkstra_inst(nb::module_& m, const char* name)
{
    using Class = Dijkstra<Distance, Graph>;
    using Parent = Class::forest_type;
    using ReachedVertices =
            remove_cvref_t<decltype(std::declval<Class>().reached_vertices())>;
    using VisitedVertices =
            remove_cvref_t<decltype(std::declval<Class>().visited_vertices())>;

    auto dijkstra = nb::class_<Class, Parent>(m, name);
    dijkstra_attrs_and_methods(dijkstra);

    if (!nb::type<ReachedVertices>().is_valid()) {
        auto reached_vertices =
                nb::class_<ReachedVertices>(dijkstra, "_ReachedVertices");
        common_iterable_attrs_and_methods(reached_vertices);
    }

    if (!nb::type<VisitedVertices>().is_valid()) {
        auto visited_vertices =
                nb::class_<VisitedVertices>(dijkstra, "_VisitedVertices");
        common_iterable_attrs_and_methods(visited_vertices);
    }
}

void
dijkstra(nb::module_& m)
{
    using f64 = double;
    using i64 = std::int64_t;

    dijkstra_inst<f64, CSRGraph<>>(m, "Dijkstra__f64_CSRGraph");
    dijkstra_inst<i64, CSRGraph<>>(m, "Dijkstra__i64_CSRGraph");
    dijkstra_inst<f64, RectangularGridGraph<>>(m, "Dijkstra__f64_RectangularGridGraph");
    dijkstra_inst<i64, RectangularGridGraph<>>(m, "Dijkstra__i64_RectangularGridGraph");
}

} // namespace whirlwind::bindings
