# Add Python extension module.
nanobind_add_module(network-pymodule NB_DOMAIN whirlwind NOMINSIZE)
target_sources(
  network-pymodule
  PRIVATE # cmake-format: sortable
          module.cpp
          network.cpp
          primal_dual.cpp
          residual_graph.cpp
          successive_shortest_paths.cpp
          uncapacitated.cpp
          unit_capacity.cpp
)
target_include_directories(
  network-pymodule PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(network-pymodule PRIVATE whirlwind::whirlwind)

# Rename the module object. The base name of the installed object must match the name of
# the Python extension module produced by `NB_MODULE` in the bindings source file.
set_target_properties(network-pymodule PROPERTIES OUTPUT_NAME _lib)

# Install the extension module. Scikit-build-core will set the destination directory to
# the Python platlib path (i.e. site-packages).
install(TARGETS network-pymodule LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME}/network)
