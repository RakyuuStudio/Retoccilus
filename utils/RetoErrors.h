#ifndef RetoErrors_h
#define RetoErrors_h

#include <QFile>
#include <QDateTime>
#include <QStandardPaths>

namespace RetoErrors {
    class RetoError {
    public:
        void generateRuntimeError(const QString &error) {
            QString errorMsg = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                               "/RakyuuStudio/Retoccilus/RuntimeErrors/RetoRuntimeError.retore";
            QFile errorFile(errorMsg);
            if (errorFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                QString errorContent = QDateTime::currentDateTime().toString(Qt::ISODateWithMs) + " "+ error + "\n";
                errorFile.write(errorContent.toUtf8());
                errorFile.close();
            }
            else {
                qWarning() << "Failed to write Error File";
            }
        }
    };
};

#endif //RetoErrors_h