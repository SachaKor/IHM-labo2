#ifndef UTILS_H
#define UTILS_H


class Utils
{
public:
    Utils();
    static void setMacEnvironment();
    static QString getFolderName(const QString &path);
    static QString getFileFormat(const QString &filename);
    static QMap<QString, QString> mapProperties(const QString& props);
    static bool fileExists(const QString& path);
};

#endif // UTILS_H
