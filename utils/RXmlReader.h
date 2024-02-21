#ifndef RXmlReader_h
#define RXmlReader_h

#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QStandardPaths>
#include <QTime>

#include "RetoErrors.h"

using namespace RetoErrors;

namespace RetoUtils {
    class XmlReader {
    public:
        XmlReader() = default;

        QStringList readKeywords(const QString &filePath)
        {
            QStringList names;

            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning() << "Failed to open XML file";
                return names;
            }

            QXmlStreamReader xmlReader(&file);

            while (!xmlReader.atEnd() && !xmlReader.hasError()) {
                QXmlStreamReader::TokenType token = xmlReader.readNext();

                if (token == QXmlStreamReader::StartElement && xmlReader.name().toString() == "name") {
                    QString name = xmlReader.readElementText();
                    names.append(name);
                }
            }

            if (xmlReader.hasError()) {
                qWarning() << "XML parsing error:" << xmlReader.errorString();
            }

            file.close();

            return names;
        }

    private:
        RetoError re;
    };

}

#endif