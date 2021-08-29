cmake_minimum_required(VERSION 3.11)

project(NDArray
  VERSION "0.1.0"
  DESCRIPTION "Numpy-like N-Dimentional array for C++"
  LANGUAGES CXX
)

include(CMakePackageConfigHelpers)

# Add options
option(NDARRAY_INSTALL "Install NDArray" ON)

add_library(NDArray INTERFACE)
# Add alias to make more friendly with FetchConent
add_library(NDArray::NDArray ALIAS NDArray)

# Add public interface includes directory
target_include_directories(NDArray INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Require C++11 standard
target_compile_features(NDArray INTERFACE cxx_std_11)

# Install NDArray
if(NDARRAY_INSTALL)
  include(GNUInstallDirs)

  configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/NDArrayConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/NDArrayConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NDArray
  )

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/NDArrayConfigVersion.cmake"
    VERSION "${NDArray_VERSION}"
    COMPATIBILITY SameMinorVersion
  )

  install(TARGETS NDArray
          EXPORT NDArrayTargets
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/ndarray.hpp"
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  install(EXPORT NDArrayTargets
          FILE NDArrayTargets.cmake
          NAMESPACE NDArray::
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NDArray
  )

  install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/NDArrayConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/NDArrayConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NDArray
  )
endif()
