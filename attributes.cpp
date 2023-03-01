#include "attributes.h"
#include "ui_attributes.h"

#include <QScroller>
#include <QScreen>
#include <QLineEdit>
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

    }
    connect(ui->AttributesGroup,&QButtonGroup::buttonToggled,this,&Attributes::bolding);

}



QLayout* Attributes::findParentLayout(QWidget* w)
{
    if (w->parentWidget() != nullptr)
        if (w->parentWidget()->layout() != nullptr)
            return findParentLayout(w, w->parentWidget()->layout());
    return nullptr;
}

QLayout* Attributes::findParentLayout(QWidget* w, QLayout* topLevelLayout)
{
    for (QObject* qo: topLevelLayout->children())
    {
        QLayout* layout = qobject_cast<QLayout*>(qo);
        if (layout != nullptr)
        {
            if (layout->indexOf(w) > -1)
                return layout;
            else if (!layout->children().isEmpty())
            {
                layout = findParentLayout(w, layout);
                if (layout != nullptr)
                    return layout;
            }
        }
    }
    return nullptr;
}

void Attributes::bolding(QAbstractButton *bt, bool state)
{
    QFont font = findParentLayout(bt)->itemAt(0)->widget()->font();
    font.setBold(state);
    findParentLayout(bt)->itemAt(0)->widget()->setFont(font);
}


void Attributes::dynamicRemoveDots(QAbstractButton *bt)
{
    if(bt->isChecked())
    {
        for(int i = 0; i < bt->group()->buttons().size(); i++)
        {
            if(bt->group()->buttons().at(i)->objectName() == bt->objectName())
                break;
            bt->group()->buttons().at(i)->setChecked(true);
        }
    }
    else
    {
        int del = 0;
        for(int i = 0; i < bt->group()->buttons().size(); i++)
        {
            if(bt->group()->buttons().at(i)->objectName() == bt->objectName())
            {
                del = i;
                break;
            }
        }
        for(int i = del; i < bt->group()->buttons().size(); i++)
        {
            bt->group()->buttons().at(i)->setChecked(false);
        }
    }
    if(Attributes::windowTitle().toStdString()[Attributes::windowTitle().toStdString().size() - 1] != '*')
        Attributes::setWindowTitle(Attributes::windowTitle() + "*");
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

