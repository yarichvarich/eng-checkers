cmake_minimum_required(VERSION 3.22)

project(eng-checkers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB checkers_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp)
file(GLOB lib_SRC ${PROJECT_SOURCE_DIR}/lib/*.lib)
file(GLOB dll_SRC ${PROJECT_SOURCE_DIR}/bin)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}include/SFML/*.hpp ${PROJECT_SOURCE_DIR}include/SFML/*.h)

add_executable(${PROJECT_NAME} ${checkers_SRC} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} ${lib_SRC} ${OPENGL_LIBRARIES})

add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/req"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)