cmake_minimum_required(VERSION 3.20)

FILE(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB INCLUDESPP ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Base config, error types, and cosmos-defs (header-only STL wrappers)
add_library(CosmosSupport STATIC
    cosmos-defs.h
    configCosmos.h
    configCosmosKernel.h
    cosmos-errno.h
    cosmos-errclass.h cosmos-errclass.cpp)
target_link_libraries(CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosSupport DESTINATION lib)

# SLIP framing
add_library(CosmosSlip STATIC sliplib.cpp sliplib.h)
target_link_libraries(CosmosSlip ${COSMOS_LIBS})
install(TARGETS CosmosSlip DESTINATION lib)

# Print utilities
add_library(CosmosPrint STATIC print_utils.cpp print_utils.h)
target_link_libraries(CosmosPrint CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosPrint DESTINATION lib)

# Strings, jsonobject, jsonvalue (json11 lives in thirdparty/libraries/json11)
add_library(CosmosString STATIC
    stringlib.cpp stringlib.h
    jsonobject.cpp jsonobject.h
    jsonvalue.cpp jsonvalue.h)
target_link_libraries(CosmosString json11 CosmosMath CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosString DESTINATION lib)

# JSON class (jsonclass.h — structural JSON, no namespace deps)
add_library(CosmosJson STATIC jsonclass.cpp jsonclass.h)
target_link_libraries(CosmosJson CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosJson DESTINATION lib)

# Time: timelib, timeutils, elapsedtime bundled to avoid layering issue (#82)
add_library(CosmosTime STATIC
    timelib.cpp timelib.h
    timeutils.cpp timeutils.h
    elapsedtime.cpp elapsedtime.h)
target_link_libraries(CosmosTime CosmosMath CosmosString CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosTime DESTINATION lib)

# Packet framing: packetcomm + ax25class
add_library(CosmosPacket STATIC packetcomm.cpp packetcomm.h ax25class.cpp ax25class.h)
target_link_libraries(CosmosPacket CosmosMath CosmosSlip CosmosTime CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosPacket DESTINATION lib)

# Channel multiplexing (depends on CosmosPacket + CosmosTime)
add_library(CosmosChannel STATIC channellib.cpp channellib.h)
target_link_libraries(CosmosChannel CosmosPacket CosmosTime CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosChannel DESTINATION lib)

# Lightweight error-checking utility (split from CosmosLog which needs socketlib)
add_library(CosmosCheck STATIC check.cpp check.h)
target_link_libraries(CosmosCheck CosmosSupport ${COSMOS_LIBS})
install(TARGETS CosmosCheck DESTINATION lib)

# convertdef.h is header-only; install alongside support headers
install(FILES ${INCLUDES} DESTINATION include/support)
install(FILES ${INCLUDESPP} DESTINATION include/support)
