#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include <QTextCharFormat>
#include <QtXml>

class RSyntaxStyle : public QObject {
    Q_OBJECT
public:
    explicit RSyntaxStyle(QObject *parent = nullptr) : QObject(parent), m_name(), m_data(), m_loaded(false) {

    }

    bool load(const QString &fl) {
        QXmlStreamReader reader(fl);
        while (!reader.atEnd() && !reader.hasError()) {
            auto token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name().toString() == "style-scheme") {
                    if (reader.attributes().hasAttribute("name")) {
                        m_name = reader.attributes().value("name").toString();
                    }
                } else if (reader.name().toString() == "style") {
                    auto attributes = reader.attributes();
                    auto name = attributes.value("name");
                    QTextCharFormat format;
                    if (attributes.hasAttribute("background")) {
                        format.setBackground(QColor(attributes.value("background").toString()));
                    }
                    if (attributes.hasAttribute("foreground")) {
                        format.setForeground(QColor(attributes.value("foreground").toString()));
                    }
                    if (attributes.hasAttribute("bold") &&
                        attributes.value("bold").toString() == "true") {
                        format.setFontWeight(QFont::Weight::Bold);
                    }
                    if (attributes.hasAttribute("italic") &&
                        attributes.value("italic").toString() == "true") {
                        format.setFontItalic(true);
                    }
                    if (attributes.hasAttribute("underlineStyle")) {
                        auto underline = attributes.value("underlineStyle");
                        auto s = QTextCharFormat::UnderlineStyle::NoUnderline;
                        if (underline.toString() == "Single") {
                            s = QTextCharFormat::UnderlineStyle::SingleUnderline;
                        } else if (underline.toString() == "Dash") {
                            s = QTextCharFormat::UnderlineStyle::DashUnderline;
                        } else if (underline.toString() == "SingleDot") {
                            s = QTextCharFormat::UnderlineStyle::DotLine;
                        } else if (underline.toString() == "DashDot") {
                            s = QTextCharFormat::DashDotLine;
                        } else if (underline.toString() == "DashDotDot") {
                            s = QTextCharFormat::DashDotDotLine;
                        } else if (underline.toString() == "SingleWave") {
                            s = QTextCharFormat::WaveUnderline;
                        } else if (underline.toString() == "SCUnderline") {
                            s = QTextCharFormat::SpellCheckUnderline;
                        } else {
                            qDebug() << "Unknown underline value " << underline;
                        }
                        format.setUnderlineStyle(s);
                    }
                    m_data[name.toString()] = format;
                }
            }
        }
        m_loaded = !reader.hasError();
        return m_loaded;
    }

    [[nodiscard]] QString name() const { return m_name; }

    [[nodiscard]] bool isLoaded() const { return m_loaded; }

    [[nodiscard]] QTextCharFormat getFormat(QString name) const {
        auto result = m_data.find(name);
        if (result == m_data.end()) {
            return {};
        }
        return result.value();
    }

    static RSyntaxStyle *defaultStyle() {
        static RSyntaxStyle style;
        if (!style.isLoaded()) {
            QFile fl(":/config/Configuration/RCodeEditor/ColorThemes/Dracula.xml");
            if (!fl.open(QIODevice::ReadOnly)) {
                return &style;
            }
            if (!style.load(fl.readAll())) {
                qDebug() << "Scheme Load Failed";
            }
        }
        return &style;
    }

private:
    QString m_name;
    QMap<QString, QTextCharFormat> m_data;
    bool m_loaded;
};

