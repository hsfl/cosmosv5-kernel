if(NOT DEFINED COSMOS_KERNEL_INCLUDED)
set(COSMOS_KERNEL_INCLUDED TRUE)

get_filename_component(COSMOS_SOURCE_KERNEL "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

if(DEFINED COSMOS_SOURCE)
    # Auto-initialize the lower-layer submodule if not present
    if(NOT EXISTS "${COSMOS_SOURCE}/thirdparty/CMakeLists.txt")
        message(STATUS "cosmosv5: initializing thirdparty submodule...")
        execute_process(
            COMMAND git submodule update --init thirdparty
            WORKING_DIRECTORY "${COSMOS_SOURCE}"
            RESULT_VARIABLE _cosmos_git_result
        )
        if(NOT _cosmos_git_result EQUAL 0)
            message(FATAL_ERROR
                "cosmosv5: thirdparty submodule is not initialized and auto-init failed.\n"
                "Run from the cosmosv5 workspace root:\n"
                "  git submodule update --init thirdparty")
        endif()
    endif()
    include(${COSMOS_SOURCE}/thirdparty/cmake/use_cosmos_from_source.cmake)
else()
    message(FATAL_ERROR "COSMOS_SOURCE not set. Set COSMOS_SOURCE to the cosmosv5 workspace root (the directory containing thirdparty/, kernel/, etc.).")
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
