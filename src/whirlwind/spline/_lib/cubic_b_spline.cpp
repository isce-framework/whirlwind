#include <span>

#include <nanobind/nanobind.h>

#include <whirlwind/common/assert.hpp>
#include <whirlwind/spline/cubic_b_spline.hpp>
#include <whirlwind/spline/cubic_b_spline_basis.hpp>

#include "array.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class>
void
cubic_b_spline_attrs_and_methods(nb::class_<Class>& cls)
{
    using Knot = typename Class::knot_type;
    using Value = typename Class::value_type;
    using Basis = typename Class::basis_type;

    // Constructors.
    cls.def(
            "__init__",
            [](Class* self, const Basis& basis,
               const PyContiguousArray1D<const Value>& control_points) {
                const auto control_points_span =
                        std::span(control_points.data(), control_points.size());
                new (self) Class(basis, control_points_span);
            },
            "basis"_a, "control_points"_a);

    // Static attributes.
    cls.def_prop_ro_static("num_dims", [](nb::handle) { return Class::num_dims(); });

    // Attributes.
    cls.def_prop_ro(
            "knots",
            [](const Class& self) {
                const auto knots = self.knots();
                return NumPyArray1D<const Knot>(knots.data(), {knots.size()},
                                                nb::handle());
            },
            nb::rv_policy::reference_internal);
    cls.def_prop_ro(
            "control_points",
            [](const Class& self) {
                const auto& control_points = self.control_points();
                WHIRLWIND_ASSERT(is_contiguous_range(control_points.container()));
                return NumPyArray1D<const Value>(control_points.data(),
                                                 {control_points.size()}, nb::handle());
            },
            nb::rv_policy::reference_internal);

    // Dunder methods.
    cls.def(
            "__call__", [](const Class& self, const Knot& x) { return self(x); }, "x"_a,
            nb::call_guard<nb::gil_scoped_release>());
    cls.def(
            "__call__",
            [](const Class& self, const PyContiguousArray<const Knot>& x) {
                const auto x_span = std::span(x.data(), x.size());

                auto y = [&]() {
                    [[maybe_unused]] const nb::gil_scoped_release nogil;
                    return self(x_span);
                }();

                const auto shape = shape_of(x);
                return to_numpy_array(std::move(y), shape);
            },
            "x"_a);
}

template<class T>
void
cubic_b_spline(nb::module_& m, const char* name)
{
    using Class = CubicBSpline<T>;
    auto cls = nb::class_<Class>(m, name);
    cubic_b_spline_attrs_and_methods(cls);
}

void
cubic_b_spline(nb::module_& m)
{
    using f32 = float;
    using f64 = double;

    cubic_b_spline<f32>(m, "CubicBSpline__f32");
    cubic_b_spline<f64>(m, "CubicBSpline__f64");
}

} // namespace whirlwind::bindings
