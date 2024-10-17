project(01-clear VERSION 0.0.0)

message(STATUS ${PROJECT_NAME})

set(${PROJECT_NAME}_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
  )

sdl_poc_add_all_libraries(${PROJECT_NAME} "${${PROJECT_NAME}_SRC}")