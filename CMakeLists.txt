cmake_minimum_required(VERSION 3.0)

project(LearnRender)

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION_EXTRA "")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(PROJECT_VERSION_FULL "${PROJECT_VERSION}.${PROJECT_VERSION_PATCH}${PROJECT_VERSION_EXTRA}")

set(WIN32 True)

# set source group recursive
# > https://www.cnblogs.com/zjutzz/p/7284114.html
# > https://stackoverflow.com/questions/31422680/how-to-set-visual-studio-filters-for-nested-sub-directory-using-cmake
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

function(my_add_executable _exe)
    foreach(_source IN ITEMS ${ARGN})
        assign_source_group("${_source}")
    endforeach()
    add_executable(${ARGV})
endfunction(my_add_executable)

function(my_get_c_files _ret_var)
    set(_ret "")
    foreach(_dir ${ARGN})
        file(GLOB_RECURSE _files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${_dir}/*.h" "${_dir}/*.hpp" "${_dir}/*.c" "${_dir}/*.cpp")
        # message(STATUS "${_dir} = ${_files}")
        set(_ret ${_ret} ${_files})
    endforeach()
    set(${_ret_var} ${_ret} PARENT_SCOPE)
endfunction(my_get_c_files)

# cmake 不推荐使用file，推荐把文件路径直接写在CMakeLists里。这样文件增减，可以自动构建工程，然而这样很不方便。
# AUX_SOURCE_DIRECTORY(src v_SRC)
my_get_c_files(v_source src include)
my_add_executable(demo ${v_source})


INCLUDE_DIRECTORIES(include)

find_library(v_glfw3 glfw3 lib)
if(NOT v_glfw3)
    message(FATAL_ERROR "glfw3 not found")
endif()

TARGET_LINK_LIBRARIES(demo ${v_glfw3} opengl32)