add_library(rcluc rcluc.c rmwu_micrortps.c)
target_include_directories(rcluc PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> )
