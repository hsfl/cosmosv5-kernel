cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(CosmosMath STATIC ${SOURCES} ${INCLUDES})
target_link_libraries(CosmosMath ${COSMOS_LIBS})

install(TARGETS CosmosMath DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include/math)
