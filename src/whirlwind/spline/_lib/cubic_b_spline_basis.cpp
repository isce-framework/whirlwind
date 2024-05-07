#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/array.h>
#include <range/v3/view/span.hpp>

#include <whirlwind/spline/cubic_b_spline_basis.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class T>
using PyArray1D = nb::ndarray<T, nb::ndim<1>, nb::c_contig, nb::device::cpu>;

template<class T>
using NumPyArray1D = nb::ndarray<T, nb::numpy, nb::ndim<1>>;

template<class Class>
void
cubic_b_spline_basis_attrs_and_methods(nb::class_<Class>& cls)
{
    using Knot = typename Class::knot_type;

    // Constructors.
    cls.def(
            "__init__",
            [](Class* self, const PyArray1D<const Knot>& knots) {
                auto knots_span = ranges::span(knots.data(), knots.size());
                new (self) Class(std::move(knots_span));
            },
            "knots"_a, nb::call_guard<nb::gil_scoped_release>());

    // Attributes.
    cls.def_prop_ro(
            "knots",
            [](const Class& self) {
                const auto knots = self.knots();
                return NumPyArray1D<const Knot>(knots.data(), {knots.size()},
                                                nb::handle());
            },
            nb::rv_policy::reference_internal);
    cls.def_prop_ro("num_knot_intervals", &Class::num_knot_intervals);
    cls.def_prop_ro("num_basis_funcs", &Class::num_basis_funcs);

    // Dunder methods.
    cls.def("get_knot_interval", &Class::get_knot_interval, "x"_a);
    cls.def("eval_in_interval", &Class::eval_in_interval, "x"_a, "i"_a);
    cls.def("eval_derivative_in_interval", &Class::eval_derivative_in_interval, "x"_a,
            "i"_a);
    cls.def("eval_second_derivative_in_interval",
            &Class::eval_second_derivative_in_interval, "x"_a, "i"_a);
}

template<class Knot>
void
cubic_b_spline_basis(nb::module_& m, const char* name)
{
    using Class = CubicBSplineBasis<Knot>;
    auto cls = nb::class_<Class>(m, name);
    cubic_b_spline_basis_attrs_and_methods(cls);
}

void
cubic_b_spline_basis(nb::module_& m)
{
    using f32 = float;
    using f64 = double;

    cubic_b_spline_basis<f32>(m, "CubicBSplineBasis__f32");
    cubic_b_spline_basis<f64>(m, "CubicBSplineBasis__f64");
}

} // namespace whirlwind::bindings
