#include <cstdint>
#include <utility>
#include <vector>

#include <nanobind/nanobind.h>

#include <whirlwind/common/stddef.hpp>
#include <whirlwind/graph/rectangular_grid_graph.hpp>
#include <whirlwind/integrate_unwrapped_gradients.hpp>
#include <whirlwind/network/network.hpp>
#include <whirlwind/network/uncapacitated.hpp>
#include <whirlwind/network/unit_capacity.hpp>

#include "array.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;
using namespace nb::literals;

template<class T,
         class Graph,
         class Cost,
         class Flow,
         // clang-format off
         template<class> class Container,
         // clang-format on
         class Mixin>
void
integrate_unwrapped_gradients(nb::module_& m)
{
    m.def(
            "integrate_unwrapped_gradients",
            [](const PyContiguousArray2D<const T>& wrapped_phase,
               const Network<Graph, Cost, Flow, Container, Mixin>& network) {
                const auto wrapped_phase_span = ndspan_of(wrapped_phase);

                auto unwrapped_phase = [&]() {
                    [[maybe_unused]] const nb::gil_scoped_release nogil;
                    return whirlwind::integrate_unwrapped_gradients(wrapped_phase_span,
                                                                    network);
                }();

                return to_numpy_array(std::move(unwrapped_phase));
            },
            "wrapped_phase"_a, "network"_a);
}

template<class T, class Graph, class Cost, class Flow, template<class> class Container>
void
integrate_unwrapped_gradients(nb::module_& m)
{
    using Uncapacitated = UncapacitatedMixin<Graph, Flow, Container>;
    integrate_unwrapped_gradients<T, Graph, Cost, Flow, Container, Uncapacitated>(m);

    using UnitCapacity = UnitCapacityMixin<Graph, Flow, Container>;
    integrate_unwrapped_gradients<T, Graph, Cost, Flow, Container, UnitCapacity>(m);
}

template<class T, class Graph, class Cost, class Flow>
void
integrate_unwrapped_gradients(nb::module_& m)
{
    integrate_unwrapped_gradients<T, Graph, Cost, Flow, std::vector>(m);
}

template<class T, class Graph, class Cost>
void
integrate_unwrapped_gradients(nb::module_& m)
{
    integrate_unwrapped_gradients<T, Graph, Cost, std::int32_t>(m);
}

template<class T, class Graph>
void
integrate_unwrapped_gradients(nb::module_& m)
{
    integrate_unwrapped_gradients<T, Graph, float>(m);
    integrate_unwrapped_gradients<T, Graph, std::int32_t>(m);
}

template<class T>
void
integrate_unwrapped_gradients(nb::module_& m)
{
    using Graph = RectangularGridGraph<1, Size>;
    integrate_unwrapped_gradients<T, Graph>(m);
}

void
integrate_unwrapped_gradients(nb::module_& m)
{
    integrate_unwrapped_gradients<float>(m);
    integrate_unwrapped_gradients<double>(m);
}

} // namespace whirlwind::bindings
