add_library(HelloWorldMessage HelloWorld.c HelloWorldWriter.c)
target_include_directories(HelloWorldMessage  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
