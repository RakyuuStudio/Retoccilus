#include "../include/fbx-importer.h"
#include "../utils/Types/Mesh.h"
#include <fbxsdk.h>
#include <exception>
#include <stdexcept>

namespace Retoccilus::Engine::Support3D::Importer {
    void RtFbxImporter::importImplementation(const std::string &path) {
        FbxManager* fbxManager = FbxManager::Create();
        FbxIOSettings *fbxIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
        fbxManager->SetIOSettings(fbxIOSettings);

        FbxScene *fbxScene = FbxScene::Create(fbxManager, "Scene");
        FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "");

        bool status = fbxImporter->Initialize(path.c_str(), -1,
                                              fbxManager->GetIOSettings());
        if (!status) {
            throw std::runtime_error("Failed to initialize FBX importer");
//            return;
        }

        status = fbxImporter->Import(fbxScene);
        if (!status) {
            throw std::runtime_error("Failed to import FBX file from: " + path);
//            return;
        }

        // Get First Mesh
        FbxNode *rootNode = fbxScene->GetRootNode();
        if (rootNode && rootNode->GetChildCount() > 0) {
            FbxMesh *mesh = rootNode->GetChild(0)->GetMesh();
            if (mesh) {
                processMesh(*mesh);
            }
        }

        fbxImporter->Destroy();
        fbxManager->Destroy();
    }

    void RtFbxImporter::processMesh(FbxMesh &mesh) {
        Utility::VulkanDevice device;
        Utility::RtMesh rtMesh(device);

        // Vertex Data
        int numVertices = mesh.GetControlPointsCount();
        for (int i = 0; i < numVertices; i++) {
            FbxVector4 position = mesh.GetControlPointAt(i);
            FbxVector4 normal = mesh.GetElementNormal(0)->GetDirectArray().GetAt(1);
            FbxVector2 uv = mesh.GetElementUV(0)->GetDirectArray().GetAt(1);

            Utility::RtVertex vertex = {
                    QVector3D(position[0], position[1], position[2]),
                    QVector2D(uv[0], uv[1]),
                    QVector3D(normal[0], normal[1], normal[2])
            };

            rtMesh.addVertex(vertex);
        }

        // Index Data
        int numPolygons = mesh.GetPolygonCount();
        for (int i = 0; i < numPolygons; i++) {
            for (int j = 0; j < mesh.GetPolygonSize(i); j++) {
                rtMesh.addIndex(mesh.GetPolygonVertex(i, j));
            }
        }

        // Upload Data to Vulkan
        rtMesh.UploadVertexDataToVulkan();
        rtMesh.UploadIndexDataToVulkan();
    }
}