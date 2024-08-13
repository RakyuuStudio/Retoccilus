add_library(Model-Importer STATIC
        include/fbx-importer.h
        src/fbx-importer.cpp
        utils/Types/Mesh.h
        utils/Types/Vertex.h
        utils/Types/Mesh.cpp
        include/model-importer-base.h
)

target_include_directories(Model-Importer PRIVATE
        "include/"
        "../../../../thirdparty_lib/"
)

target_link_libraries(Model-Importer PRIVATE Qt6::Widgets)