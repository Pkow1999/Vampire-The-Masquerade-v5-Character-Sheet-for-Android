#include "disciplines.h"
#include "ui_disciplines.h"

#include <QAbstractButton>
#include <QPushButton>
#include "mainwindow.h"

Disciplines::Disciplines(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Disciplines)
{
    ui->setupUi(this);
    connect(ui->disciplineButtonGroup1, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicRemoveDots);
    connect(ui->disciplineButtonGroup1, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicPowersCreation);
    connect(ui->newButton, &QAbstractButton::clicked, this, &Discipline::dynamicDysciplinesCreation);
}

Disciplines::~Disciplines()
{
    delete ui;
}


void Disciplines::dynamicPowersCreation(QAbstractButton *bt)
{
//    QLayout *lay = bt->parentWidget()->layout()->itemAt(1)->layout();
    int size_ = MainWindow::countDots(bt->group());
    QWidget *disciplineWidget = bt->parentWidget();
    QLayout *lay = disciplineWidget->layout()->itemAt(1)->layout();
    if(lay->count() > size_)
    {
        while(lay->count() != size_)
        {
            lay->itemAt(lay->count() - 1)->widget()->deleteLater();
            lay->removeWidget(lay->itemAt(lay->count() - 1)->widget());
        }
    }
    if(lay->count() < size_)
    {
        while(lay->count() != size_)
        {
            lay->addWidget(new QLineEdit());
        }
    }
}

QWidget *Disciplines::createDysciplineWidget()
{
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    QPushButton *deleteButton = new QPushButton("Delete");
    deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    deleteButton->setLayoutDirection(Qt::RightToLeft);
    mainLayout->addWidget(deleteButton);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    QCheckBox *checkBox = new QCheckBox();
    QLineEdit *lineEdit = new QLineEdit();
    horizontalLayout->addWidget(checkBox);
    horizontalLayout->addWidget(lineEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QButtonGroup *groupBut = new QButtonGroup();
    groupBut->setExclusive(false);
    for(int i = 0; i < 5; ++i)
    {
        QRadioButton *but = new QRadioButton();
        groupBut->addButton(but);
        buttonsLayout->addWidget(but);
    }
    horizontalLayout->addLayout(buttonsLayout);
}

void Disciplines::dynamicRemoveDots(QAbstractButton *bt)
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
}
