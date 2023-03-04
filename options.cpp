#include "mainwindow.h"
#include "options.h"
#include "ui_options.h"



Options::Options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    listOfAllSaves = QList<QWidget *>();
    loadSaveButtonGroup = new QButtonGroup(this);
    loadSaveButtonGroup->setExclusive(true);
    deleteSaveButtonGroup = new QButtonGroup(this);
    deleteSaveButtonGroup->setExclusive(false);

    connect(loadSaveButtonGroup, &QButtonGroup::buttonClicked, this, &Options::on_saveClicked);

    ui->savesWidget->setVisible(false);
    checkSaves();

}

Options::~Options()
{
    for(auto save: listOfAllSaves)
        save->deleteLater();
    delete ui;
}

void Options::checkSaves()
{
    if(MainWindow::checkFilesPermission())
    {
        qDebug() << path;
        if(!dir.exists())
        {
            if(!dir.mkdir(""))
            {
                qDebug() << "Can't create dir";
                return;
            }
        }
        else
        {
            for(const auto &saveName : dir.entryList(QDir::Files))
            {
                if(saveName.contains(".sav"))
                    createSaveWidget(saveName);
            }
        }
    }
}
void Options::on_saveButton_clicked()
{
    QJsonObject json;
    MainWindow *w = dynamic_cast<MainWindow *> (this->nativeParentWidget());
    if (0 != w){
        json = w->getSaveData();
    }
    else{return;}

    if(MainWindow::checkFilesPermission())
    {

        qDebug() << path;
        if(!dir.exists())
        {
            if(!dir.mkdir(""))
            {
                qDebug() << "Can't create dir";
                return;
            }
        }
        else
        {
            qDebug() << dir.entryList() << "ISTNIEJE";
        }

        QString fileName = ui->saveName->text() + ".sav";
        fileName = fileName.simplified();
        if(ui->saveName->text().simplified() == "" || ui->saveName->text().simplified() == " ")
        {
            fileName = "save.sav";
        }
        QString absolutePathToSave = path + "/" + fileName;
        QFile saveFile(absolutePathToSave);
        if(saveFile.exists())
        {
            QPushButton *widget = this->findChild<QPushButton *>(fileName);
            widget->parentWidget()->deleteLater();
        }
        saveFile.open(QFile::WriteOnly);
        saveFile.write(QJsonDocument(json).toJson());
        saveFile.close();
        qDebug() << "Exists? " << QFile::exists(saveFile.fileName());

        createSaveWidget(fileName);
        ui->saveName->clear();
    }
}

void Options::createSaveWidget(const QString& saveName)
{
    QWidget *mainWidget = new QWidget(this);
    QPushButton *loadButton = new QPushButton(mainWidget);
    loadButton->setObjectName(saveName);
    loadButton->setText(saveName);
    loadButton->setCheckable(true);
    loadButton->setSizePolicy(ui->pushButton_2->sizePolicy());
    QCheckBox *deleteCheckBox = new QCheckBox(mainWidget);
    deleteCheckBox->setSizePolicy(ui->checkBox->sizePolicy());
    deleteCheckBox->setText("Delete?");

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->addWidget(loadButton);
    mainLayout->addWidget(deleteCheckBox);

    loadSaveButtonGroup->addButton(loadButton);
    deleteSaveButtonGroup->addButton(deleteCheckBox);

    ui->savesLayout->addWidget(mainWidget);
    listOfAllSaves.append(mainWidget);
}

void Options::on_loadSaveButton_clicked()
{
    MainWindow *w = dynamic_cast<MainWindow *> (this->nativeParentWidget());
    QList<QPushButton *> listOfButtons = this->findChildren<QPushButton *>();
    for(auto but : listOfButtons)
    {
        if(but->isChecked())
        {
            QString nameOfFile = but->text();
            if(!nameOfFile.contains(".sav"))
            {
                nameOfFile.append(".sav");
            }
            QString absolutePathToSave = path + "/" + nameOfFile;
            QFile saveFile(absolutePathToSave);
            qDebug() << "FAJL";
            saveFile.open(QFile::ReadOnly);
            QByteArray saveData = saveFile.readAll();
            qDebug() <<"readALl";
            QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
            QJsonObject json = loadDoc.object();
            if (0 != w){
                w->readSaveData(json);
            }
            saveFile.close();
            return;
        }
    }
}


void Options::on_deleteSavesButton_clicked()
{
    for(auto but : deleteSaveButtonGroup->buttons())
    {
        if(but->isChecked())
        {
            QWidget *widget = but->parentWidget();
            QPushButton *buttonWithName = widget->findChild<QPushButton *>();
            QFile file(path + "/" +buttonWithName->text());
            file.remove();
            qDebug() << dir.entryList();
            listOfAllSaves.removeOne(widget);
            widget->deleteLater();
        }
    }
}

void Options::on_saveClicked(QAbstractButton *bt)
{
    QString save(bt->text());
    save.chop(4);
    ui->saveName->setText(save);
}

