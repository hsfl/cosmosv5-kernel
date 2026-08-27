if(NOT DEFINED COSMOS_KERNEL_INCLUDED)
set(COSMOS_KERNEL_INCLUDED TRUE)

get_filename_component(COSMOS_SOURCE_KERNEL "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

if(EXISTS "${COSMOS_SOURCE_KERNEL}/deps/thirdparty/cmake/use_cosmos_from_source.cmake")
    include(${COSMOS_SOURCE_KERNEL}/deps/thirdparty/cmake/use_cosmos_from_source.cmake)
elseif(DEFINED COSMOS_SOURCE)
    include(${COSMOS_SOURCE}/thirdparty/cmake/use_cosmos_from_source.cmake)
else()
    message(FATAL_ERROR "Cannot find cosmosv5-thirdparty. Clone with --recurse-submodules or set COSMOS_SOURCE.")
endif()

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
