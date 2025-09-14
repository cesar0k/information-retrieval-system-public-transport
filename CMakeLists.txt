cmake_minimum_required(VERSION 3.10)
project(Kursach)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Kursach main.cpp)

target_link_libraries(Kursach PRIVATE sfml-graphics)

if(MSVC)
    target_compile_options(Kursach PRIVATE /utf-8)
endif()

add_custom_command(TARGET Kursach POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Arial.ttf"
        "${CMAKE_CURRENT_SOURCE_DIR}/Avtiki.jpg"
        "${CMAKE_CURRENT_SOURCE_DIR}/transport.txt"
        "$<TARGET_FILE_DIR:Kursach>"
    COMMENT "Copying resources to build directory"
    VERBATIM
)

if(APPLE)
    set_target_properties(Kursach PROPERTIES MACOSX_BUNDLE ON)
endif()
