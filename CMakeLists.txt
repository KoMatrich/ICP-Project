# CMakeLists.txt
# author: Vitezslav Kriz (xkrizv03) & Martin Kocich (xkocic02)

cmake_minimum_required (VERSION 3.16)

project(ICP CXX)

set(CXX_STANDARD 23)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g")
endif()

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
# Include Qt deployment helper function
# include(windeployqt.cmake)

file(GLOB_RECURSE vsrc src/**.cpp)
file(GLOB_RECURSE vrec resources.qrc resources/**)
file(GLOB_RECURSE vhead include/**.h)

add_executable ("${PROJECT_NAME}"  "${vsrc}" "${vrec}" "${vhead}")

target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_precompile_headers("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
target_link_libraries("${PROJECT_NAME}" Qt5::Widgets)

#Edit and Continue for CMake projects
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# windeployqt("${PROJECT_NAME}")

# Doxygen support
find_package(Doxygen OPTIONAL_COMPONENTS)
if(DOXYGEN_FOUND)
	doxygen_add_docs(
	    doxygen
	    ${PROJECT_SOURCE_DIR}/doc
	    COMMENT "Generate man pages"
	)
    message("-- Doxygen done.")
endif()
