#pragma once

#include <stdexcept>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

namespace whirlwind::bindings {

namespace nb = nanobind;

template<template<class, class> class Pair, class T1, class T2>
void
common_pair_attrs_and_methods(nb::class_<Pair<T1, T2>>& cls)
{
    using Class = Pair<T1, T2>;

    // Constructors.
    cls.def(nb::init<T1, T2>());

    // Dunder methods.
    cls.def("__len__", [](const Class&) { return 2; });
    cls.def("__getitem__", [](Class& self, int key) {
        if (key == 0 || key == -2) {
            return nb::cast(self.first);
        }
        if (key == 1 || key == -1) {
            return nb::cast(self.second);
        }
        throw std::out_of_range("tuple index out of range");
    });
    cls.def("__repr__", [](const Class& self) {
        return nb::str("({}, {})").format(self.first, self.second);
    });

    // Operator overloads.
    cls.def(nb::self == nb::self);
    cls.def(nb::self != nb::self);
}

} // namespace whirlwind::bindings
