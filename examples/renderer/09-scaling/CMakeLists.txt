project(09-scaling VERSION 0.0.0)

message(STATUS ${PROJECT_NAME})

set(${PROJECT_NAME}_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
  )

sdl_poc_add_all_libraries(${PROJECT_NAME} "${${PROJECT_NAME}_SRC}")


set(SAMPLE_BMP ${CMAKE_CURRENT_SOURCE_DIR}/sample.bmp)
set(SAMPLE_BMP_DEST ${CMAKE_CURRENT_BINARY_DIR}/sample.bmp)

add_custom_command(
  OUTPUT ${SAMPLE_BMP_DEST}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SAMPLE_BMP} ${SAMPLE_BMP_DEST}
  DEPENDS ${SAMPLE_BMP}
  COMMENT "Copying sample.bmp to build directory"
)

add_custom_target(copy_sample_bmp ALL
  DEPENDS ${SAMPLE_BMP_DEST}
)

add_dependencies(${PROJECT_NAME} copy_sample_bmp)