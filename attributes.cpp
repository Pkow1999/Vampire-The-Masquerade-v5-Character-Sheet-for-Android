#include "attributes.h"
#include "ui_attributes.h"

#include <QScroller>
#include <QScreen>
#include <QLineEdit>
#include <QJsonArray>
#include "mainwindow.h"
Attributes::Attributes(QWidget *parent, Indicators *anotherWindowPointer) :
    QWidget(parent),
    ui(new Ui::Attributes)
{
    ui->setupUi(this);

    indicatorsPointer = anotherWindowPointer;

    connectAllButtonGroup();
}

QStringList Attributes::getAttributesList()
{
    QStringList listOfAttributes{};
    foreach (auto button, ui->AttributesGroup->buttons()) {
        listOfAttributes.append(button->text());
    }
    return listOfAttributes;
}

void Attributes::setIndicatorsPointer(Indicators *ind)
{
    indicatorsPointer = ind;
    connect(ui->Resolve, &QButtonGroup::buttonClicked,indicatorsPointer,&Indicators::createWillpower);
    connect(ui->Composure, &QButtonGroup::buttonClicked,indicatorsPointer,&Indicators::createWillpower);
    connect(ui->Stamina, &QButtonGroup::buttonClicked,indicatorsPointer,&Indicators::createHealth);
}



Attributes::~Attributes()
{
    delete ui;
}



void Attributes::connectAllButtonGroup(){
    connect(ui->Strength,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Dexterity,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Stamina,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Stamina,&QButtonGroup::buttonClicked,this,&Attributes::calculateHealth);


    connect(ui->Charisma,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Manipulation,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Composure,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Composure,&QButtonGroup::buttonClicked,this,&Attributes::calculateWP);

    connect(ui->Intelligence,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Wits,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Resolve,&QButtonGroup::buttonClicked,this,&Attributes::dynamicRemoveDots);
    connect(ui->Resolve,&QButtonGroup::buttonClicked,this,&Attributes::calculateWP);
    if(indicatorsPointer != nullptr)
    {
        connect(ui->Resolve, &QButtonGroup::buttonClicked,indicatorsPointer,&Indicators::createWillpower);
        connect(ui->Composure, &QButtonGroup::buttonClicked,indicatorsPointer,&Indicators::createWillpower);
        connect(ui->Stamina, &QButtonGroup::buttonClicked,indicatorsPointer,&Indicators::createHealth);

    }
    connect(ui->AttributesGroup,&QButtonGroup::buttonToggled,this,&Attributes::callBoldingFromParent);

}

void Attributes::callBoldingFromParent(QAbstractButton *bt, bool state)
{
    MainWindow *w = dynamic_cast<MainWindow *> (this->nativeParentWidget());
    if (0 != w)
        w->bolding(bt, state, 1, 1);
}

void Attributes::dynamicRemoveDots(QAbstractButton *bt)
{
    MainWindow::dynamicRemoveDots(bt);
}

void Attributes::calculateHealth()
{
    MainWindow::healthFromAttributes = MainWindow::countDots(ui->Stamina) + 3;
}

void Attributes::calculateWP()
{
    MainWindow::willpowerFromAttributes = MainWindow::countDots(ui->Composure) + MainWindow::countDots(ui->Resolve);

}

void Attributes::on_lockButton_toggled(bool checked)
{
    if(checked)
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/lock20x20.png")));
        QList<QRadioButton *> allRadioButtons = this->findChildren<QRadioButton *>();
        for(auto but: allRadioButtons)
        {
            but->setEnabled(false);
        }
    }
    else
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/unlock20x20.png")));
        QList<QRadioButton *> allRadioButtons = this->findChildren<QRadioButton *>();
        for(auto but: allRadioButtons)
        {
            but->setEnabled(true);
        }

    }
}

QJsonObject Attributes::write() const
{
    QList<QCheckBox *> listOfAttributes = this->findChildren<QCheckBox *>();
    QJsonObject json;
    for(auto checkbox : listOfAttributes)
    {
        QLayout *layoutButton = MainWindow::findParentLayout(checkbox)->itemAt(1)->layout();
        QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
        json[checkbox->text()] = MainWindow::countDots(but->group());
    }
    return json;
}
