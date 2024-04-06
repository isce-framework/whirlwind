#pragma once

#include <nanobind/nanobind.h>

#include <whirlwind/common/type_traits.hpp>

#include "iterable.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class Container>
void
common_sequence_attrs_and_methods(nb::class_<Container>& cls)
{
    // Dunder methods.
    cls.def("__len__", &Container::size);
    cls.def("__getitem__", nb::overload_cast<typename Container::size_type>(
                                   &Container::operator[], nb::const_));

    common_iterable_attrs_and_methods(cls);
}

} // namespace whirlwind::bindings
