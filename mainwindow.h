#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class QTreeView;
class FileSystemModel;
class FileSystemDelegate;
class QLabel;
class QPushButton;
class QFileDialog;
class QTextEdit;
class QDateTimeEdit;
class QGroupBox;
class QCheckBox;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFileSelected(const QModelIndex &index);
    void onSetPermissionsClicked(/*const QModelIndex &index*/);

private:
    QTextEdit * textChecksums;
    QDateTimeEdit * editNewDate;
    QGroupBox * permissionsGroup;
//    QTextEdit * textPermissions;
    QFileDialog * fileDialog;
    QTreeView *treeView;
    FileSystemModel *model;
    QLabel *fileInfoLabel;
    QPushButton *calcChecksumButton;
    QPushButton *changePermissionsButton;
    QPushButton *changeModificationDateButton;
    QList<QCheckBox *> permissions;
private:
    Ui::MainWindow * ui;
};
#endif // MAINWINDOW_H
