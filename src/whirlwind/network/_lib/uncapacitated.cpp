#include <cstdint>
#include <vector>

#include <nanobind/nanobind.h>

#include <whirlwind/network/residual_graph.hpp>
#include <whirlwind/network/uncapacitated.hpp>

#include "capacity.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class Graph,
         class Flow = std::int32_t,
         template<class> class Container = std::vector>
void
uncapacitated(nb::module_& m, const char* name)
{
    using Parent = ResidualGraphMixin<Graph, Container>;
    using Class = UncapacitatedMixin<Graph, Flow, Container, Parent>;
    auto cls = nb::class_<Class, Parent>(m, name);
    common_capacity_mixin_attrs_and_methods(cls);
}

void
uncapacitated(nb::module_& m)
{
    uncapacitated<RectangularGridGraph<>>(m,
                                          "UncapacitatedMixin__RectangularGridGraph");
}

} // namespace whirlwind::bindings
