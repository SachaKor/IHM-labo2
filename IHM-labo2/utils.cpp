#include <QProcessEnvironment>

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
