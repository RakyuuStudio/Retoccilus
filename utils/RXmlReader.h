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
        XmlReader() {}
        QStringList readKeyword(const QString &filePath) {
            QStringList  names;
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                re.generateRuntimeError("Failed to open file");
            }

            QXmlStreamReader xml(&file);
            while (!xml.atEnd() && !xml.hasError()) {
                QXmlStreamReader::TokenType token = xml.readNext();
                if (token == QXmlStreamReader::StartDocument) {
                    continue;
                }
                if (token == QXmlStreamReader::StartElement) {
                    if (xml.name().toString() ==  "name") {
                        QString value = xml.readElementText();
                        if (!value.isEmpty()) {
                            names.append(value);
                        }
                    }
                    else {
                        xml.skipCurrentElement();
                    }
                }
            }

            return names;
        }
    private:
        RetoError re;
    };

}

#endif