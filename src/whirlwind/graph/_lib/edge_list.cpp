#include <nanobind/nanobind.h>

#include <whirlwind/common/stddef.hpp>
#include <whirlwind/graph/edge_list.hpp>

#include "sequence.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

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
    cls.def("add_edge", &Class::add_edge, "tail"_a, "head"_a,
            nb::rv_policy::reference_internal);
    cls.def("clear", &Class::clear);
    cls.def("empty", &Class::empty);

    common_sequence_attrs_and_methods(cls);
}

void
edge_list(nb::module_& m)
{
    auto edge_list = nb::class_<EdgeList<Size>>(m, "EdgeList");

    edge_list_attrs_and_methods(edge_list);
}

} // namespace whirlwind::bindings
