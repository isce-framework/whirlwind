#pragma once

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>

#include <whirlwind/common/type_traits.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class Range>
void
common_iterable_attrs_and_methods(nb::class_<Range>& cls)
{
    // Dunder methods.
    cls.def(
            "__iter__",
            [](const Range& self) {
                return nb::make_iterator(nb::type<Range>(), "_Iterator", self);
            },
            nb::keep_alive<0, 1>{});
}

template<class Range>
auto
iterator_value_type(nb::class_<Range>& cls)
{
    return nb::class_<ranges::value_type<Range>>(cls, "_Value");
}

} // namespace whirlwind::bindings
