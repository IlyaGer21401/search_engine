cmake_minimum_required(VERSION 3.24)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable(
        search_engine
        main.cpp
        ConverterJSON/ConverterJSON.cpp
        ConverterJSON/ConverterJSON.h
        InvertedIndex/InvertedIndex.cpp
        InvertedIndex/InvertedIndex.h
        SearchServer/SearchServer.cpp
        SearchServer/SearchServer.h
        INVISIBLE_EXCEPTIONS.h)
target_include_directories(
        search_engine PUBLIC
        JSON_files
        resource
        ConverterJSON
        InvertedIndex
        SearchServer
)
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        hello_test
        InvertedIndex/InvertedIndexTests.cc
        InvertedIndex/InvertedIndex.h
        InvertedIndex/InvertedIndex.cpp
        SearchServer/SearchServerTest.cc
        SearchServer/SearchServer.h
        SearchServer/SearchServer.cpp
)
target_link_libraries(
        hello_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)