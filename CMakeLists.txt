# guessed...
cmake_minimum_required(VERSION 2.6)
PROJECT(uci-facts C)

set(BUILD_SHARED_LIBS true)

INCLUDE(FindPkgConfig)
pkg_search_module(LUA REQUIRED lua5.1)
pkg_get_variable(LUA_CMOD_DESTINATION lua5.1 INSTALL_CMOD)
pkg_search_module(JSON REQUIRED json-c)

add_library(uci-facts uci-facts.c)
target_link_libraries(uci-facts uci)
target_link_libraries(uci-facts ${UNL_LIBRARIES})
target_include_directories(uci-facts PUBLIC ${UNL_INCLUDE_DIRS})
install(TARGETS uci-facts
	LIBRARY DESTINATION lib)

add_library(uci-facts-respondd uci-facts-respondd.c)
target_link_libraries(uci-facts-respondd uci-facts)
target_link_libraries(uci-facts-respondd uci-facts ${JSON_LIBRARIES})
target_include_directories(uci-facts-respondd PUBLIC ${JSON_INCLUDE_DIRS})
install(FILES $<TARGET_FILE:uci-facts-respondd>
	RENAME uci-facts.so
	DESTINATION lib/gluon/respondd/)

add_library(uci-facts-lua uci-facts-lua.c)
target_include_directories(uci-facts-lua PUBLIC ${LUA_INCLUDE_DIRS})
target_link_libraries(uci-facts-lua uci-facts ${LUA_LIBRARIES})
install(FILES $<TARGET_FILE:uci-facts-lua>
	RENAME uci_facts.so
	DESTINATION ${LUA_CMOD_DESTINATION})
install(FILES gluon-announce.lua
	RENAME uci_facts
	DESTINATION lib/gluon/announce/nodeinfo.d)

enable_testing()

add_executable(uci-facts-test uci-facts-test.c)
target_link_libraries(uci-facts-test uci-facts)
add_test(NAME uci-facts-test COMMAND uci-facts-test)

add_executable(uci-facts-respondd-test uci-facts-respondd-test.c)
target_link_libraries(uci-facts-respondd-test uci-facts-respondd)
add_test(NAME uci-facts-respondd-test COMMAND uci-facts-respondd-test)

add_test(NAME uci-facts-lua-test COMMAND uci-facts-lua-test)
