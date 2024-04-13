#pragma once

#include <nanobind/nanobind.h>

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class Class, class... Extra>
void
common_capacity_mixin_attrs_and_methods(nb::class_<Class, Extra...>& cls)
{
    // Methods.
    cls.def("arc_capacity", &Class::arc_capacity, "arc"_a);
    cls.def("is_arc_saturated", &Class::is_arc_saturated, "arc"_a);
    cls.def("arc_residual_capacity", &Class::arc_residual_capacity, "arc"_a);
    cls.def("increase_arc_flow", &Class::increase_arc_flow, "arc"_a, "delta"_a);
}

} // namespace whirlwind::bindings
