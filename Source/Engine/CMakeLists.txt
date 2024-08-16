set(LIBRARY_NAME RetoGameEngine)

#find_package(Qt6Core CONFIG REQUIRED)
#find_package(Qt6Widgets CONFIG REQUIRED)
#find_package(Qt6Gui CONFIG REQUIRED)
#
add_library(${LIBRARY_NAME} STATIC
        # Scene
        3D-Support/Utility/Scene/RtScene.h
        3D-Support/Utility/Scene/RtScene.cpp
        3D-Support/Utility/Scene/RtNode.h

        # Utility - Math
        3D-Support/Utility/Math/Matrix4x4.h
        3D-Support/Utility/Math/Vector2.h
        3D-Support/Utility/Math/Vector2.inl
        3D-Support/Utility/Math/Quaternion.h
        3D-Support/Utility/Math/Matrix3x3.h
        3D-Support/Utility/Math/Matrix3x3.inl
        3D-Support/Utility/Math/Vector3.h
        3D-Support/Utility/Math/Vector3.inl
        3D-Support/Utility/Math/Quaternion.inl

        # Utility - [Global]
        3D-Support/Utility/Definition.h
)

target_link_libraries(
        ${LIBRARY_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
)