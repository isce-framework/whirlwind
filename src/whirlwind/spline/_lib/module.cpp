#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/version.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;

// clang-format off
void bi_cubic_b_spline(nb::module_&);
void cubic_b_spline(nb::module_&);
void cubic_b_spline_basis(nb::module_&);
void tri_cubic_b_spline(nb::module_&);
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

    whirlwind::bindings::cubic_b_spline_basis(m);
    whirlwind::bindings::cubic_b_spline(m);
    whirlwind::bindings::bi_cubic_b_spline(m);
    whirlwind::bindings::tri_cubic_b_spline(m);
}
