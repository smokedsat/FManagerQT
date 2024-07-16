#ifndef FILESYSTEMDELEGATE_H
#define FILESYSTEMDELEGATE_H


#include <QStyledItemDelegate>
#include <QFile>

const QString PERMISSIONS_NAMES[] = {
                                     "ReadUser", "WriteUser", "ExeUser",
                                     "ReadGroup", "WriteGroup", "ExeGroup",
                                     "ReadOther", "WriteOther", "ExeOther"};

const int PERMISSIONS_MAX=QFile::ReadOwner;
const int c_permission[]{
    /*ReadUser  = */ 0x0400, /*WriteUser  = */ 0x0200, /*ExeUser  = */ 0x0100,
    /*ReadGroup = */ 0x0040, /*WriteGroup = */ 0x0020, /*ExeGroup = */ 0x0010,
    /*ReadOther = */ 0x0004, /*WriteOther = */ 0x0002, /*ExeOther = */ 0x0001
};

class FileSystemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    FileSystemDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // FILESYSTEMDELEGATE_H
