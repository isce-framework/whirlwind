#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/stddef.hpp>
#include <whirlwind/graph/csr_graph.hpp>
#include <whirlwind/graph/edge_list.hpp>
#include <whirlwind/graph/graph_traits.hpp>

#include "sequence.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

inline constexpr auto edge_list_add_edge_doc = R"(
    Add a new edge.

    Parameters
    ----------
    tail : Vertex
        The tail vertex of the edge.
    head : Vertex
        The head vertex of the edge.

    Returns
    -------
    EdgeList
        A reference to the `EdgeList` object.
    )";

template<class Vertex, template<class> class Container>
void
edge_list_attrs_and_methods(nb::class_<EdgeList<Vertex, Container>>& cls)
{
    using Class = EdgeList<Vertex, Container>;

    // Constructors.
    cls.def(nb::init<>());

    // Attributes & properties.
    cls.def_prop_ro("size", &Class::size);

    // Methods.
    cls.def("add_edge", &Class::add_edge, nb::rv_policy::reference_internal, "tail"_a,
            "head"_a, edge_list_add_edge_doc);
    cls.def("clear", &Class::clear, "Erase all edges from the container.");
    cls.def("empty", &Class::empty, "Check if the container is empty.");

    common_sequence_attrs_and_methods(cls);
}

void
edge_list(nb::module_& m)
{
    using Vertex = typename GraphTraits<CSRGraph<>>::vertex_type;
    auto edge_list = nb::class_<EdgeList<Vertex>>(m, "EdgeList");

    edge_list_attrs_and_methods(edge_list);
}

} // namespace whirlwind::bindings
