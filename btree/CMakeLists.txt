cmake_minimum_required(VERSION 3.27.3)

project(BTREE)

set(SOURCE_FILES
    src/Application.cpp

    # Template src files
    include/BTREE/detail/BTree.tpp
    include/BTREE/detail/BTreeNode.tpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)