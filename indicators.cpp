#include "indicators.h"
#include "ui_indicators.h"

#include <QCheckBox>
#include <QJsonArray>
#include "mainwindow.h"


Indicators::Indicators(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Indicators)
{
    ui->setupUi(this);
    humanityGenerator();

    connect(ui->wpModifier,&QAbstractSpinBox::editingFinished,this,&Indicators::createWillpower);
    connect(ui->healthModifier,&QAbstractSpinBox::editingFinished,this,&Indicators::createHealth);
    connect(ui->hungerGroup, &QButtonGroup::buttonClicked,this, &Indicators::dynamicRemoveDots);
}

Indicators::~Indicators()
{
    delete ui;
}

void Indicators::createWillpower()
{
    qDebug() << "willpowerPool Before: " << MainWindow::willpowerPool;
    if(ui->wpModifier->hasFocus())
        ui->wpModifier->clearFocus();
    deleteWP(MainWindow::willpowerPool);
    MainWindow::willpowerModifier = ui->wpModifier->value();

    MainWindow::willpowerPool = MainWindow::willpowerFromAttributes + MainWindow::willpowerModifier;

    for(int i = 0; i < MainWindow::willpowerPool; i++)
    {
        QCheckBox *dynCheck = new QCheckBox(this);
        dynCheck->setCheckable(true);
        dynCheck->setTristate(true);
        QString checkBoxStyle("QCheckBox::indicator::unchecked {background-image : url(:/images/checkbox/20x20/checkbox_unchecked.png); }"
                              "QCheckBox::indicator::unchecked::hover {background-image : url(:/images/checkbox/20x20/checkbox_unchecked_hover.png); }"
                              "QCheckBox::indicator::unchecked::pressed {background-image : url(:/images/checkbox/20x20/checkbox_unchecked_pressed.png); }"

                              "QCheckBox::indicator::indeterminate {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate.png); }"
                              "QCheckBox::indicator::indeterminate::hover {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate_hover.png); }"
                              "QCheckBox::indicator::indeterminate::pressed {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate_pressed.png); }"

                              "QCheckBox::indicator::checked {background-image : url(:/images/checkbox/20x20/checkbox_checked.png); }"
                              "QCheckBox::indicator::checked::hover {background-image : url(:/images/checkbox/20x20/checkbox_checked_hover.png); }"
                              "QCheckBox::indicator::checked::pressed {background-image : url(:/images/checkbox/20x20/checkbox_checked_pressed.png); }"

                              "QCheckBox::indicator {width: 20; height: 20 }");
        dynCheck->setStyleSheet(checkBoxStyle);
        dynCheck->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
        dynCheck->setAutoExclusive(false);
        ui->Willpower->addWidget(dynCheck);
    }

    qDebug() << "willpowerPool After: " << MainWindow::willpowerPool;
}

void Indicators::createHealth()
{
    qDebug() << "Health Before: " << MainWindow::healthPool;
    if(ui->healthModifier->hasFocus())
        ui->healthModifier->clearFocus();
    deleteHealth(MainWindow::healthPool);
    MainWindow::healthModifier = ui->healthModifier->value();

    MainWindow::healthPool = MainWindow::healthFromAttributes + MainWindow::healthModifier;

    for(int i = 0; i < MainWindow::healthPool; i++)
    {
        QCheckBox *dynCheck = new QCheckBox(this);
        dynCheck->setCheckable(true);
        dynCheck->setTristate(true);
        QString checkBoxStyle("QCheckBox::indicator::unchecked {background-image : url(:/images/checkbox/20x20/checkbox_unchecked.png); }"
                              "QCheckBox::indicator::unchecked::hover {background-image : url(:/images/checkbox/20x20/checkbox_unchecked_hover.png); }"
                              "QCheckBox::indicator::unchecked::pressed {background-image : url(:/images/checkbox/20x20/checkbox_unchecked_pressed.png); }"

                              "QCheckBox::indicator::indeterminate {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate.png); }"
                              "QCheckBox::indicator::indeterminate::hover {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate_hover.png); }"
                              "QCheckBox::indicator::indeterminate::pressed {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate_pressed.png); }"

                              "QCheckBox::indicator::checked {background-image : url(:/images/checkbox/20x20/checkbox_checked.png); }"
                              "QCheckBox::indicator::checked::hover {background-image : url(:/images/checkbox/20x20/checkbox_checked_hover.png); }"
                              "QCheckBox::indicator::checked::pressed {background-image : url(:/images/checkbox/20x20/checkbox_checked_pressed.png); }"

                              "QCheckBox::indicator {width: 20; height: 20 }");
        dynCheck->setStyleSheet(checkBoxStyle);
        dynCheck->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
        dynCheck->setAutoExclusive(false);
        ui->Health->addWidget(dynCheck);
    }

    qDebug() << "Health After: " << MainWindow::healthPool;
}



void Indicators::deleteHealth(int size_)
{
    for(int i = 0; i < size_; i++)
    {
        ui->Health->itemAt(ui->Health->count() - 1)->widget()->deleteLater();
        ui->Health->removeWidget(ui->Health->itemAt(ui->Health->count() - 1)->widget());
    }

}

void Indicators::deleteWP(int size_)
{
    for(int i = 0; i < size_; i++)
    {
        ui->Willpower->itemAt(ui->Willpower->count() - 1)->widget()->deleteLater();
        ui->Willpower->removeWidget(ui->Willpower->itemAt(ui->Willpower->count() - 1)->widget());
    }
}

QPair<int, int> Indicators::countIndicators(QLayout *layout,int size_) const
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

