#ifndef RXmlReader_h
#define RXmlReader_h

#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QStandardPaths>
#include <QTime>

#include <unordered_set>

#include "RetoErrors.h"

using namespace Retoccilus;
using namespace std;

namespace Retoccilus {
    class XmlReader {
    public:
        XmlReader() = default;

        QStringList readKeywords(const QString &filePath) {
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

    class stdCpp {
    public:
        static unordered_set<string> returnKeyWord() {
            unique_ptr<XmlReader> xrPtr(new XmlReader);
            QStringList keywordQs = xrPtr->readKeywords(":/config/Configuration/RCodeEditor/KeywordList/cppKeywords.xml");
            unordered_set<string> nm;
            for (const auto& keyword : keywordQs) {
                nm.insert(keyword.toStdString());
            }
            return nm;
        }
    };
}

#endif