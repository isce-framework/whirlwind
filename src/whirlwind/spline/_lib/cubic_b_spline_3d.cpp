#include <tuple>
#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>
#include <range/v3/view/span.hpp>

#include <whirlwind/common/assert.hpp>
#include <whirlwind/spline/cubic_b_spline_3d.hpp>
#include <whirlwind/spline/cubic_b_spline_basis.hpp>

#include "array.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class>
void
tri_cubic_b_spline_attrs_and_methods(nb::class_<Class>& cls)
{
    using Knot = typename Class::knot_type;
    using Value = typename Class::value_type;
    using Basis = typename Class::basis_type;

    // Constructors.
    cls.def(
            "__init__",
            [](Class* self, const std::tuple<Basis, Basis, Basis>& bases,
               const PyContiguousArray3D<const Value>& control_points) {
                const auto control_points_span =
                        ranges::span(control_points.data(), control_points.size());
                new (self) Class(bases, control_points_span);
            },
            "bases"_a, "control_points"_a);
    cls.def(
            "__init__",
            [](Class* self, const Basis& basis0, const Basis& basis1,
               const Basis& basis2,
               const PyContiguousArray3D<const Value>& control_points) {
                const auto control_points_span =
                        ranges::span(control_points.data(), control_points.size());
                new (self) Class(basis0, basis1, basis2, control_points_span);
            },
            "basis0"_a, "basis1"_a, "basis2"_a, "control_points"_a);

    // Static attributes.
    cls.def_prop_ro_static("num_dims", [](nb::handle) { return Class::num_dims(); });

    // Attributes.
    cls.def_prop_ro(
            "knots",
            [](const Class& self) {
                const auto knots0 = self.knots(0);
                const auto knots1 = self.knots(1);
                const auto knots2 = self.knots(2);

                auto knots_array0 = NumPyArray1D<const Knot>(
                        knots0.data(), {knots0.size()}, nb::handle());
                auto knots_array1 = NumPyArray1D<const Knot>(
                        knots1.data(), {knots1.size()}, nb::handle());
                auto knots_array2 = NumPyArray1D<const Knot>(
                        knots2.data(), {knots2.size()}, nb::handle());

                return std::tuple{std::move(knots_array0), std::move(knots_array1),
                                  std::move(knots_array2)};
            },
            nb::rv_policy::reference_internal);
    cls.def_prop_ro(
            "control_points",
            [](const Class& self) {
                const auto& control_points = self.control_points();
                WHIRLWIND_ASSERT(is_contiguous_range(control_points.container()));
                return NumPyArray3D<const Value>(control_points.data(),
                                                 {control_points.extent(0),
                                                  control_points.extent(1),
                                                  control_points.extent(2)},
                                                 nb::handle());
            },
            nb::rv_policy::reference_internal);

    // Dunder methods.
    cls.def("__call__", &Class::operator(), "x0"_a, "x1"_a, "x2"_a,
            nb::call_guard<nb::gil_scoped_release>());
}

template<class T>
void
tri_cubic_b_spline(nb::module_& m, const char* name)
{
    using Class = TriCubicBSpline<T>;
    auto cls = nb::class_<Class>(m, name);
    tri_cubic_b_spline_attrs_and_methods(cls);
}

void
tri_cubic_b_spline(nb::module_& m)
{
    using f32 = float;
    using f64 = double;

    tri_cubic_b_spline<f32>(m, "TriCubicBSpline__f32");
    tri_cubic_b_spline<f64>(m, "TriCubicBSpline__f64");
}

} // namespace whirlwind::bindings
