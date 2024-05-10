#pragma once

#include <cstddef>

#include <nanobind/ndarray.h>

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class T, std::size_t N>
using PyContiguousArray = nb::ndarray<T, nb::ndim<N>, nb::c_contig, nb::device::cpu>;

template<class T>
using PyContiguousArray1D = PyContiguousArray<T, 1>;

template<class T>
using PyContiguousArray2D = PyContiguousArray<T, 2>;

template<class T>
using PyContiguousArray3D = PyContiguousArray<T, 3>;

template<class T, std::size_t N>
using NumPyArray = nb::ndarray<T, nb::numpy, nb::ndim<N>>;

template<class T>
using NumPyArray1D = NumPyArray<T, 1>;

template<class T>
using NumPyArray2D = NumPyArray<T, 2>;

template<class T>
using NumPyArray3D = NumPyArray<T, 3>;

} // namespace whirlwind::bindings
