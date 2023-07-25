cmake_minimum_required(VERSION 3.0.0)
project(OPGL VERSION 0.0.1)
set(SHADERS 
    ${CMAKE_SOURCE_DIR}/framebuffer.vert ${CMAKE_SOURCE_DIR}/framebuffer.frag 
    ${CMAKE_SOURCE_DIR}/default.vert ${CMAKE_SOURCE_DIR}/default.frag 
    ${CMAKE_SOURCE_DIR}/default.geom)
set(H_DIR  ${CMAKE_SOURCE_DIR}/Libraries/include)
set(LIBRARY_FOLDER ${CMAKE_SOURCE_DIR}/Libraries/lib)
link_directories(${LIBRARY_FOLDER})
file(GLOB SOURCES "*.cpp" "*.h" "*.c")
add_executable(OPGL ${SOURCES}) 

# Create a custom target to copy shader files to the build directory
add_custom_target(copy_files)
foreach(file ${SHADERS})
  file(COPY ${file} DESTINATION ${PROJECT_BINARY_DIR})
  add_custom_command(
      TARGET copy_files POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
              ${file} ${PROJECT_BINARY_DIR}
  )
endforeach()
add_custom_target(Textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/textures
    ${PROJECT_SOURCE_DIR}/textures
    ${PROJECT_BINARY_DIR}/textures
    COMMENT "Copying texture"
)
add_dependencies(OPGL Textures)
target_link_libraries(OPGL PRIVATE -lglfw3 -lglew32s -lopengl32 -lgdi32)
target_include_directories(OPGL PRIVATE ${H_DIR})