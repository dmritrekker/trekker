include("${CMAKE_CURRENT_LIST_DIR}/utils.cmake")

conditional_make_directory("${CMAKE_INSTALL_PREFIX}/lib/nibrary_v${NIBRARY_MIN_VERSION}")
conditional_make_directory("${CMAKE_INSTALL_PREFIX}/include/nibrary_v${NIBRARY_MIN_VERSION}")

conditional_copy_directory("${CMAKE_BINARY_DIR}/nibrary/src/build_nibrary-build/install/lib/nibrary_v${NIBRARY_MIN_VERSION}" "${CMAKE_INSTALL_PREFIX}/lib/nibrary_v${NIBRARY_MIN_VERSION}")
conditional_copy_directory("${CMAKE_BINARY_DIR}/nibrary/src/build_nibrary-build/install/include/nibrary_v${NIBRARY_MIN_VERSION}" "${CMAKE_INSTALL_PREFIX}/include/nibrary_v${NIBRARY_MIN_VERSION}")
