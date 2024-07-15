#include <cstdint>
#include <string>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/heap.hpp>
#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/dijkstra.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>
#include <whirlwind/graph/shortest_path_forest.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class, class... Extra>
void
dijkstra_attrs_and_methods(nb::class_<Class, Extra...>& cls)
{
    using Graph = typename Class::graph_type;

    // Constructors.
    cls.def(nb::init<const Graph&>(), "graph"_a, nb::keep_alive<1, 2>());

    // Methods.
    cls.def("push_vertex", &Class::push_vertex, "vertex"_a, "distance"_a);
    cls.def("add_source", &Class::add_source, "source"_a);
    cls.def("pop_next_unvisited_vertex", &Class::pop_next_unvisited_vertex);
    cls.def("reach_vertex", &Class::reach_vertex, "edge"_a, "tail"_a, "head"_a,
            "distance"_a);
    cls.def("visit_vertex", &Class::visit_vertex, "vertex"_a, "distance"_a);
    cls.def("relax_edge", &Class::relax_edge, "edge"_a, "tail"_a, "head"_a,
            "distance"_a);
    cls.def("done", &Class::done);
    cls.def("reset", &Class::reset);
}

template<class Distance,
         class Graph, // clang-format off
         template<class> class Container, // clang-format on
         class Heap,
         class ShortestPathForest>
void
dijkstra(nb::module_& m, const std::string& name)
{
    using Class = Dijkstra<Distance, Graph, Container, Heap, ShortestPathForest>;
    using Parent = ShortestPathForest;

    auto dijkstra = nb::class_<Class, Parent>(m, name.c_str());
    dijkstra_attrs_and_methods(dijkstra);

    m.def("Dijkstra", [](const Graph& graph, Distance) { return Class(graph); });
}

template<class Distance, class Graph, template<class> class Container, class Heap>
void
dijkstra(nb::module_& m, const std::string& name)
{
    dijkstra<Distance, Graph, Container, Heap,
             ShortestPathForest<Distance, Graph, Container>>(m, name);
}

template<class Distance, class Graph, template<class> class Container>
void
dijkstra(nb::module_& m, const std::string& name)
{
    using Heap = BinaryHeap<typename Graph::vertex_type, Distance, Container>;
    dijkstra<Distance, Graph, Container, Heap>(m, name);
}

template<class Distance, class Graph>
void
dijkstra(nb::module_& m, const std::string& name)
{
    dijkstra<Distance, Graph, std::vector>(m, name);
}

template<class Distance>
void
dijkstra(nb::module_& m, const std::string& name)
{
    dijkstra<Distance, CSRGraph<>>(m, name + "_CSRGraph");
    dijkstra<Distance, RectangularGridGraph<>>(m, name + "_RectangularGridGraph");
}

void
dijkstra(nb::module_& m)
{
    using f64 = double;
    using i64 = std::int64_t;

    dijkstra<f64>(m, "Dijkstra__f64");
    dijkstra<i64>(m, "Dijkstra__i64");
}

} // namespace whirlwind::bindings
