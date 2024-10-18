//#include <iostream>
//using namespace std;
//
//int main() {
//    cout << "Placeholder" << endl;
//}

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

void processNode(aiNode* node, const aiScene* scene) {
    // 遍历节点中的所有网格
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        // 处理当前的网格
        std::cout << "Processing mesh: " << i << std::endl;

        // 打印顶点数据
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3D vertex = mesh->mVertices[v];
            std::cout << "Vertex " << v << ": ("
                      << vertex.x << ", "
                      << vertex.y << ", "
                      << vertex.z << ")" << std::endl;
        }
    }

    // 递归处理子节点
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

int main() {
    Assimp::Importer importerx;
    const aiScene *scene = importerx.ReadFile("F:/.3DSpringFestivalGala/Ryan/fullYesIDK2.fbx",
                                              aiProcess_Triangulate |
                                              aiProcess_FlipUVs |
                                              aiProcess_CalcTangentSpace |
                                              aiProcess_GenNormals
                                              );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error::Assimp: " << importerx.GetErrorString() << std::endl;
        return -1;
    }

    processNode(scene->mRootNode, scene);
    return 0;
}