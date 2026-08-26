cmake_minimum_required(VERSION 3.20)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} : ")
message(${files})

foreach(file ${files})
    get_filename_component(bfile ${file} NAME_WE)
    add_executable(${bfile} ${bfile}.cpp)
    install(TARGETS ${bfile} DESTINATION bin)
endforeach()

target_link_libraries(calc_transform CosmosMath)
target_link_libraries(countdown CosmosTime)
target_link_libraries(json2tab CosmosJson)
target_link_libraries(mjd CosmosTime)
target_link_libraries(serial_talk CosmosDeviceSerial CosmosTime)
target_link_libraries(tab2json CosmosString CosmosJson)
