add_library(rcluc rcluc.c rmwu_micrortps.c)
target_include_directories(rcluc PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> )
target_link_libraries(rcluc micrortps_client)
target_link_libraries(rcluc microcdr)
