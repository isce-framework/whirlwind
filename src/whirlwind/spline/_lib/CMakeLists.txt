# Add Python extension module.
nanobind_add_module(spline-pymodule NB_DOMAIN whirlwind)
target_sources(
  spline-pymodule
  PRIVATE # cmake-format: sortable
          cubic_b_spline.cpp cubic_b_spline_2d.cpp cubic_b_spline_basis.cpp module.cpp
)
target_include_directories(
  spline-pymodule PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(spline-pymodule PRIVATE whirlwind::whirlwind)

# Rename the module object. The base name of the installed object must match the name of
# the Python extension module produced by `NB_MODULE` in the bindings source file.
set_target_properties(spline-pymodule PROPERTIES OUTPUT_NAME _lib)

# Install the extension module. Scikit-build-core will set the destination directory to
# the Python platlib path (i.e. site-packages).
install(TARGETS spline-pymodule LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME}/spline)
