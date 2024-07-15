#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/version.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;

// clang-format off
void network(nb::module_&);
void primal_dual(nb::module_&);
void residual_graph(nb::module_&);
void successive_shortest_paths(nb::module_&);
void uncapacitated(nb::module_&);
void unit_capacity(nb::module_&);
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

    whirlwind::bindings::residual_graph(m);
    whirlwind::bindings::uncapacitated(m);
    whirlwind::bindings::unit_capacity(m);
    whirlwind::bindings::network(m);
    whirlwind::bindings::successive_shortest_paths(m);
    whirlwind::bindings::primal_dual(m);
}
