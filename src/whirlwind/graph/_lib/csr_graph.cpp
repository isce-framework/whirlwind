#include <nanobind/nanobind.h>

#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/edge_list.hpp>

#include "graph.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class>
void
csr_graph_attrs_and_methods(nb::class_<Class>& cls)
{
    using Vertex = typename Class::vertex_type;

    // Constructors.
    cls.def(nb::init<>());
    cls.def(nb::init<EdgeList<Vertex>>(), "edge_list"_a,
            nb::call_guard<nb::gil_scoped_release>());

    common_graph_attrs_and_methods(cls);
}

void
csr_graph(nb::module_& m)
{
    auto graph = nb::class_<CSRGraph<>>(m, "CSRGraph");
    csr_graph_attrs_and_methods(graph);
}

} // namespace whirlwind::bindings
