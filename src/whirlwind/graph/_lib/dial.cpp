#include <cstdint>
#include <string>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/queue.hpp>
#include <whirlwind/common/stddef.hpp>
#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/dial.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>
#include <whirlwind/graph/shortest_path_forest.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class, class... Extra>
void
dial_attrs_and_methods(nb::class_<Class, Extra...>& cls)
{
    using Graph = typename Class::graph_type;
    using Size = typename Class::size_type;
    using Distance = typename Class::distance_type;

    // Constructors.
    cls.def(nb::init<const Graph&, Size>(), "graph"_a, "num_buckets"_a,
            nb::keep_alive<1, 2>());

    // Attributes & properties.
    cls.def_prop_ro("num_buckets", &Class::num_buckets);
    cls.def_prop_ro("current_bucket_id", &Class::current_bucket_id);

    // Methods.
    cls.def("get_bucket_id", &Class::get_bucket_id, "distance"_a);
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
         class Queue,
         class ShortestPathForest>
void
dial(nb::module_& m, const std::string& name)
{
    using Class = Dial<Distance, Graph, Container, Queue, ShortestPathForest>;
    using Parent = ShortestPathForest;

    auto dial = nb::class_<Class, Parent>(m, name.c_str());
    dial_attrs_and_methods(dial);

    m.def("Dial", [](const Graph& graph, Size num_buckets) {
        return Class(graph, num_buckets);
    });
}

template<class Distance, class Graph, template<class> class Container, class Queue>
void
dial(nb::module_& m, const std::string& name)
{
    dial<Distance, Graph, Container, Queue,
         ShortestPathForest<Distance, Graph, Container>>(m, name);
}

template<class Distance, class Graph, template<class> class Container>
void
dial(nb::module_& m, const std::string& name)
{
    dial<Distance, Graph, Container, Queue<typename Graph::vertex_type>>(m, name);
}

template<class Distance, class Graph>
void
dial(nb::module_& m, const std::string& name)
{
    dial<Distance, Graph, std::vector>(m, name);
}

template<class Distance>
void
dial(nb::module_& m, const std::string& name)
{
    // dial<Distance, CSRGraph<>>(m, name + "_CSRGraph");
    dial<Distance, RectangularGridGraph<>>(m, name + "_RectangularGridGraph");
}

void
dial(nb::module_& m)
{
    dial<std::int64_t>(m, "Dial_");
}

} // namespace whirlwind::bindings
