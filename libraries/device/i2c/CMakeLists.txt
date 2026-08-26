cmake_minimum_required(VERSION 3.20)

FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(CosmosDeviceI2C STATIC ${SOURCES} ${INCLUDES})
# Note: core linked CosmosDeviceGeneral here but that lives in micro-agent.
# Verify whether this actually needs anything beyond CosmosSupport.
target_link_libraries(CosmosDeviceI2C CosmosTime CosmosSupport ${COSMOS_LIBS})

install(TARGETS CosmosDeviceI2C DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include/device/i2c)

message("Build CosmosDeviceI2C from ${CMAKE_CURRENT_SOURCE_DIR}")
