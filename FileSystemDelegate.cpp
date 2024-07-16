#include "FileSystemDelegate.h"

#include <QComboBox>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QAction>
#include <QList>
#include <QFile>

FileSystemDelegate::FileSystemDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *FileSystemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QComboBox *editor = new QComboBox(parent);
    QCheckBox * checkBox = new QCheckBox(parent);

    if(index.column() == 3)
    {
        QDateTimeEdit * editor = new QDateTimeEdit(parent);
        editor->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        return editor;
    }

    if(index.column() == 4)
    {
        int permission_max = QFile::ReadOwner;
        QList<QAction*> actionsList;

        for(int i = 0 ; i < 9; i++, permission_max /= 2 )
        {
            QAction * action = new QAction(PERMISSIONS_NAMES[i]);
            action->setCheckable(true);
            action->setData(permission_max);
            actionsList.push_back(action);
        }

        checkBox->addActions(actionsList);
        return checkBox;
    }
    return QStyledItemDelegate::createEditor(parent, option,index);
}

void FileSystemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (index.column() == 4) {
        int permissions = index.model()->data(index, Qt::EditRole).toInt();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentIndex(comboBox->findData(permissions));
    } else if (index.column() == 3) {
        QString date = index.model()->data(index, Qt::EditRole).toString();
        QDateTimeEdit *dateEdit = static_cast<QDateTimeEdit*>(editor);
        dateEdit->setDateTime(QDateTime::fromString(date, Qt::ISODate));
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void FileSystemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  if (index.column() == 3) {
        QDateTimeEdit *dateEdit = static_cast<QDateTimeEdit*>(editor);
        model->setData(index, dateEdit->dateTime().toString(Qt::ISODate), Qt::EditRole);
    }
  else if(index.column() == 4)
  {
      QComboBox * comboBox = static_cast<QComboBox*>(editor);
      int permissions = comboBox->currentData().toInt();
      model->setData(index,permissions,Qt::EditRole);
  }
  else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void FileSystemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
