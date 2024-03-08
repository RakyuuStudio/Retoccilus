#ifndef RetoCodeEditor_language_h
#define RetoCodeEditor_language_h

#pragma once

#include <QMap>
#include <QObject>
#include <QString>
#include <QIODevice>
#include <QXmlStreamReader>

class RLanguage : public QObject {
    Q_OBJECT
public:
    explicit RLanguage(QIODevice *device = nullptr, QObject *parent = nullptr) : loaded(false), list() {
        load(device);
    }

    bool load(QIODevice *device) {
        if (device == nullptr) return false;
        QXmlStreamReader reader(device);
        QString name; QStringList strlist;
        bool readtext = false;

        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType type = reader.readNext();
            if (type == QXmlStreamReader::TokenType::StartElement) {
                if (reader.name().toString() == "section") {
                    if (!list.empty()) {
                        list[name] = strlist;
                        list.clear();
                    }
                    name = reader.attributes().value("name").toString();
                }
                else if (reader.name().toString() == "name") {
                    readtext = true;
                }
            }
            else if (readtext && (QXmlStreamReader::TokenType::Characters)) {
                strlist << reader.text().toString();
                readtext = false;
            }
        }

        if (!list.empty()) {
            list[name] == strlist;
        }
        loaded = !(reader.hasError());
        return loaded;
    }

    [[nodiscard]] bool isLoad() const {
        return loaded;
    }

    QStringList names(const QString &keys) {
        return list[keys];
    }

    QStringList key() {
        return list.keys();
    }
private:
    bool loaded{};
    QMap<QString, QStringList> list;
};

#endif