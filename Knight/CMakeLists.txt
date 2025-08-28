add_subdirectory(Knight)

file(GLOB ABSOLUTE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/*)

list(REMOVE_ITEM ABSOLUTE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Knight)
list(REMOVE_ITEM ABSOLUTE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Knight.sln)
list(REMOVE_ITEM ABSOLUTE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

message("Building modules automatically")

foreach(ABSOLUTE_DIR ${ABSOLUTE_DIRS})
    cmake_path(GET ABSOLUTE_DIR STEM RELATIVE_DIR)
    
    file(GLOB ABSOLUTE_DIR_FILES ${ABSOLUTE_DIR}/*.cpp)

    add_executable(${RELATIVE_DIR} ${ABSOLUTE_DIR_FILES})
    target_link_libraries(${RELATIVE_DIR} Knight)
endforeach()