#include <QProcessEnvironment>
#include <QMap>

#include "utils.h"

Utils::Utils() {}

void Utils::setMacEnvironment() {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QStringList envlist = env.toStringList();

    for(int i=0; i < envlist.size(); i++)
    {
        QString entry = envlist[i];

        if(entry.startsWith("PATH="))
        {
            int index = entry.indexOf("=");

            if(index != -1)
            {
                QString value = entry.right(entry.length() - (index+1));
                value += ":/usr/texbin:/usr/local/bin";

                setenv("PATH", value.toLatin1().constData(), true);
            }

            break;
        }
    }
}

QString Utils::getFolderName(const QString &path) {
    QStringList pathList = path.split("/");
    pathList.pop_back();
    QString folderPath = "";
    for(QString p : pathList) {
        folderPath += p;
        folderPath += "/";
    }
    return folderPath;
}

QString Utils::getFileFormat(const QString &filename) {
    int indexOfLastDot = filename.lastIndexOf(".");

    if(indexOfLastDot == -1) {
        return "";
    }

    return filename.mid(indexOfLastDot+1, filename.size()-indexOfLastDot-1);
}

QMap<QString, QString> Utils::mapProperties(const QString& props) {
    QMap<QString, QString> map;
    int formatOptions = props.indexOf("[FORMAT]");
    int endFormatOptions = props.indexOf("[/FORMAT]");

    if(formatOptions == -1 || endFormatOptions == -1) {
        return map;
    }

    QString formatProperties = props.mid(formatOptions,
                                         endFormatOptions-formatOptions);
    formatProperties.replace("[FORMAT]", "");
    QStringList optionsList = formatProperties.split("\n");
    for(QString op : optionsList) {
        QStringList keyValue = op.split("=");
        if(keyValue.size() == 2) {
            map.insert(keyValue.at(0), keyValue.at(1));
        }
    }

    return map;
}