void Indicators::humanityChanged()//nienawidze tego, ale nie chce mi się ogarniać algorytmu do tego, mam wrazenie ze to by zajelo zbyt duzo czasu
{
    MainWindow::humanity = countIndicators(ui->Humanity, 10).second;
    QString text;
    if(MainWindow::humanity == 10)
    {
        text = tr("You don't need Blush of Life.\nYou can have sex.\nYou can keep watch at day like mortal.\nYou can eat normal food.\nDamage from sunlight are split by half.");
    }
    else if(MainWindow::humanity == 9)
    {
        text = tr("You don't need Blush of Life.\nYou can have sex.\nYou can awaken 1 hour before the sunrise and keep watch 1h after sunset.\nYou can eat raw meat and drink.\n");
    }
    else if(MainWindow::humanity == 8)
    {
        text = tr("You roll 2 dices for Blush of Life.\nYou can have sex with Blush of Life.\nYou can drink wine with Blush of Life.\nYou can awaken 1 hour before the sunrise.");
    }
    else if(MainWindow::humanity == 7)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma.\nUnless you use Blush of Life foods and drinks makes you vomit.");
    }
    else if(MainWindow::humanity == 6)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma with 1 die penalty.\nEven with Blush Of Life foods and drinks make you vomit.");
    }
    else if(MainWindow::humanity == 5)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma with 2 dice penalty.\nYou suffer 1 die penalty when interacting with humans.");
    }
    else if(MainWindow::humanity == 4)
    {
        text = tr("You can not have sex but you can fake it with Dexterity + Charisma with 2 dice penalty.\nYou suffer 2 dice penalty when interacting with humans.");
    }
    else if(MainWindow::humanity == 3)
    {
        text = tr("You can no longer have sex.\nYou suffer 4 dice penalty when interacting with humans.");
    }
    else if(MainWindow::humanity == 2)
    {
        text = tr("You can no longer have sex.\nYou suffer 6 dice penalty when interacting with humans.");
    }
    else if(MainWindow::humanity == 1)
    {
        text = tr("You can no longer have sex.\nYou suffer 8 dice penalty when interacting with humans.");
    }
    else
    {
        text = tr("You have become the beast, you have no longer have control over your character.");
    }
    ui->humanityText->setText(text);
}



void Indicators::humanityGenerator()
{
    for(int i = 0; i < 10; i++)
    {
        QCheckBox *dynCheck = new QCheckBox(this);
        dynCheck->setCheckable(true);
        dynCheck->setTristate(true);
        QString checkBoxStyle("QCheckBox::indicator::unchecked {background-image : url(:/images/checkbox/20x20/checkbox_unchecked.png); }"
                              "QCheckBox::indicator::unchecked::hover {background-image : url(:/images/checkbox/20x20/checkbox_unchecked_hover.png); }"
                              "QCheckBox::indicator::unchecked::pressed {background-image : url(:/images/checkbox/20x20/checkbox_unchecked_pressed.png); }"

                              "QCheckBox::indicator::indeterminate {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate.png); }"
                              "QCheckBox::indicator::indeterminate::hover {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate_hover.png); }"
                              "QCheckBox::indicator::indeterminate::pressed {background-image : url(:/images/checkbox/20x20/checkbox_indeterminate_pressed.png); }"

                              "QCheckBox::indicator::checked {background-image : url(:/images/checkbox/20x20/checkbox_humanity_checked.png); }"
                              "QCheckBox::indicator::checked::hover {background-image : url(:/images/checkbox/20x20/checkbox_humanity_checked_hover.png); }"
                              "QCheckBox::indicator::checked::pressed {background-image : url(:/images/checkbox/20x20/checkbox_humanity_checked_pressed.png); }"

                              "QCheckBox::indicator {width: 20; height: 20 }");
        dynCheck->setStyleSheet(checkBoxStyle);
        dynCheck->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
        dynCheck->setAutoExclusive(false);
        ui->Humanity->addWidget(dynCheck);
        connect(dynCheck,&QCheckBox::stateChanged,this,&Indicators::humanityChanged);
    }
}




void Indicators::on_lockButton_toggled(bool checked)
{
    if(checked)
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/lock20x20.png")));
        QList<QSpinBox *> allSpinBoxes = this->findChildren<QSpinBox *>();
        for(auto spin: allSpinBoxes)
        {
            spin->setEnabled(false);
        }
    }
    else
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/unlock20x20.png")));
        QList<QSpinBox *> allSpinBoxes = this->findChildren<QSpinBox *>();
        for(auto spin: allSpinBoxes)
        {
            spin->setEnabled(true);
        }
    }
}

void Indicators::dynamicRemoveDots(QAbstractButton *bt)
{
    MainWindow::dynamicRemoveDots(bt);
    MainWindow::hunger = MainWindow::countDots(bt->group());
}

QJsonObject Indicators::write()
{
    QJsonObject json;
    json["hunger"] = MainWindow::countDots(ui->hungerGroup);

    QJsonObject healthDetails;
    healthDetails["modifier"] = ui->healthModifier->value();
    healthDetails["superficial"] = countIndicators(ui->Health, MainWindow::healthPool).first;
    healthDetails["agravated"] = countIndicators(ui->Health, MainWindow::healthPool).second;
    json["health"] = healthDetails;


    QJsonObject willpowerDetails;
    willpowerDetails["modifier"] = ui->wpModifier->value();
    willpowerDetails["superficial"] = countIndicators(ui->Willpower, MainWindow::willpowerPool).first;
    willpowerDetails["agravated"] = countIndicators(ui->Willpower, MainWindow::willpowerPool).second;
    json["willpower"] = willpowerDetails;

    json["humanity"] = countIndicators(ui->Humanity,10).second;
    return json;
}
