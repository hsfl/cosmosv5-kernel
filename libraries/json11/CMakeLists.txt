cmake_minimum_required(VERSION 3.20)

# json11 — MIT license (Copyright 2013 Dropbox, Inc.)
# Bundled directly in cosmosv5-kernel so kernel has no thirdparty dependency.
# Copyright notice must be preserved in all distributions (see json11.hpp header).
add_library(json11 STATIC json11.cpp json11.hpp)
install(TARGETS json11 DESTINATION lib)
install(FILES json11.hpp DESTINATION include/kernel/json11)
