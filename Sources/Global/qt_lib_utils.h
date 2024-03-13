#ifndef Retoccilus_Qt_LibraryExtent_Utility_h
#define Retoccilus_Qt_LibraryExtent_Utility_h

#include <type_traits>
#include <functional>
#include <utility>
#include <memory>
#include <chrono>

#include <QString>
#include <QRect>
#include <QThread>
#include <QProcessEnvironment>
#include <QStringList>
#include <QByteArray>

#include <QFile>
#include <QProcess>
#include <QTextCodec>
#include <QtGlobal>
#include <QDebug>
#include <QStyleFactory>
#include <QDateTime>
#include <QColor>
#include <QWindow>
#include <QScreen>
#include <QDirIterator>
#include <QTextEdit>
#include <QGuiApplication>

#include "Global/charset_info.h"

#define ENCODE_AUTO "AUTO"

#define ENCODE_TYPE_UTF8 "UTF-8"
#define ENCODE_TYPE_UTF16 "UTF-16"
#define ENCODE_TYPE_UTF32 "UTF-32"
#define ENCODE_TYPE_UTF8_BOM "UTF-8 BOM"
#define ENCODE_TYPE_UTF16_BOM "UTF-16 BOM"
#define ENCODE_TYPE_UTF32_BOM "UTF-32 BOM"
#define ENCODE_ASCII "ASCII"

#define ENCODE_DEFAULT "SYSTEM"

#define ENCODE_ALLPROJECT "PROJECT"

using namespace std;

namespace Retoccilus {
    using LineProcessFunc = std::function<void(const QString &)>;
    enum class NewLineType {
        Unix_LF,
        Windows_CRLF,
        Mac_CR
    };

    class BaseException {
    public:
        explicit BaseException(QString message) : pMessage(std::move(message)) {}

        [[nodiscard]] QString error() const {
            return pMessage;
        }

    protected:
        QString pMessage;
    };

    class ContainerException_IndexOutOfRange : public BaseException {
    public:
        explicit ContainerException_IndexOutOfRange(int index) : BaseException(
                QObject::tr("Index in Container %1 out of range").arg(index)) {}
    };

    class FatalError_FileError : public BaseException {
    public:
        explicit FatalError_FileError(const QString &message) : BaseException(message) {}
    };

    /*
     * WHAT THE FUCK IS THIS:
     * 1: We create 2 variables: isAllAscii and ii;
     *    isAllAscii is used to check if the text is all ASCII, ii is used to iterate through the text.
     *    We also create a QByteArray s to store the text.
     *    We also create a variable size to store the size of the text.
     * 2: Check the previous 3 bytes of the text to see if it is a UTF-8 BOM (Byte Order Mark) -> 0xEF 0xBB 0xBF
     *    If yes, we return ENCODE_TYPE_UTF8_BOM.
     *    If no, we continue.
     * 3: Enter a loop, iterate through the text.
     *    > If the character is less than 0x80(in ASCII, smaller than 128), it is an ASCII Character, we continue.
     *    > If the character is less than 0xC0(in ASCII, smaller than 192) and greater than 0x80(in ASCII, greater than 128),
     *      we return the default code style.
     *    > If the character is less than 0xE0(in ASCII, smaller than 224) and greater than 0xC0(in ASCII, greater than 192),
     *      it might be a UTF-8 Character, then we check next.
     *    > If the character is less than 0xF0(in ASCII, smaller than 240) and greater than 0xE0(in ASCII, greater than 224),
     *      it might be a UTF-8 Character, then we check next.
     *  4: If all the character is ASCII Character, we return the type of ASCII.
     *     Else, we return the type of UTF-8.
     */
    QByteArray predictEncode(const QByteArray &text) {
        bool isAllAscii = false;
        int ii, size;
        const QByteArray &s = text;
        size = s.size();

        if (size >= 3 &&
            static_cast<unsigned char>(s[0]) == 0xEF &&
            static_cast<unsigned char>(s[1]) == 0xBB &&
            static_cast<unsigned char>(s[2]) == 0xBF) {
            return ENCODE_TYPE_UTF8_BOM;
        }

        isAllAscii = true;
        ii = 0;

        while (ii < size) {
            unsigned char ch = s.at(ii);

            if (ch < 0x80) ii++;
            else if (ch < 0xC0) return ENCODE_DEFAULT;
            else if (ch < 0xE0) {
                if (ii >= size - 1) return ENCODE_DEFAULT;
                if ((s.at(ii + 1) & 0xC0) != 0x80) return ENCODE_DEFAULT;
                isAllAscii = false;
                ii += 2;
            } else if (ch < 0xF0) {
                if (ii >= size - 2) return ENCODE_DEFAULT;
                if (((s.at(ii + 1) & 0xC0) != 0x80) || ((s.at(ii + 2) & 0xC0) != 0x80)) return ENCODE_DEFAULT;
                isAllAscii = false;
                ii += 3;
            } else {
                return ENCODE_DEFAULT;
            }
        }

        if (isAllAscii) return ENCODE_ASCII;
        return ENCODE_TYPE_UTF8;
    }

