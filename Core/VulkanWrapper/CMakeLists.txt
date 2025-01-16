add_library(VulkanWrapper STATIC
    VulkanWrapper.cpp
    VulkanWrapper.h
    stb_image_impl.cpp
)

target_link_libraries(VulkanWrapper PUBLIC Vulkan::Vulkan glfw glm)

target_include_directories(VulkanWrapper 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/stb
)

# 编译着色器
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

function(compile_shader SHADER_NAME)
    set(SHADER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER_NAME})
    set(SHADER_SPV ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER_NAME}.spv)
    
    add_custom_command(
        OUTPUT ${SHADER_SPV}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders/
        COMMAND ${GLSL_VALIDATOR} -V ${SHADER_SOURCE} -o ${SHADER_SPV}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compiling shader ${SHADER_NAME}"
    )
    list(APPEND SHADER_BINARY_FILES ${SHADER_SPV})
endfunction()

compile_shader(sprite.vert)
compile_shader(sprite.frag)

add_custom_target(shaders 
    DEPENDS 
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/sprite.vert.spv
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/sprite.frag.spv
)

add_dependencies(VulkanWrapper shaders)