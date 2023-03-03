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

    calculateBlood();
    listOfClans = {"banu haqim", "brujah", "gangrel", "hecata", "lasombra", "malkavian", "ministry", "nosferatu", "ravnos", "salubri", "toreador", "tremere", "tzimisce", "ventrue"};
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
//    connect(ui->lineEdit_7,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
//    connect(ui->lineEdit_8,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_9,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_10,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);

    connect(ui->spinBox,&QAbstractSpinBox::editingFinished, this, &PersonalData::spinBoxHandling);
    connect(ui->spinBox_2,&QAbstractSpinBox::editingFinished, this, &PersonalData::spinBoxHandling);

    connect(ui->lineEdit_4,&QLineEdit::editingFinished, this, &PersonalData::clanHandle);

    connect(ui->bloodPotencyGroup, &QButtonGroup::buttonClicked, this, &PersonalData::dynamicRemoveDots);
    connect(ui->bloodPotencyGroup, &QButtonGroup::buttonClicked, this, &PersonalData::calculateBlood);


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

void PersonalData::calculateBlood()
{
    int bloodPotency = MainWindow::countDots(ui->bloodPotencyGroup);
    ui->BloodPotency->setText(QString::number(bloodPotency /2 + bloodPotency % 2 + 1));
    ui->MendAmount->setText(QString::number(bloodPotency < 6 ? bloodPotency / 2 + 1 : bloodPotency / 2));
    ui->PowerBonus->setText(QString::number(bloodPotency / 2));
    ui->RouseReRoll->setText(tr("Level ") + QString::number(bloodPotency > 0 ? bloodPotency /2 + bloodPotency % 2 : 0));
    ui->BaneSevarity->setText(QString::number(bloodPotency > 0 ? bloodPotency /2 + bloodPotency % 2 + 1 : 0 ));



    if(bloodPotency > 0)
        ui->BloodPotency->setText(ui->BloodPotency->text() + tr(" dices"));
    else
        ui->BloodPotency->setText(ui->BloodPotency->text() + tr(" die"));

    if(bloodPotency > 3)
        ui->PowerBonus->setText(ui->PowerBonus->text() + tr(" dices"));
    else
        ui->PowerBonus->setText(ui->PowerBonus->text() + tr(" die"));

    if(bloodPotency > 1)
        ui->MendAmount->setText(ui->MendAmount->text() + tr(" superficial damage"));
    else
        ui->MendAmount->setText(ui->MendAmount->text() + tr(" superficial damage"));

    if(bloodPotency > 2)
        ui->RouseReRoll->setText(ui->RouseReRoll->text() + tr(" and Lower"));
}

void PersonalData::dynamicRemoveDots(QAbstractButton *bt)
{
    MainWindow::dynamicRemoveDots(bt);
}

QJsonObject PersonalData::write() const
{
    QJsonObject json;
    int counter = 1;
    json["Blood Potency"] = QString::number(MainWindow::countDots(ui->bloodPotencyGroup));
    QList<QLabel *> listOfLabels = ui->personalWidget->findChildren<QLabel *>();
    listOfLabels.removeOne(ui->currentExp);
    listOfLabels.removeOne(ui->allExp);
    for(auto label : listOfLabels)
    {
        QLayout *layout = MainWindow::findParentLayout(label);
        QLineEdit * valueText = static_cast<QLineEdit *>(layout->itemAt(1)->widget());
        json[label->text()] = valueText->text();
    }
    json[ui->currentExp->text()] = QString::number(ui->spinBox->value());
    json[ui->allExp->text()] = QString::number(ui->spinBox_2->value());
    return json;
}

void PersonalData::read(const QJsonObject &json)
{
    if(json.contains("Blood Potency") && json["Blood Potency"].isString())
    {
        if(json["Blood Potency"].toString().toInt() != 0)
        {
            int dots = -json["Blood Potency"].toString().toInt() - 1;
            ui->bloodPotencyGroup->button(dots)->click();
        }
    }
    QList<QLabel *> listOfLabels = ui->personalWidget->findChildren<QLabel *>();
    listOfLabels.removeOne(ui->currentExp);
    listOfLabels.removeOne(ui->allExp);
    for(auto label : listOfLabels)
    {
        QLayout *layout = MainWindow::findParentLayout(label);
        QLineEdit * valueText = static_cast<QLineEdit *>(layout->itemAt(1)->widget());
        if(json.contains(label->text()) && json[label->text()].isString())
        {
            QLineEdit * valueText = static_cast<QLineEdit *>(layout->itemAt(1)->widget());
            valueText->setText(json[label->text()].toString());
        }
    }
    if(json.contains(ui->currentExp->text()) && json[ui->currentExp->text()].isString())
    {
        ui->spinBox->setValue(json[ui->currentExp->text()].toString().toInt());
    }
    if(json.contains(ui->allExp->text()) && json[ui->allExp->text()].isString())
    {
        ui->spinBox_2->setValue(json[ui->allExp->text()].toString().toInt());
    }


}

void PersonalData::clear()
{
    for(auto radioBut : this->findChildren<QRadioButton *>())
    {
        radioBut->setChecked(false);
    }
    for(auto lineEdit : this->findChildren<QLineEdit *>())
    {
        lineEdit->clear();
    }
    for(auto spinBox : this->findChildren<QSpinBox *>())
    {
        spinBox->setValue(0);
    }
}
