#include "diceroller.h"
#include "ui_diceroller.h"

DiceRoller::DiceRoller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiceRoller)
{
    ui->setupUi(this);

}

DiceRoller::DiceRoller(QWidget *parent, const QStringList &listOfAttributes) :
    QWidget(parent),
    ui(new Ui::DiceRoller)
{
    ui->setupUi(this);
    ui->comboBox->addItems(listOfAttributes);
}

DiceRoller::~DiceRoller()
{
    delete ui;
}
