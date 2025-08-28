get_filename_component(PROJ_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJ_NAME} CXX)

file(GLOB SOURCES *.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_compile_definitions(${PROJ_NAME} PUBLIC RESOURCES_DIR="${CMAKE_SOURCE_DIR}/resources")
target_include_directories(${PROJ_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJ_NAME} PUBLIC
    raylib
)