cmake_minimum_required(VERSION 3.25.0)
project(GD VERSION 2024.10.0 DESCRIPTION "sdl3-imgui-poc" LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##########################################################
#--------------------------------------------------------
# Import dependencies with CPM
#--------------------------------------------------------
##########################################################
include(cmake/CPM.cmake)
#ImGUI
CPMAddPackage(
  NAME imgui
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG master  
  DOWNLOAD_ONLY TRUE
)
if(NOT imgui_ADDED)
  message(FATAL_ERROR "imgui target not found!")
endif()

# SDL3
CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG release-3.2.0
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
if(NOT TARGET SDL3::SDL3)
  message(FATAL_ERROR "SDL3 target not found!")
endif()

function(sdl_poc_add_all_libraries APP_NAME APP_SRC)
    add_executable(${APP_NAME} ${APP_SRC})
    target_compile_options(${APP_NAME} PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wshadow
        -Wno-unused
    )

    target_include_directories(${APP_NAME}
      PUBLIC
         $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>

    )
    # Link SDL
    target_link_libraries(${APP_NAME} PUBLIC SDL3::SDL3)
    # Link IMGUI 
      set(imgui_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
      )

      target_include_directories(${APP_NAME} PUBLIC ${imgui_SOURCE_DIR})
      target_include_directories(${APP_NAME} PUBLIC ${imgui_SOURCE_DIR}/backends)
      target_sources(${APP_NAME} PRIVATE ${imgui_SOURCES})

endfunction()


add_subdirectory(examples/renderer/01-clear)
add_subdirectory(examples/renderer/03-lines)
add_subdirectory(examples/renderer/09-scaling)