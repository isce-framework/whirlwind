#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/version.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;

// clang-format off
void csr_graph(nb::module_&);
void dial(nb::module_&);
void dijkstra(nb::module_&);
void edge_list(nb::module_&);
void forest(nb::module_&);
void rectangular_grid_graph(nb::module_&);
void shortest_path_forest(nb::module_&);
// clang-format on

} // namespace whirlwind::bindings

// The name of the Python extension module produced by `NB_MODULE()` below must match
// the name of the CMake target produced by `nanobind_add_module()` in the corresponding
// CMakeLists.txt file.
NB_MODULE(_lib, m)
{
    m.attr("__version__") = WHIRLWIND_VERSION_STRING;
    m.attr("__version_tuple__") =
            std::pair(WHIRLWIND_VERSION_MAJOR, WHIRLWIND_VERSION_MINOR);

    whirlwind::bindings::edge_list(m);
    whirlwind::bindings::csr_graph(m);
    whirlwind::bindings::rectangular_grid_graph(m);
    whirlwind::bindings::forest(m);
    whirlwind::bindings::shortest_path_forest(m);
    whirlwind::bindings::dial(m);
    whirlwind::bindings::dijkstra(m);
}
