#include "attributes.h"
#include "ui_attributes.h"

#include <QScroller>
#include <QScreen>
#include <QLineEdit>
Attributes::Attributes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Attributes)
{
    ui->setupUi(this);


    humanityGenerator();
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

Attributes::~Attributes()
{
    ui->Humanity->deleteLater();
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

    connect(ui->wpModifier,&QAbstractSpinBox::editingFinished,this,&Attributes::calculateWP);
    connect(ui->healthModifier,&QAbstractSpinBox::editingFinished,this,&Attributes::calculateHealth);

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

int Attributes::countDots(QButtonGroup *grp)
{
    int counter = 0;
    for(int i = 0; i < grp->buttons().size(); i++)
    {
        if(grp->buttons().at(i)->isChecked())
        {
            counter++;
        }
        else
        {
            break;
        }
    }
    return counter;
}

void Attributes::calculateHealth()
{

    ui->healthModifier->clearFocus();


    deleteHealth(healthPool);
    healthPool = 3 + countDots(ui->Stamina) + ui->healthModifier->value();
    if(healthPool < 0){
        healthPool = 0;
    }else if(healthPool > 14){
        healthPool = 14;
    }
    ui->healthModifier->setValue(healthPool - 3 - countDots(ui->Stamina));
    for(int i = 0; i < healthPool; i++)
    {
        QCheckBox *dynCheck = new QCheckBox();
        dynCheck->setCheckable(true);
        dynCheck->setTristate(true);
        QString checkBoxStyle("QCheckBox::indicator::unchecked {background-image : url(:/images/checkbox_unchecked.png); }"
                              "QCheckBox::indicator::unchecked::hover {background-image : url(:/images/checkbox_unchecked_hover.png); }"
                              "QCheckBox::indicator::unchecked::pressed {background-image : url(:/images/checkbox_unchecked_pressed.png); }"

                              "QCheckBox::indicator::indeterminate {background-image : url(:/images/checkbox_indeterminate.png); }"
                              "QCheckBox::indicator::indeterminate::hover {background-image : url(:/images/checkbox_indeterminate_hover.png); }"
                              "QCheckBox::indicator::indeterminate::pressed {background-image : url(:/images/checkbox_indeterminate_pressed.png); }"

                              "QCheckBox::indicator::checked {background-image : url(:/images/checkbox_checked.png); }"
                              "QCheckBox::indicator::checked::hover {background-image : url(:/images/checkbox_checked_hover.png); }"
                              "QCheckBox::indicator::checked::pressed {background-image : url(:/images/checkbox_checked_pressed.png); }"

                              "QCheckBox::indicator {width: 16; height: 16 }");
        dynCheck->setStyleSheet(checkBoxStyle);
        dynCheck->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        dynCheck->setAutoExclusive(false);
        ui->Health->addWidget(dynCheck);
    }
}

void Attributes::calculateWP()
{
    ui->wpModifier->clearFocus();
    deleteWP(willpowerPool);
    willpowerPool = countDots(ui->Composure) + countDots(ui->Resolve) + ui->wpModifier->value();
    if(willpowerPool < 0){
        willpowerPool = 0;
    }else if(willpowerPool > 13){
        willpowerPool = 13;
    }
    ui->wpModifier->setValue(willpowerPool - countDots(ui->Resolve) - countDots(ui->Composure));
    for(int i = 0; i < willpowerPool; i++)
    {
        QCheckBox *dynCheck = new QCheckBox();
        dynCheck->setCheckable(true);
        dynCheck->setTristate(true);
        QString checkBoxStyle("QCheckBox::indicator::unchecked {background-image : url(:/images/checkbox_unchecked.png); }"
                              "QCheckBox::indicator::unchecked::hover {background-image : url(:/images/checkbox_unchecked_hover.png); }"
                              "QCheckBox::indicator::unchecked::pressed {background-image : url(:/images/checkbox_unchecked_pressed.png); }"

                              "QCheckBox::indicator::indeterminate {background-image : url(:/images/checkbox_indeterminate.png); }"
                              "QCheckBox::indicator::indeterminate::hover {background-image : url(:/images/checkbox_indeterminate_hover.png); }"
                              "QCheckBox::indicator::indeterminate::pressed {background-image : url(:/images/checkbox_indeterminate_pressed.png); }"

                              "QCheckBox::indicator::checked {background-image : url(:/images/checkbox_checked.png); }"
                              "QCheckBox::indicator::checked::hover {background-image : url(:/images/checkbox_checked_hover.png); }"
                              "QCheckBox::indicator::checked::pressed {background-image : url(:/images/checkbox_checked_pressed.png); }"

                              "QCheckBox::indicator {width: 16; height: 16 }");
        dynCheck->setStyleSheet(checkBoxStyle);
        dynCheck->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        dynCheck->setAutoExclusive(false);
        ui->Willpower->addWidget(dynCheck);
    }
}

void Attributes::deleteHealth(int size_)
{
    for(int i = 0; i < size_; i++)
    {
        ui->Health->itemAt(ui->Health->count() - 1)->widget()->deleteLater();
        ui->Health->removeWidget(ui->Health->itemAt(ui->Health->count() - 1)->widget());
    }

}

void Attributes::deleteWP(int size_)
{
    for(int i = 0; i < size_; i++)
    {
        ui->Willpower->itemAt(ui->Willpower->count() - 1)->widget()->deleteLater();
        ui->Willpower->removeWidget(ui->Willpower->itemAt(ui->Willpower->count() - 1)->widget());
    }
}

QPair<int, int> Attributes::countIndicators(QLayout *layout,int size_)
{
    int superficial = 0;
    int agravated = 0;
    for(int i = 0; i < size_; i++)
    {
        QCheckBox *check = qobject_cast<QCheckBox *>(layout->itemAt(i)->widget());
        if(check->checkState() == Qt::CheckState::Checked)
            agravated++;
        if(check->checkState() == Qt::CheckState::PartiallyChecked)
            superficial++;
    }
    return QPair<int,int>(superficial, agravated);
}

void Attributes::humanityChanged()//nienawidze tego, ale nie chce mi się ogarniać algorytmu do tego, mam wrazenie ze to by zajelo zbyt duzo czasu
{
    unsigned short hum = countIndicators(ui->Humanity, 10).second;
    QString text;
    if(hum == 10)
    {
        text = tr("You don't need Blush of Life.\nYou can have sex.\nYou can keep watch at day like mortal.\nYou can eat normal food.\nDamage from sunlight are split by half.");
    }
    else if(hum == 9)
    {
        text = tr("You don't need Blush of Life.\nYou can have sex.\nYou can awaken 1 hour before the sunrise and keep watch 1h after sunset.\nYou can eat raw meat and drink.\n");
    }
    else if(hum == 8)
    {
        text = tr("You roll 2 dices for Blush of Life.\nYou can have sex with Blush of Life.\nYou can drink wine with Blush of Life.\nYou can awaken 1 hour before the sunrise.");
    }
    else if(hum == 7)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma.\nUnless you use Blush of Life foods and drinks makes you vomit.");
    }
    else if(hum == 6)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma with 1 die penalty.\nEven with Blush Of Life foods and drinks make you vomit.");
    }
    else if(hum == 5)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma with 2 dice penalty.\nYou suffer 1 die penalty when interacting with humans.");
    }
    else if(hum == 4)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma with 2 dice penalty.\nYou suffer 2 dice penalty when interacting with humans.");
    }
    else if(hum == 3)
    {
        text = tr("You can no longer have sex.\nYou suffer 4 dice penalty when interacting with humans.");
    }
    else if(hum == 2)
    {
        text = tr("You can no longer have sex.\nYou suffer 6 dice penalty when interacting with humans.");
    }
    else if(hum == 1)
    {
        text = tr("You can no longer have sex.\nYou suffer 8 dice penalty when interacting with humans.");
    }
    else
    {
        text = tr("You have become the beast, you have no longer have control over your character.");
    }
    ui->humanityText->setText(text);
}



