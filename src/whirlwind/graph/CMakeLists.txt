# Add Python extension module.
nanobind_add_module(graph-pymodule NB_DOMAIN whirlwind)
target_sources(
  graph-pymodule
  PRIVATE # cmake-format: sortable
          csr_graph.cpp edge_list.cpp forest.cpp module.cpp rectangular_grid_graph.cpp
)
target_include_directories(
  graph-pymodule PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(graph-pymodule PRIVATE whirlwind::whirlwind)

# Rename the module object. The base name of the installed object must match the name of
# the Python extension module produced by `NB_MODULE` in the bindings source file.
set_target_properties(graph-pymodule PROPERTIES OUTPUT_NAME _lib)

# Install the extension module. Scikit-build-core will set the destination directory to
# the Python platlib path (i.e. site-packages).
install(TARGETS graph-pymodule LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME}/graph)
