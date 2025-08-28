file(GLOB KNIGHT_SOURCE "*.cpp" "*.h")

add_library(Knight STATIC ${KNIGHT_SOURCE})
target_include_directories(Knight PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Knight raylib)