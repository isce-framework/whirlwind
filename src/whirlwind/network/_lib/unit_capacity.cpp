#include <cstdint>
#include <vector>

#include <nanobind/nanobind.h>

#include <whirlwind/network/residual_graph.hpp>
#include <whirlwind/network/unit_capacity.hpp>

#include "capacity.hpp"

namespace whirlwind::bindings {

namespace nb = nanobind;

template<class Graph,
         class Flow = std::int32_t,
         template<class> class Container = std::vector>
void
unit_capacity(nb::module_& m, const char* name)
{
    using Parent = ResidualGraphMixin<Graph, Container>;
    using Class = UnitCapacityMixin<Graph, Flow, Container, Parent>;
    auto cls = nb::class_<Class, Parent>(m, name);
    common_capacity_mixin_attrs_and_methods(cls);
}

void
unit_capacity(nb::module_& m)
{
    unit_capacity<RectangularGridGraph<>>(m, "UnitCapacityMixin__RectangularGridGraph");
}

} // namespace whirlwind::bindings