    bool isTextAllAscii(const QByteArray &text) {
        for (QChar c: text) {
            if (c.unicode() > 127) {
                return false;
            }
        }
        return true;
    }

    QStringList readStreamToLines(QTextStream *stream) {
        QStringList list;
        QString s;
        while (stream->readLineInto(&s)) {
            list.append(s);
        }
        return list;
    }

    void readStreamToLines(QTextStream *stream,
                           LineProcessFunc lineFunc) {
        QString s;
        while (stream->readLineInto(&s)) {
            lineFunc(s);
        }
    }

    bool isTextAllAscii(const QString &text) {
        for (QChar c: text) {
            if (c.unicode() > 127) {
                return false;
            }
        }
        return true;
    }

    bool isNonPrintableAsciiChar(char ch) {
        return (ch <= 32) && (ch >= 0);
    }

    QStringList textToLines(const QString &text) {
        QTextStream stream(&((QString &) text), QIODevice::ReadOnly);
        return readStreamToLines(&stream);
    }


    void textToLines(const QString &text, LineProcessFunc lineFunc) {
        QTextStream stream(&((QString &) text), QIODevice::ReadOnly);
        readStreamToLines(&stream, lineFunc);
    }

    QString linesToText(const QStringList &lines, const QString &lineBreak) {
        return lines.join(lineBreak);
    }

    QList<QByteArray> splitByteArrayToLines(const QByteArray &content) {
        QList<QByteArray> lines;
        const char *p = content.constData();
        const char *end = p + content.length();
        const char *lineStart = p;
        QByteArray line;
        while (p <= end) {
            char ch = *p;
            switch (ch) {
                case '\r':
                    line = QByteArray(lineStart, p - lineStart);
                    lines.append(line);
                    p++;
                    if (*p == '\n')
                        p++;
                    lineStart = p;
                    break;
                case '\n':
                    line = QByteArray(lineStart, p - lineStart);
                    lines.append(line);
                    p++;
                    lineStart = p;
                    break;
                default:
                    p++;
            }
        }
        if (lineStart > end) {
            lines.append("");
        } else {
            line = QByteArray(lineStart, end - lineStart + 1);
            lines.append(line);
        }
        return lines;
    }

    QString trimRight(const QString &s) {
        if (s.isEmpty())
            return s;
        int i = s.length() - 1;
//   while ((i>=0) && ((s[i] == '\r') || (s[i]=='\n') || (s[i] == '\t') || (s[i]==' ')))  {
        while ((i >= 0) && ((s[i] == '\t') || (s[i] == ' '))) {
            i--;
        };
        if (i >= 0) {
            return s.left(i + 1);
        } else {
            return {};
        }
    }

    QString trimLeft(const QString &s) {
        if (s.isEmpty())
            return s;
        int i = 0;
        while ((i < s.length()) && ((s[i] == '\t') || (s[i] == ' '))) {
            i++;
        };
        if (i < s.length()) {
            return s.mid(i);
        } else {
            return {};
        }
    }

    int countLeadingWhitespaceChars(const QString &line) {
        int n = 0;
        while (n < line.length()) {
            if (line[n].unicode() > 32)
                break;
            n++;
        }
        return n;
    }

    bool stringIsBlank(const QString &s) {
        for (QChar ch: s) {
            if (ch != ' ' && ch != '\t')
                return false;
        }
        return true;
    }

    QByteArray toByteArray(const QString &s) {
        //return s.toLocal8Bit();
        return s.toUtf8();
    }

