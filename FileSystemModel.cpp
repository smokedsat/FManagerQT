#include "FileSystemModel.h"

#include <QFile>
#include <QCryptographicHash>
#include <QDateTime>

FileSystemModel::FileSystemModel(QObject *parent)
    : QFileSystemModel(parent) {


}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QFileInfo fileInfo = QFileSystemModel::fileInfo(index);
        if (index.column() == 1) {
            return fileInfo.size();
//            return permissionsToString(fileInfo.permissions());
        }
//        if (index.column() == 2) {
//            QString filePath = QFileSystemModel::filePath(index);
//            return calculateChecksum(filePath);
//        }
        if (index.column() == 3) {
            return fileInfo.lastModified(); //.toString()
        }
        if(index.column() == 4)
        {
            return permissionsToString(fileInfo.permissions());
        }
    }
    return QFileSystemModel::data(index, role);
}

bool FileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::EditRole)
    {
        QFileInfo fileInfo = QFileSystemModel::fileInfo(index);

//        if (index.column() == 1) {
//            QFile file(fileInfo.absoluteFilePath());
//            QFileDevice::Permissions permissions = static_cast<QFileDevice::Permissions>(value.toInt());
//            return file.setPermissions(permissions);
//        }
//        if (index.column() == 3) {
//            QDateTime newDate = QDateTime::fromString(value.toString(), Qt::ISODate);
//            if (newDate.isValid()) {
//                qDebug() << "Time data is valid";

//                QFile file(fileInfo.absoluteFilePath());
//                file.open(QFile::ReadOnly);
//                if(file.isOpen())
//                {

//                }

//                return QFile::setFileTime(fileInfo.absoluteFilePath(), newDate, QFileDevice::FileModificationTime);
//            }
//        }
    }
    return QFileSystemModel::setData(index, value, role);
}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);
//    if (index.column() == 3 || index.column() == 4)
//        return defaultFlags | Qt::ItemIsEditable;
    return defaultFlags;
}

int FileSystemModel::columnCount(const QModelIndex &parent) const
{
    return QFileSystemModel::columnCount(parent) + 1;
}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section == 4)
    {
        return "Разрешения";
    }
    else
    {
        return QFileSystemModel::headerData(section, orientation, role);
    }
}


QString FileSystemModel::calculateChecksum(const QString &filePath, QCryptographicHash::Algorithm type) const {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QCryptographicHash hash(type);
        if (hash.addData(&file)) {
            return QString(hash.result().toHex());
        }
    }
    else
    {
        qDebug() << "Cant open file to calculate checksum: " << filePath;
    }
    return QString();
}


QString FileSystemModel::permissionsToString(QFileDevice::Permissions permissions) const {
    QString str;
    str += (permissions & QFileDevice::ReadOwner) ? "r" : "-";
    str += (permissions & QFileDevice::WriteOwner) ? "w" : "-";
    str += (permissions & QFileDevice::ExeOwner) ? "x" : "-";

    str += (permissions & QFileDevice::ReadGroup) ? "r" : "-";
    str += (permissions & QFileDevice::WriteGroup) ? "w" : "-";
    str += (permissions & QFileDevice::ExeGroup) ? "x" : "-";

    str += (permissions & QFileDevice::ReadOther) ? "r" : "-";
    str += (permissions & QFileDevice::WriteOther) ? "w" : "-";
    str += (permissions & QFileDevice::ExeOther) ? "x" : "-";
    return str;
}
