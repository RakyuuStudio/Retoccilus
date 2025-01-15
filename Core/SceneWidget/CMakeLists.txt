# Add Rt2DSceneWidget
add_library(Rt2DSceneWidget
    Rt2DSceneWidget/Rt2DSceneWidget.cpp
    Rt2DSceneWidget/Rt2DSceneWidget.h
)

target_include_directories(Rt2DSceneWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Rt2DSceneWidget PRIVATE VulkanWrapper glm)
