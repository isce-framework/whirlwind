#include <utility>

#include <nanobind/nanobind.h>

#include <whirlwind/residue.hpp>

#include "array.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class T>
void
residue(nb::module_& m)
{
    m.def(
            "residue",
            [](const PyContiguousArray2D<const T>& wrapped_phase) {
                const auto wrapped_phase_span = ndspan_of(wrapped_phase);

                auto out = [&]() {
                    [[maybe_unused]] const nb::gil_scoped_release nogil;
                    return whirlwind::residue(wrapped_phase_span);
                }();

                return to_numpy_array(std::move(out));
            },
            "wrapped_phase"_a);
}

void
residue(nb::module_& m)
{
    residue<float>(m);
    residue<double>(m);
}

} // namespace whirlwind::bindings
