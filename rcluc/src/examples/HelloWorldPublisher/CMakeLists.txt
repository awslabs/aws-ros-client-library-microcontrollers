add_executable(HelloWorldPublisher main.c)
target_include_directories(HelloWorldPublisher PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/examples/HelloWorldMessage> )
target_link_libraries(HelloWorldPublisher rcluc HelloWorldMessage "-static")
