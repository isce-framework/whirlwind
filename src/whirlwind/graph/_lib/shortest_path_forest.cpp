#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <nanobind/nanobind.h>

#include <whirlwind/common/type_traits.hpp>
#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/forest.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>
#include <whirlwind/graph/shortest_path_forest.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class, class... Extra>
void
shortest_path_forest_attrs_and_methods(nb::class_<Class, Extra...>& cls)
{
    using Graph = typename Class::graph_type;

    // Constructors.
    cls.def(nb::init<const Graph&>(), "graph"_a, nb::keep_alive<1, 2>());

    // Methods.
    cls.def("has_reached_vertex", &Class::has_reached_vertex, "vertex"_a);
    cls.def("has_visited_vertex", &Class::has_visited_vertex, "vertex"_a);
    cls.def("label_vertex_reached", &Class::label_vertex_reached, "vertex"_a);
    cls.def("label_vertex_visited", &Class::label_vertex_visited, "vertex"_a);
    cls.def("reached_vertices", &Class::reached_vertices, nb::keep_alive<0, 1>());
    cls.def("visited_vertices", &Class::reached_vertices, nb::keep_alive<0, 1>());
    cls.def("distance_to_vertex", &Class::distance_to_vertex, "vertex"_a);
    cls.def("set_distance_to_vertex", &Class::set_distance_to_vertex, "vertex"_a,
            "distance"_a);
    cls.def("reset", &Class::reset);
}

template<class Distance, class Graph, template<class> class Container, class Forest>
void
shortest_path_forest(nb::module_& m, const std::string& name)
{
    using Class = ShortestPathForest<Distance, Graph, Container, Forest>;
    using Parent = Forest;
    using ReachedVertices =
            remove_cvref_t<decltype(std::declval<Class>().reached_vertices())>;
    using VisitedVertices =
            remove_cvref_t<decltype(std::declval<Class>().visited_vertices())>;

    auto shortest_path_forest = nb::class_<Class, Parent>(m, name.c_str());
    shortest_path_forest_attrs_and_methods(shortest_path_forest);

    if (!nb::type<ReachedVertices>().is_valid()) {
        auto reached_vertices =
                nb::class_<ReachedVertices>(shortest_path_forest, "_ReachedVertices");
        common_iterable_attrs_and_methods(reached_vertices);
    }

    if (!nb::type<VisitedVertices>().is_valid()) {
        auto visited_vertices =
                nb::class_<VisitedVertices>(shortest_path_forest, "_VisitedVertices");
        common_iterable_attrs_and_methods(visited_vertices);
    }

    m.def("ShortestPathForest",
          [](const Graph& graph, Distance) { return Class(graph); });
}

template<class Distance, class Graph, template<class> class Container>
void
shortest_path_forest(nb::module_& m, const std::string& name)
{
    shortest_path_forest<Distance, Graph, Container, Forest<Graph, Container>>(m, name);
}

template<class Distance, class Graph>
void
shortest_path_forest(nb::module_& m, const std::string& name)
{
    shortest_path_forest<Distance, Graph, std::vector>(m, name);
}

template<class Distance>
void
shortest_path_forest(nb::module_& m, const std::string& name)
{
    shortest_path_forest<Distance, CSRGraph<>>(m, name + "_CSRGraph");
    shortest_path_forest<Distance, RectangularGridGraph<>>(
            m, name + "_RectangularGridGraph");
}

void
shortest_path_forest(nb::module_& m)
{
    using f64 = double;
    using i64 = std::int64_t;

    shortest_path_forest<f64>(m, "ShortestPathForest__f64");
    shortest_path_forest<i64>(m, "ShortestPathForest__i64");
}

} // namespace whirlwind::bindings
