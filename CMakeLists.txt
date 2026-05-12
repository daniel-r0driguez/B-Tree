cmake_minimum_required(VERSION 3.27.3)

project(BTREE_TESTING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -static")
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    main.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
else ()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif ()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

add_subdirectory(${CMAKE_SOURCE_DIR}/btree) # Goes to the libraries CMakeLists.txt
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/btree/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/btree)
target_link_libraries(${PROJECT_NAME} BTREE)