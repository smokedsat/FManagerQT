#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "FileSystemModel.h"
#include "FileSystemDelegate.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QDateTime>
#include <QFileDialog>
#include <QCheckBox>
#include <QList>

#include <QPushButton>
#include <QFileDialog>
#include <QTreeView>

#include <QTextEdit>
#include <QLabel>
#include <QDebug>
#include <QDateTimeEdit>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      treeView(new QTreeView(this)),
      model(new FileSystemModel(this)),
      fileInfoLabel(new QLabel(this)),
      calcChecksumButton(new QPushButton("Calculate Checksum", this)),
      changePermissionsButton(new QPushButton("Change Permissions", this)),
      changeModificationDateButton(new QPushButton("Change Modification Date", this)),
      fileDialog(new QFileDialog(this)),
      textChecksums(new QTextEdit(this)),
      editNewDate(new QDateTimeEdit(this)),
      //      textPermissions(new QTextEdit(this)),
      permissionsGroup(new QGroupBox("Permissions", this)),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSplitter *splitter = new QSplitter(this);

    QString path = fileDialog->getExistingDirectory(layout()->widget(),
                                                    QString::fromUtf8("Открыть папку"),
                                                    QDir::currentPath());

    model->setRootPath(path);
    model->setOption(QFileSystemModel::DontResolveSymlinks);

    treeView->setAcceptDrops(true);
    treeView->setDragEnabled(true);
    treeView->setUpdatesEnabled(true);
    treeView->setMouseTracking(true);
    treeView->setDragDropMode(QTreeView::DragDropMode::DragDrop);

    treeView->setModel(model);
    treeView->setRootIndex(model->index(path));
    treeView->setItemDelegate(new FileSystemDelegate(this));

    splitter->addWidget(treeView);

    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    editNewDate->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

    textChecksums->setReadOnly(true);


    QVBoxLayout * permissionsLayout = new QVBoxLayout;
    for(int i = 0 ; i < 9; i++)
    {
        QCheckBox * checkBox = new QCheckBox(PERMISSIONS_NAMES[i]);
        permissionsLayout->addWidget(checkBox);
        permissions.push_back(checkBox);
    }
    permissionsGroup->setLayout(permissionsLayout);


    rightLayout->addWidget(fileInfoLabel);
    rightLayout->addWidget(calcChecksumButton);
    rightLayout->addWidget(textChecksums);
    rightLayout->addWidget(changePermissionsButton);
    rightLayout->addWidget(permissionsGroup);
    rightLayout->addWidget(changeModificationDateButton);
    rightLayout->addWidget(editNewDate);
    rightLayout->addStretch();

    splitter->addWidget(rightPanel);
    setCentralWidget(splitter);


    // Подключение сигналов и слотов
    connect(treeView, &QTreeView::clicked, this, &MainWindow::onFileSelected);


    connect(calcChecksumButton, &QPushButton::clicked, this, [=]() {
        QModelIndex index = treeView->currentIndex();
        textChecksums->clear();
        if (index.isValid()) {

            QString md5, sha256, sha512;
            QFileInfo fileInfo = model->fileInfo(index);
            QString filename = fileInfo.absoluteFilePath();

            md5 = model->calculateChecksum(filename, QCryptographicHash::Md5);
            sha256 = model->calculateChecksum(filename,QCryptographicHash::Sha256);
            sha512 = model->calculateChecksum(filename,QCryptographicHash::Sha512);

            textChecksums->setText(QString("MD5: " + md5 + "\n SHA256: " + sha256 + "\n SHA512: " + sha512));
            textChecksums->update();
        }
    });

    connect(changePermissionsButton, &QPushButton::clicked, this, &MainWindow::onSetPermissionsClicked);


    connect(changeModificationDateButton, &QPushButton::clicked, this, [&]() {
        QModelIndex index = treeView->currentIndex();
        if (index.isValid()) {
            QDateTime newDate = editNewDate->dateTime();

            QFileInfo fileInfo = model->fileInfo(index);

            if(newDate.isValid())
            {
                qDebug() << "Time data is valid";
                QFile file(fileInfo.absoluteFilePath());
                file.open(QFile::ReadOnly);

                if(file.isOpen())
                {
                    auto res = file.setFileTime(newDate, QFile::FileModificationTime);

                    if(res)
                    {
                        qDebug() << "Date changed to " << newDate.toString();
                        qDebug() << "New date of modification after changing: " << file.fileTime(QFile::FileModificationTime).toString();
                    }
                    else
                    {
                        qDebug() << "Cant set time for file " << fileInfo.fileName();
                    }
                    this->layout()->update();
                }
                else
                {
                    qDebug() << "Cant open file in read only mode "<< fileInfo.fileName();
                }
            }
            else
            {
                qDebug() << "New Date is invalid.";
            }
        }
    });


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onFileSelected(const QModelIndex &index) {
    if (index.isValid()) {
        QString filePath = model->filePath(index);
        QFileInfo fileInfo(filePath);
        QString info = QString("File: %1\nSize: %2 bytes\nLast Modified: %3\nPermissions: %4")
                .arg(fileInfo.fileName())
                .arg(fileInfo.size())
                .arg(fileInfo.lastModified().toString())
                .arg(model->data(index.sibling(index.row(), 1), Qt::DisplayRole).toString());

        QString permissions = model->permissionsToString(fileInfo.permissions());

        fileInfoLabel->setText(info);

    } else {
        fileInfoLabel->clear();
    }
}

void MainWindow::onSetPermissionsClicked(/*const QModelIndex &index*/)
{
    QModelIndex index = treeView->currentIndex();

    if(index.isValid())
    {
        QFileInfo fileInfo = model->fileInfo(index);

        QFile file(fileInfo.absoluteFilePath());

        if(file.exists())
        {
            QFileDevice::Permissions newPermissions;

            if(file.open(QFile::ReadOnly))
            {
                for(int i = 0; i < 9; i++)
                {
                    if(permissions[i]->isChecked())
                    {
                        newPermissions |= (QFile::Permission)c_permission[i];
                    }
                    else
                    {
                        newPermissions &= ~(QFile::Permission)c_permission[i];
                    }
                }

                if(file.setPermissions(newPermissions))
                {
                    QMessageBox::information(this, "Permissions", "Permissions successfully changed for "+fileInfo.fileName());
                }
                else
                {
                    QMessageBox::warning(this, "Permissions", "Failed to change permissions for " + fileInfo.fileName());
                }
            }
            else
            {
                qDebug() << "Failed to open file " << fileInfo.absoluteFilePath();
            }


        }




    }
}
