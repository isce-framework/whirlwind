#pragma once

#include <cstddef>
#include <vector>

#include <nanobind/ndarray.h>

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class T>
using PyContiguousArray = nb::ndarray<T, nb::c_contig, nb::device::cpu>;

template<class T, std::size_t Rank>
using PyContiguousArrayND =
        nb::ndarray<T, nb::ndim<Rank>, nb::c_contig, nb::device::cpu>;

template<class T>
using PyContiguousArray1D = PyContiguousArrayND<T, 1>;

template<class T>
using PyContiguousArray2D = PyContiguousArrayND<T, 2>;

template<class T>
using PyContiguousArray3D = PyContiguousArrayND<T, 3>;

template<class T>
using NumPyArray = nb::ndarray<T, nb::numpy>;

template<class T, std::size_t Rank>
using NumPyArrayND = nb::ndarray<T, nb::numpy, nb::ndim<Rank>>;

template<class T>
using NumPyArray1D = NumPyArrayND<T, 1>;

template<class T>
using NumPyArray2D = NumPyArrayND<T, 2>;

template<class T>
using NumPyArray3D = NumPyArrayND<T, 3>;

template<class... Args>
[[nodiscard]] constexpr auto
shape_of(const nb::ndarray<Args...>& arr) -> std::vector<std::size_t>
{
    const std::size_t ndim = arr.ndim();
    auto shape = std::vector<std::size_t>(ndim);
    for (std::size_t i = 0; i < ndim; ++i) {
        shape[i] = arr.shape(i);
    }
    return shape;
}

template<class T, class Allocator>
[[nodiscard]] auto
to_numpy_array(std::vector<T, Allocator> arr, const std::vector<std::size_t>& shape)
        -> NumPyArray<T>
{
    auto out = new auto(std::move(arr));
    auto owner = nb::capsule(
            out, [](void* p) noexcept { delete static_cast<decltype(out)>(p); });
    return NumPyArray<T>(out->data(), shape.size(), shape.data(), std::move(owner));
}

} // namespace whirlwind::bindings
