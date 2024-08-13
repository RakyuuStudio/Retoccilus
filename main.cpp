#include "Source/Engine/3D-Support/Model-Importer/include/fbx-importer.h"
using namespace Retoccilus::Engine::Support3D::Importer;

int main(int argc, char **argv) {
    RtFbxImporter importer;
    importer.importModel(R"(F:\.3DSpringFestivalGala\Ryan\FullWithTextureAndNoRig.fbx)");
}