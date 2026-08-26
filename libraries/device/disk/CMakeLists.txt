cmake_minimum_required(VERSION 3.20)

FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(CosmosDeviceDisk STATIC ${SOURCES} ${INCLUDES})
target_link_libraries(CosmosDeviceDisk CosmosSupport ${COSMOS_LIBS})

install(TARGETS CosmosDeviceDisk DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include/device/disk)

message("Build CosmosDeviceDisk from ${CMAKE_CURRENT_SOURCE_DIR}")
