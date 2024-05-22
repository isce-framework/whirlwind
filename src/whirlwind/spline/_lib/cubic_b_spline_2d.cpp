#include <span>
#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <whirlwind/common/assert.hpp>
#include <whirlwind/spline/cubic_b_spline_2d.hpp>
#include <whirlwind/spline/cubic_b_spline_basis.hpp>

#include "array.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class>
void
bi_cubic_b_spline_attrs_and_methods(nb::class_<Class>& cls)
{
    using Knot = typename Class::knot_type;
    using Value = typename Class::value_type;
    using Basis = typename Class::basis_type;

    // Constructors.
    cls.def(
            "__init__",
            [](Class* self, const std::pair<Basis, Basis>& bases,
               const PyContiguousArray2D<const Value>& control_points) {
                const auto control_points_span =
                        std::span(control_points.data(), control_points.size());
                new (self) Class(bases, control_points_span);
            },
            "bases"_a, "control_points"_a);
    cls.def(
            "__init__",
            [](Class* self, const Basis& basis0, const Basis& basis1,
               const PyContiguousArray2D<const Value>& control_points) {
                const auto control_points_span =
                        std::span(control_points.data(), control_points.size());
                new (self) Class(basis0, basis1, control_points_span);
            },
            "basis0"_a, "basis1"_a, "control_points"_a);

    // Static attributes.
    cls.def_prop_ro_static("num_dims", [](nb::handle) { return Class::num_dims(); });

    // Attributes.
    cls.def_prop_ro(
            "knots",
            [](const Class& self) {
                const auto knots0 = self.knots(0);
                const auto knots1 = self.knots(1);

                auto knots_array0 = NumPyArray1D<const Knot>(
                        knots0.data(), {knots0.size()}, nb::handle());
                auto knots_array1 = NumPyArray1D<const Knot>(
                        knots1.data(), {knots1.size()}, nb::handle());

                return std::pair{std::move(knots_array0), std::move(knots_array1)};
            },
            nb::rv_policy::reference_internal);
    cls.def_prop_ro(
            "control_points",
            [](const Class& self) {
                const auto& control_points = self.control_points();
                WHIRLWIND_ASSERT(is_contiguous_range(control_points.container()));
                return NumPyArray2D<const Value>(
                        control_points.data(),
                        {control_points.extent(0), control_points.extent(1)},
                        nb::handle());
            },
            nb::rv_policy::reference_internal);

    // Dunder methods.
    cls.def(
            "__call__",
            [](const Class& self, const Knot& x0, const Knot& x1) {
                return self(x0, x1);
            },
            "x0"_a, "x1"_a, nb::call_guard<nb::gil_scoped_release>());
    cls.def(
            "__call__",
            [](const Class& self, const PyContiguousArray<const Knot>& x0,
               const PyContiguousArray<const Knot>& x1) {
                const auto shape = shape_of(x0);
                WHIRLWIND_ASSERT(shape_of(x1) == shape);

                const auto x0_span = std::span(x0.data(), x0.size());
                const auto x1_span = std::span(x1.data(), x1.size());

                auto y = [&]() {
                    [[maybe_unused]] const nb::gil_scoped_release nogil;
                    return self(x0_span, x1_span);
                }();

                return to_numpy_array(std::move(y), shape);
            },
            "x0"_a, "x1"_a);
}

template<class T>
void
bi_cubic_b_spline(nb::module_& m, const char* name)
{
    using Class = BiCubicBSpline<T>;
    auto cls = nb::class_<Class>(m, name);
    bi_cubic_b_spline_attrs_and_methods(cls);
}

void
bi_cubic_b_spline(nb::module_& m)
{
    using f32 = float;
    using f64 = double;

    bi_cubic_b_spline<f32>(m, "BiCubicBSpline__f32");
    bi_cubic_b_spline<f64>(m, "BiCubicBSpline__f64");
}

} // namespace whirlwind::bindings
