#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/version.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;

// clang-format off
void residue(nb::module_&);
void integrate_unwrapped_gradients(nb::module_&);
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

    whirlwind::bindings::residue(m);
    whirlwind::bindings::integrate_unwrapped_gradients(m);
}
