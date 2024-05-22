#pragma once

#include <cstddef>
#include <utility>

#include <nanobind/ndarray.h>

#include <whirlwind/common/ndarray.hpp>
#include <whirlwind/common/ndspan.hpp>

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class T, std::size_t Rank>
using PyContiguousArrayND =
        nb::ndarray<T, nb::ndim<Rank>, nb::c_contig, nb::device::cpu>;

template<class T>
using PyContiguousArray1D = PyContiguousArrayND<T, 1>;

template<class T>
using PyContiguousArray2D = PyContiguousArrayND<T, 2>;

template<class T>
using PyContiguousArray3D = PyContiguousArrayND<T, 3>;

template<class T, std::size_t Rank>
using NumPyArrayND = nb::ndarray<T, nb::numpy, nb::ndim<Rank>>;

template<class T>
using NumPyArray1D = NumPyArrayND<T, 1>;

template<class T>
using NumPyArray2D = NumPyArrayND<T, 2>;

template<class T>
using NumPyArray3D = NumPyArrayND<T, 3>;

template<class T>
[[nodiscard]] constexpr auto
ndspan_of(const PyContiguousArray1D<T>& arr) -> Span1D<T>
{
    return Span1D<T>(arr.data(), arr.shape(0));
}

template<class T>
[[nodiscard]] constexpr auto
ndspan_of(const PyContiguousArray2D<T>& arr) -> Span2D<T>
{
    return Span2D<T>(arr.data(), arr.shape(0), arr.shape(1));
}

template<class T>
[[nodiscard]] constexpr auto
ndspan_of(const PyContiguousArray3D<T>& arr) -> Span3D<T>
{
    return Span3D<T>(arr.data(), arr.shape(0), arr.shape(1), arr.shape(2));
}

template<class T, class Container>
[[nodiscard]] constexpr auto
to_numpy_array(Array1D<T, Container> arr) -> NumPyArray1D<T>
{
    auto out = new auto(std::move(arr));
    auto owner = nb::capsule(
            out, [](void* p) noexcept { delete static_cast<decltype(out)>(p); });
    return NumPyArray1D<T>(out->data(), {out->size()}, std::move(owner));
}

template<class T, class Container>
[[nodiscard]] constexpr auto
to_numpy_array(Array2D<T, Container> arr) -> NumPyArray2D<T>
{
    auto out = new auto(std::move(arr));
    auto owner = nb::capsule(
            out, [](void* p) noexcept { delete static_cast<decltype(out)>(p); });
    return NumPyArray2D<T>(out->data(), {out->extent(0), out->extent(1)},
                           std::move(owner));
}

template<class T, class Container>
[[nodiscard]] constexpr auto
to_numpy_array(Array3D<T, Container> arr) -> NumPyArray3D<T>
{
    auto out = new auto(std::move(arr));
    auto owner = nb::capsule(
            out, [](void* p) noexcept { delete static_cast<decltype(out)>(p); });
    return NumPyArray3D<T>(out->data(),
                           {out->extent(0), out->extent(1), out->extent(2)},
                           std::move(owner));
}

} // namespace whirlwind::bindings
