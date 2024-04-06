#include <nanobind/nanobind.h>

#include <whirlwind/graph/rectangular_grid_graph.hpp>

#include "graph.hpp"
#include "iterable.hpp"
#include "pair.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

inline constexpr auto rectangular_grid_graph_ctor_doc = R"(
    Create a new `RectangularGridGraph`.

    Parameters
    ----------
    num_rows : int
        The number of rows in the 2-D array of vertices.
    num_cols : int
        The number of columns in the 2-D array of vertices.
)";

template<class Dim>
void
rectangular_grid_graph_attrs_and_methods(nb::class_<RectangularGridGraph<Dim>>& cls)
{
    using Class = RectangularGridGraph<Dim>;

    // Constructors.
    cls.def(nb::init<>(), "Default constructor. Creates an empty `RectangularGridGraph`"
                          " with no vertices or edges.");
    cls.def(nb::init<Dim, Dim>(), "num_rows"_a, "num_cols"_a,
            rectangular_grid_graph_ctor_doc);

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

void
rectangular_grid_graph(nb::module_& m)
{
    auto graph = nb::class_<RectangularGridGraph<>>(m, "RectangularGridGraph");
    auto [vertices, edges, outgoing_edges] = graph_member_types(graph);
    auto vertices_value = iterator_value_type(vertices);

    rectangular_grid_graph_attrs_and_methods(graph);
    common_iterable_attrs_and_methods(vertices);
    common_pair_attrs_and_methods(vertices_value);
    common_iterable_attrs_and_methods(edges);
    common_iterable_attrs_and_methods(outgoing_edges);
}

} // namespace whirlwind::bindings
