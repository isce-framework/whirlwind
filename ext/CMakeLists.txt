if(NOT IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/libwhirlwind)
  message(
    FATAL_ERROR
      "The libwhirlwind submodule was not found. Did you forget to clone with"
      " `--recursive`? If so, use `git submodule update --init --recursive` to add"
      " the missing submodule(s)."
  )
endif()

add_subdirectory(libwhirlwind)