    QString fromByteArray(const QByteArray &s) {
        QTextCodec *codec = QTextCodec::codecForName(ENCODE_TYPE_UTF8);
        QTextCodec::ConverterState state;
        if (!codec)
            return QString(s);
        QString tmp = codec->toUnicode(s, s.length(), &state);
        if (state.invalidChars > 0)
            tmp = QString::fromLocal8Bit(s);
        return tmp;
    }

    QStringList readFileToLines(const QString &fileName, const QString &codecName) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QTextStream stream(&file);
            QByteArray data = file.readAll();
            QString text = QString::fromUtf8(data);
            return text.split("\n");
        }
        return QStringList();
    }

    void readFileToLines(const QString &fileName, const QString &codecName, LineProcessFunc lineFunc) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QTextStream stream(&file);
            QByteArray data = file.readAll();
            QString text = QString::fromUtf8(data);
            QStringList lines = text.split("\n");
            for (const QString &line : lines) {
                lineFunc(line);
            }
        }
    }

    static QStringList tryLoadFileByEncoding(QByteArray encodingName, QFile &file, bool *isOk) {
        QStringList result;
        *isOk = false;
        QTextCodec *codec = QTextCodec::codecForName(encodingName);
        if (!codec)
            return result;
        file.reset();
        QTextCodec::ConverterState state;
        while (true) {
            if (file.atEnd()) {
                break;
            }
            QByteArray line = file.readLine();
            if (line.endsWith("\r\n")) {
                line.remove(line.length() - 2, 2);
            } else if (line.endsWith("\r")) {
                line.remove(line.length() - 1, 1);
            } else if (line.endsWith("\n")) {
                line.remove(line.length() - 1, 1);
            }
            QString newLine = codec->toUnicode(line.constData(), line.length(), &state);
            if (state.invalidChars > 0) {
                return QStringList();
            }
            result.append(newLine);
        }
        *isOk = true;
        return result;
    }

    QStringList readFileToLines(const QString &fileName) {
        QFile file(fileName);
        if (file.size() <= 0)
            return QStringList();
        QStringList result;
        if (file.open(QFile::ReadOnly)) {
            bool ok;
            result = tryLoadFileByEncoding("UTF-8", file, &ok);
            if (ok) {
                return result;
            }

            QByteArray realEncoding = pCharsetInfoManager->getDefaultSystemEncoding();
            result = tryLoadFileByEncoding(realEncoding, file, &ok);
            if (ok) {
                return result;
            }
            QList<PCharsetInfo> charsets = pCharsetInfoManager->findCharsetByLocale(pCharsetInfoManager->localeName());
            if (!charsets.isEmpty()) {
                QSet<QByteArray> encodingSet;
                for (int i = 0; i < charsets.size(); i++) {
                    encodingSet.insert(charsets[i]->name);
                }
                encodingSet.remove(realEncoding);
                        foreach (const QByteArray &encodingName, encodingSet) {
                        if (encodingName == ENCODE_TYPE_UTF8)
                            continue;
                        result = tryLoadFileByEncoding("UTF-8", file, &ok);
                        if (ok) {
                            return result;
                        }
                    }
            }
        }
        return result;
    }

    QByteArray readFileToByteArray(const QString &fileName) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            return file.readAll();
        }
        return QByteArray();
    }

    bool stringToFile(const QString &str, const QString &fileName) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return false;
        QTextStream stream(&file);
        stream << str;
        return true;
    }


    bool stringsToFile(const QStringList &list, const QString &fileName) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return false;
        QTextStream stream(&file);
        for (const QString &s: list) {
            stream << s
                   #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                   << Qt::endl;
#else
            <<endl;
#endif
        }
        return true;
    }

    bool fileExists(const QString &file) {
        if (file.isEmpty())
            return false;
        return QFile(file).exists();
    }

    bool fileExists(const QString &dir, const QString &fileName) {
        if (dir.isEmpty() || fileName.isEmpty())
            return false;
        QDir dirInfo(dir);
        return dirInfo.exists(fileName);
    }

    bool directoryExists(const QString &file) {
        if (file.isEmpty())
            return false;
        QFileInfo dir(file);
        return dir.exists() && dir.isDir();
    }

    bool removeFile(const QString &filename) {
        QFile file(filename);
        return file.remove();
    }

    bool copyFile(const QString &fromPath, const QString &toPath, bool overwrite) {
        QFile fromFile(fromPath);
        QFile toFile(toPath);
        if (!fromFile.exists())
            return false;
        if (toFile.exists()) {
            if (!overwrite)
                return false;
            if (!toFile.remove())
                return false;
        }

        if (!fromFile.open(QFile::ReadOnly))
            return false;
        if (!toFile.open(QFile::WriteOnly | QFile::Truncate))
            return false;

        constexpr int bufferSize = 64 * 1024;
        char buffer[bufferSize];

        while (!fromFile.atEnd()) {
            int readed = fromFile.read(buffer, bufferSize);
            toFile.write(buffer, readed);
        }
        toFile.close();
        fromFile.close();
        return true;
    }

    void copyFolder(const QString &fromDir, const QString &toDir) {
        QDirIterator it(fromDir);
        QDir dir(fromDir);
        QDir targetDir(toDir);
        const int absSourcePathLength = dir.absolutePath().length();


        if (targetDir.exists())
            return;
        targetDir.mkpath(targetDir.absolutePath());

        while (it.hasNext()) {
            it.next();
            const auto fileInfo = it.fileInfo();
            if (!fileInfo.isHidden() && !fileInfo.fileName().startsWith('.')) {
                const QString subPathStructure = fileInfo.absoluteFilePath().mid(absSourcePathLength);
                const QString constructedAbsolutePath = targetDir.absolutePath() + subPathStructure;
                if (fileInfo.isDir()) {
                    dir.mkpath(constructedAbsolutePath);
                    copyFolder(fileInfo.absoluteFilePath(), constructedAbsolutePath);
                } else if (fileInfo.isFile()) {
                    QFile::remove(constructedAbsolutePath);
                    QFile::copy(fileInfo.absoluteFilePath(), constructedAbsolutePath);
                    QFile newFile(constructedAbsolutePath);
                    QFile::Permissions permissions = newFile.permissions();
                    permissions |= (QFile::Permission::WriteOwner
                                    | QFile::Permission::WriteUser
                                    | QFile::Permission::WriteGroup
                                    | QFile::Permission::WriteOther);
                    newFile.setPermissions(permissions);
                }
            }
        }
    }

    QString includeTrailingPathDelimiter(const QString &path) {
        if (path.endsWith('/') || path.endsWith(QDir::separator())) {
            return path;
        } else {
            return path + "/";
        }
    }

    QString excludeTrailingPathDelimiter(const QString &path) {
        int pos = path.length() - 1;
        while (pos >= 0 && (path[pos] == '/' || path[pos] == QDir::separator()))
            pos--;
        return path.mid(0, pos + 1);
    }

    QString changeFileExt(const QString &filename, QString ext) {
        QFileInfo fileInfo(filename);
        QString suffix = fileInfo.suffix();
        QString name = fileInfo.fileName();
        QString path;
        if (!ext.isEmpty() && !ext.startsWith(".")) {
            ext = "." + ext;
        }
        if (fileInfo.path() != ".") {
            path = includeTrailingPathDelimiter(fileInfo.path());
        }
        if (suffix.isEmpty()) {
            return path + name + ext;
        } else {
            return path + fileInfo.completeBaseName() + ext;
        }
    }

    QString extractRelativePath(const QString &base, const QString &dest) {
        if (dest.isEmpty())
            return QString();

        QFileInfo baseInfo(base);
        QDir baseDir;
        if (baseInfo.isDir()) {
            baseDir = QDir(baseInfo.absoluteFilePath());
        } else {
            baseDir = baseInfo.absoluteDir();
        }
        return baseDir.relativeFilePath(dest);
    }

    QString localizePath(const QString &path) {
        QString result = path;
        result.replace("/", QDir::separator());
        return result;
    }

    QString extractFilePath(const QString &filePath) {
        QFileInfo info(filePath);
        return info.path();
    }

    QString extractFileName(const QString &fileName) {
        QFileInfo fileInfo(fileName);
        return fileInfo.fileName();
    }

    QString extractFileDir(const QString &fileName) {
        return extractFilePath(fileName);
    }

    QString extractAbsoluteFilePath(const QString &filePath) {
        QFileInfo info(filePath);
        return info.absoluteFilePath();
    }

    bool isReadOnly(const QString &filename) {
        return QFile(filename).isWritable();
    }

    int compareFileModifiedTime(const QString &filename1, const QString &filename2) {
        QFileInfo fileInfo1(filename1);
        QFileInfo fileInfo2(filename2);
        qint64 time1 = fileInfo1.lastModified().toMSecsSinceEpoch();
        qint64 time2 = fileInfo2.lastModified().toMSecsSinceEpoch();
        if (time1 > time2)
            return 1;
        if (time1 < time2)
            return -1;
        return 0;
    }

    void inflateRect(QRect & rect, int dx, int dy) {
        rect.setLeft(rect.left()-dx);
        rect.setRight(rect.right() + dx);
        rect.setTop(rect.top() - dy);
        rect.setBottom(rect.bottom() + dy);
    }

    void inflateRect(QRect & rect, int delta) {
        inflateRect(rect, delta, delta);
    }

    static int defaultScreenDPI = -1;
    int screenDPI() {
        if (defaultScreenDPI < 1) {
            defaultScreenDPI = qApp->primaryScreen()->logicalDotsPerInch();
        }
        return defaultScreenDPI;
    }

    void setScreenDPI(int dpi) {
        defaultScreenDPI = dpi;
    }

    float pointToPixel(float point, float dpi) {
        return point * dpi / 72;
    }

    float pointToPixel(float point) {
        return pointToPixel(point, static_cast<float>(screenDPI()));
    }

    float pixelToPoint(float pixel) {
        return pixel * 72 / static_cast<float>(screenDPI());
    }


    void decodeKey(const int combinedKey, int &key, Qt::KeyboardModifiers &modifiers) {
        modifiers = Qt::NoModifier;
        if (combinedKey & Qt::ShiftModifier) {
            modifiers |= Qt::ShiftModifier;
        }
        if (combinedKey & Qt::ControlModifier) {
            modifiers |= Qt::ControlModifier;
        }
        if (combinedKey & Qt::AltModifier) {
            modifiers |= Qt::AltModifier;
        }
        if (combinedKey & Qt::MetaModifier) {
            modifiers |= Qt::MetaModifier;
        }
        if (combinedKey & Qt::KeypadModifier) {
            modifiers |= Qt::KeypadModifier;
        }
        key = combinedKey & ~(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier | Qt::KeypadModifier);
    }

    bool isInFolder(const QString &folderpath, const QString &filepath) {
        QDir folder(folderpath);
        QFileInfo fileInfo(filepath);
        return fileInfo.absoluteFilePath().startsWith(includeTrailingPathDelimiter(folder.absolutePath()));
    }

    void createFile(const QString &fileName) {
        stringToFile("", fileName);
    }

    QString cleanPath(const QString &dirPath) {
        return QDir::cleanPath(dirPath);
    }

    QString generateAbsolutePath(const QString &dirPath, const QString &relativePath) {
        if (relativePath.isEmpty())
            return QString();
        return QDir::cleanPath(QDir(dirPath).absoluteFilePath(relativePath));
    }

    QString escapeSpacesInString(const QString &str) {
        QString result = str;
        return result.replace(' ', "%20");
    }

    QStringList extractRelativePaths(const QString &base, const QStringList &destList) {
        QStringList list;
        foreach(const QString &dest, destList) {
            list.append(extractRelativePath(base, dest));
        }
        return list;
    }

    QStringList absolutePaths(const QString &dirPath, const QStringList &relativePaths) {
        QStringList list;
        foreach(const QString &path, relativePaths) {
            list.append(generateAbsolutePath(dirPath, path));
        }
        return list;
    }

    bool isBinaryContent(const QByteArray &text) {
        for (char c: text) {
            if (c == 0) {
                return true;
            }
        }
        return false;
    }

    void clearQPlainTextEditFormat(QTextEdit *editor) {
        QTextCursor cursor = editor->textCursor();
        cursor.select(QTextCursor::Document);
        cursor.setCharFormat(QTextCharFormat());
        cursor.clearSelection();
    }

    int compareFileModifiedTime(const QString &filename, qint64 timestamp) {
        QFileInfo fileInfo1(filename);
        qint64 time = fileInfo1.lastModified().toMSecsSinceEpoch();
        if (time > timestamp) return 1;
        if (time < timestamp) return -1;
        return 0;
    }
}

#endif