void Attributes::humanityGenerator()
{
    for(int i = 0; i < 10; i++)
    {
        QCheckBox *dynCheck = new QCheckBox();
        dynCheck->setCheckable(true);
        dynCheck->setTristate(true);
        QString checkBoxStyle("QCheckBox::indicator::unchecked {background-image : url(:/images/checkbox_unchecked.png); }"
                              "QCheckBox::indicator::unchecked::hover {background-image : url(:/images/checkbox_unchecked_hover.png); }"
                              "QCheckBox::indicator::unchecked::pressed {background-image : url(:/images/checkbox_unchecked_pressed.png); }"

                              "QCheckBox::indicator::indeterminate {background-image : url(:/images/checkbox_indeterminate.png); }"
                              "QCheckBox::indicator::indeterminate::hover {background-image : url(:/images/checkbox_indeterminate_hover.png); }"
                              "QCheckBox::indicator::indeterminate::pressed {background-image : url(:/images/checkbox_indeterminate_pressed.png); }"

                              "QCheckBox::indicator::checked {background-image : url(:/images/checkbox_humanity_checked.png); }"
                              "QCheckBox::indicator::checked::hover {background-image : url(:/images/checkbox_humanity_checked_hover.png); }"
                              "QCheckBox::indicator::checked::pressed {background-image : url(:/images/checkbox_humanity_checked_pressed.png); }"

                              "QCheckBox::indicator {width: 16; height: 16 }");
        dynCheck->setStyleSheet(checkBoxStyle);
        dynCheck->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        dynCheck->setAutoExclusive(false);
        ui->Humanity->addWidget(dynCheck);
        connect(dynCheck,&QCheckBox::stateChanged,this,&Attributes::humanityChanged);
    }
}
