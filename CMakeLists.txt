cmake_minimum_required(VERSION 3.0.0)
project(OPGL VERSION 0.0.1)
set(SHADERS 
    #${CMAKE_SOURCE_DIR}/outlining.frag ${CMAKE_SOURCE_DIR}/outlining.vert 
    #${CMAKE_SOURCE_DIR}/normals.geom ${CMAKE_SOURCE_DIR}/normals.frag
    #${CMAKE_SOURCE_DIR}/light.vert ${CMAKE_SOURCE_DIR}/light.frag 
    ${CMAKE_SOURCE_DIR}/framebuffer.vert ${CMAKE_SOURCE_DIR}/framebuffer.frag 
    ${CMAKE_SOURCE_DIR}/default.vert ${CMAKE_SOURCE_DIR}/default.frag 
    ${CMAKE_SOURCE_DIR}/default.geom)
set(H_DIR  ${CMAKE_SOURCE_DIR}/Libraries/include)
set(LIBRARY_FOLDER ${CMAKE_SOURCE_DIR}/Libraries/lib)
# set(GLAD_FOLDER ${CMAKE_SOURCE_DIR}/Libraries/include/glad/glad.c)
link_directories(${LIBRARY_FOLDER})
file(GLOB SOURCES "*.cpp" "*.h" "*.c")
add_executable(OPGL ${SOURCES}) 
# stb.cpp Texture.cpp Texture.h VAO.cpp VAO.h VBO.cpp VBO.h glad.c)

# Set the directory containing your shader files
# set(FRAGM_SHAD_DIR ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_SOURCE_DIR}/fragments)

# Create a custom target to copy shader files to the build directory
add_custom_target(copy_files)
foreach(file ${SHADERS})
  file(COPY ${file} DESTINATION ${PROJECT_BINARY_DIR})
  add_custom_command(
      TARGET copy_files POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
              ${file} ${PROJECT_BINARY_DIR}
              # ${CMAKE_CURRENT_BINARY_DIR}/${file}
              # ${destination_dir}/${file}
  )
endforeach()
# add_custom_target(ShaderFiles ALL
#     COMMAND ${CMAKE_COMMAND} -E copy ${SHADERS}
#     ${PROJECT_SOURCE_DIR}/${SHADERS}
#     ${PROJECT_BINARY_DIR}/${SHADERS}
#     COMMENT "Copying shader"
# )
add_custom_target(ModelFiles ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/models
    ${PROJECT_SOURCE_DIR}/models
    ${PROJECT_BINARY_DIR}/models
    COMMENT "Copying model"
)
add_custom_target(Textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/textures
    ${PROJECT_SOURCE_DIR}/textures
    ${PROJECT_BINARY_DIR}/textures
    COMMENT "Copying texture"
)
add_dependencies(OPGL ModelFiles Textures)
target_link_libraries(OPGL PRIVATE -lglfw3 -lglew32s -lopengl32 -lgdi32)
target_include_directories(OPGL PRIVATE ${H_DIR})