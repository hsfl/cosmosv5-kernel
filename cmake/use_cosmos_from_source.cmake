if(NOT DEFINED COSMOS_KERNEL_INCLUDED)
set(COSMOS_KERNEL_INCLUDED TRUE)

include(${COSMOS_SOURCE}/thirdparty/cmake/use_cosmos_from_source.cmake)

set(COSMOS_SOURCE_KERNEL ${COSMOS_SOURCE}/kernel)
message("Using COSMOS kernel from " ${COSMOS_SOURCE_KERNEL})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${COSMOS_SOURCE_KERNEL}/libraries)

add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/math      ${CMAKE_BINARY_DIR}/kernel/libraries/math)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/support   ${CMAKE_BINARY_DIR}/kernel/libraries/support)

include_directories(${COSMOS_SOURCE_KERNEL}/libraries/device)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/device/disk   ${CMAKE_BINARY_DIR}/kernel/libraries/device/disk)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/device/i2c    ${CMAKE_BINARY_DIR}/kernel/libraries/device/i2c)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/device/serial ${CMAKE_BINARY_DIR}/kernel/libraries/device/serial)

endif()
