#include "personaldata.h"
#include "QScreen"
#include "ui_personaldata.h"
#include "mainwindow.h"

PersonalData::PersonalData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalData)
{
    ui->setupUi(this);
    connectAllLineEdits();

    listOfClans = {"banu haqim", "brujah", "gangrel", "hecata", "lasombra", "malkavian", "ministry", "nosferatu", "ravnos", "toreador", "tremere", "tzimisce", "ventrue"};
}

QLineEdit* PersonalData::getClanLineEdit()
{
    return ui->lineEdit;
}

void PersonalData::connectAllLineEdits()
{
    connect(ui->lineEdit,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_2,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_3,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_4,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_5,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_6,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_7,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);

    connect(ui->spinBox,&QAbstractSpinBox::editingFinished, this, &PersonalData::spinBoxHandling);
    connect(ui->spinBox_2,&QAbstractSpinBox::editingFinished, this, &PersonalData::spinBoxHandling);

    connect(ui->lineEdit_4,&QLineEdit::editingFinished, this, &PersonalData::clanHandle);

}
PersonalData::~PersonalData()
{
    delete ui;
}

void PersonalData::lineEditHandling()
{
    QLineEdit *line = static_cast<QLineEdit *>(sender());
    line->clearFocus();
}

void PersonalData::spinBoxHandling()
{
    QSpinBox *spin = static_cast<QSpinBox *>(sender());
    spin->clearFocus();
}

void PersonalData::clanHandle()
{
    QString lowerText = ui->lineEdit_4->text().toLower();
    QString url = "";
    for(const auto &clan : listOfClans)
    {
        if(lowerText.contains(clan))
        {
            url = clan+"_background_transparent.png";
            break;
        }
    }
    if(url == ""){
        MainWindow::backgroundImageUrl = "";
        return;
    }
    qDebug() <<"URL: "<< url;
    MainWindow::backgroundImageUrl = ":/images/background-image/"+url;
    qDebug() << "BACKGROUND IMAGE URL: " << MainWindow::backgroundImageUrl;
}
