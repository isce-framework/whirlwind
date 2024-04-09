#include <nanobind/nanobind.h>

#include <whirlwind/common/stddef.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>

#include "graph.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<Size P, class Dim>
void
rectangular_grid_graph_attrs_and_methods(nb::class_<RectangularGridGraph<P, Dim>>& cls)
{
    using Class = RectangularGridGraph<P, Dim>;

    // Constructors.
    cls.def(nb::init<>());
    cls.def(nb::init<Dim, Dim>(), "num_rows"_a, "num_cols"_a);

    // Static attributes
    cls.def_prop_ro_static("num_parallel_edges",
                           [](nb::handle) { return Class::num_parallel_edges(); });

    // Attributes.
    cls.def_prop_ro("num_rows", &Class::num_rows);
    cls.def_prop_ro("num_cols", &Class::num_cols);

    // Dunder methods.
    cls.def("__repr__", [](const Class& self) {
        return nb::str("RectangularGridGraph(num_rows={}, num_cols={})")
                .format(self.num_rows(), self.num_cols());
    });

    common_graph_attrs_and_methods(cls);
}

template<Size P>
void
rectangular_grid_graph_inst(nb::module_& m, const char* name)
{
    using Class = RectangularGridGraph<P>;
    auto graph = nb::class_<Class>(m, name);
    rectangular_grid_graph_attrs_and_methods(graph);
    graph_member_types(graph);
}

void
rectangular_grid_graph(nb::module_& m)
{
    rectangular_grid_graph_inst<1>(m, "RectangularGridGraph__1");
    rectangular_grid_graph_inst<2>(m, "RectangularGridGraph__2");
}

} // namespace whirlwind::bindings
