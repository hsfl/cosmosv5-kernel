cmake_minimum_required(VERSION 3.20)

FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(CosmosDeviceSerial STATIC ${SOURCES} ${INCLUDES})
target_link_libraries(CosmosDeviceSerial CosmosTime CosmosSupport ${COSMOS_LIBS})

install(TARGETS CosmosDeviceSerial DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include/device/serial)

message("Build CosmosDeviceSerial from ${CMAKE_CURRENT_SOURCE_DIR}")
