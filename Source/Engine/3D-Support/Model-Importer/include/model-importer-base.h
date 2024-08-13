#ifndef MODEL_IMPORTER_BASE_H
#define MODEL_IMPORTER_BASE_H

#include <vulkan/vulkan.h>
#include <QString>
#include <string>

namespace Retoccilus::Engine::Support3D::Importer {
    template<typename T>
    class ModelImporterBase {
    public:
        virtual ~ModelImporterBase() = default;

        virtual void importModel(const QString &path) {
            importImplementation(QStringToStdString(path));
        }

        virtual void importModel(const std::string &path) {
            importImplementation(path);
        }

        virtual void importModel(const char *path) {
            importImplementation(std::string(path));
        }

//        virtual void destroyData() = 0;
    protected:
        virtual void importImplementation(const std::string &path) = 0;
    private:
        static std::string QStringToStdString(const QString &qstr) {
            return qstr.toStdString();
        }
    };
}

#endif // MODEL_IMPORTER_BASE_H