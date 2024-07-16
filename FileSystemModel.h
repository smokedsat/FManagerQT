#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QCryptographicHash>

class FileSystemModel : public QFileSystemModel {
    Q_OBJECT

public:
    FileSystemModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QString calculateChecksum(const QString &filePath, QCryptographicHash::Algorithm type = QCryptographicHash::Sha256) const;
    int columnCount(const QModelIndex & parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override ;

    QString permissionsToString(QFileDevice::Permissions permissions) const;
};

#endif // FILESYSTEMMODEL_H
