if(NOT DEFINED COSMOS_KERNEL_INCLUDED)
set(COSMOS_KERNEL_INCLUDED TRUE)

get_filename_component(COSMOS_SOURCE_KERNEL "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

message("Using COSMOS kernel from " ${COSMOS_SOURCE_KERNEL})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build json11 from kernel's bundled copy.
# json11 has been removed from thirdparty; kernel is now the canonical source.
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/json11 ${CMAKE_BINARY_DIR}/kernel/libraries/json11)
include_directories(${COSMOS_SOURCE_KERNEL}/libraries/json11)

include_directories(${COSMOS_SOURCE_KERNEL}/libraries)

add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/math      ${CMAKE_BINARY_DIR}/kernel/libraries/math)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/support   ${CMAKE_BINARY_DIR}/kernel/libraries/support)

include_directories(${COSMOS_SOURCE_KERNEL}/libraries/device)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/device/disk   ${CMAKE_BINARY_DIR}/kernel/libraries/device/disk)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/device/i2c    ${CMAKE_BINARY_DIR}/kernel/libraries/device/i2c)
add_subdirectory(${COSMOS_SOURCE_KERNEL}/libraries/device/serial ${CMAKE_BINARY_DIR}/kernel/libraries/device/serial)

endif()
