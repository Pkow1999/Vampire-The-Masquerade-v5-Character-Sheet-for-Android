#include "diceroller.h"
#include "ui_diceroller.h"
#include "mainwindow.h"

#include <QCheckBox>
#include <QRandomGenerator>

DiceRoller::DiceRoller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiceRoller)
{
    ui->setupUi(this);

}

DiceRoller::DiceRoller(QWidget *parent, const QStringList &listOfAttributes, const QStringList &listOfSkills) :
    QWidget(parent),
    ui(new Ui::DiceRoller)
{
    ui->setupUi(this);
    ui->attributeComboBox->addItems(listOfAttributes);
    ui->skillComboBox->addItems(listOfSkills);
}

DiceRoller::~DiceRoller()
{
    delete ui;
}

void DiceRoller::on_pushButton_clicked()//updateInfo
{
    QString Text = "";
    QMapIterator<QString, int> i(MainWindow::mapOfSkillsWithValue);
    while (i.hasNext()) {
        i.next();
        Text += i.key() + ": " + QString::number(i.value()) + ", ";
    }
    ui->textBrowser->setText(Text);
}


void DiceRoller::deleteDices(int size_)
{
    for(int i = 0; i < size_; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            ui->Rolls->itemAt(i)->layout()->itemAt(j)->widget()->deleteLater();
        }
        ui->Rolls->itemAt(i)->layout()->deleteLater();
    }
}

void DiceRoller::on_rollButton_clicked()
{
    deleteDices(MainWindow::counter);
    MainWindow::counter = 0;
    //MainWindow::hunger = countDots(ui->Hunger);

    QMapIterator<QString, int> i(MainWindow::mapOfSkillsWithValue);
    while (i.hasNext()) {
        i.next();
        MainWindow::counter += i.value();
    }
    MainWindow::counter += ui->diceModifier->value();
    createDices(MainWindow::counter);
}

void DiceRoller::createDices(int size_)
{
    for(int i = 0; i < size_; i++)
    {
        QCheckBox *dynCheck = new QCheckBox(this);
        dynCheck->setStyleSheet("QCheckBox::indicator{width: 15px;height: 15px;}");
        dynCheck->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        QVBoxLayout *dynLayout = new QVBoxLayout();
        dynLayout->setSizeConstraint(QLayout::SetFixedSize);
        int generatedNumber = QRandomGenerator::system()->bounded(10) + 1;
        QLabel *dynLabel = new QLabel(this);
        dynLabel->setAlignment(Qt::AlignCenter);
        dynLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        dynLabel->setText(QString::number(generatedNumber));
        dynLabel->setStyleSheet("QLabel { font-size : 20px;}");
        dynCheck->setObjectName("czek" + QString::number(i));
        dynCheck->setCheckable(true);
        if(i > MainWindow::counter - MainWindow::hunger - 1)
        {
            dynLabel->setStyleSheet("QLabel { color : red; font-size : 20px;}");
        }
        dynLayout->addWidget(dynLabel);
        dynLayout->addWidget(dynCheck);
        dynLayout->setAlignment(dynLabel, Qt::AlignCenter);
        dynLayout->setAlignment(dynCheck, Qt::AlignCenter);
        ui->Rolls->addLayout(dynLayout);
    }
}


void DiceRoller::on_rerollButton_clicked()
{
    for(int i = 0; i < MainWindow::counter; i++)
    {
        QAbstractButton *bt = qobject_cast<QAbstractButton *>(ui->Rolls->itemAt(i)->layout()->itemAt(1)->widget());
        if(bt->isChecked())
        {
            int generatedNumber = QRandomGenerator::system()->bounded(10) + 1;
            {
                QLabel *lb = qobject_cast<QLabel *>(ui->Rolls->itemAt(i)->layout()->itemAt(0)->widget());
                lb->setText(QString::number(generatedNumber));
                if(lb->styleSheet() == "QLabel { color : red; font-size : 20px;}"){
                    lb->setStyleSheet("QLabel { color : purple; font-size: 20px;}");
                }
                else lb->setStyleSheet(" QLabel{color : blue; font-size : 20px;}");
            }
        }
    }
}

