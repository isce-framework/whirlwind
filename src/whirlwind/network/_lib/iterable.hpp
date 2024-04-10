#pragma once

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class Range>
void
common_iterable_attrs_and_methods(nb::class_<Range>& cls)
{
    // Dunder methods.
    cls.def(
            "__iter__",
            [](Range& self) {
                return nb::make_iterator(nb::type<Range>(), "_Iterator", self);
            },
            nb::keep_alive<0, 1>());
}

} // namespace whirlwind::bindings
