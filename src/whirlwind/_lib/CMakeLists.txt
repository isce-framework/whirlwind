# Add Python extension module.
nanobind_add_module(whirlwind-pymodule NB_DOMAIN whirlwind)
target_sources(
  whirlwind-pymodule PRIVATE # cmake-format: sortable
                             integrate_unwrapped_gradients.cpp module.cpp residue.cpp
)
target_include_directories(
  whirlwind-pymodule PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(whirlwind-pymodule PRIVATE whirlwind::whirlwind)
target_compile_options(whirlwind-pymodule PRIVATE -fno-strict-aliasing)

# Rename the module object. The base name of the installed object must match the name of
# the Python extension module produced by `NB_MODULE` in the bindings source file.
set_target_properties(whirlwind-pymodule PROPERTIES OUTPUT_NAME _lib)

# Install the extension module. Scikit-build-core will set the destination directory to
# the Python platlib path (i.e. site-packages).
install(TARGETS whirlwind-pymodule LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})
