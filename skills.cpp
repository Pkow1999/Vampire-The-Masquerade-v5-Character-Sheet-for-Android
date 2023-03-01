#include "skills.h"
#include "qboxlayout.h"
#include "ui_skills.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QLineEdit>
#include <QRadioButton>

Skills::Skills(QWidget *parent, const QList<QString> &listOfSkills) :
    QWidget(parent),
    ui(new Ui::Skills)
{
    ui->setupUi(this);
    createDynamicWidget(listOfSkills);
}




Skills::~Skills()
{
    delete ui;
}
void Skills::createDynamicWidget(const QList<QString> &listOfSkills)
{
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(this->layout());
    for(auto skill : listOfSkills)
    {
        layout->insertLayout(layout->count(), createSkill(skill));
    }
}

QHBoxLayout* Skills::createSkill(QString nameOfSkill)
{
    QHBoxLayout *skillLayout = new QHBoxLayout();
    skillLayout->setSizeConstraint(QHBoxLayout::SizeConstraint::SetDefaultConstraint);
    skillLayout->setSpacing(6);

    QCheckBox *skillNameCheck = new QCheckBox(this);
    skillNameCheck->setText(nameOfSkill);
    QLineEdit *specializationLine = new QLineEdit(this);
    connect(specializationLine, &QLineEdit::editingFinished, this, &Skills::lineEditHandling);
    QHBoxLayout *buttons = createButtons(nameOfSkill);

    skillLayout->addWidget(skillNameCheck);
    skillLayout->addWidget(specializationLine);
    skillLayout->addLayout(buttons);
    return skillLayout;
}

QHBoxLayout* Skills::createButtons(QString nameOfSkill)
{
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSizeConstraint(QHBoxLayout::SizeConstraint::SetFixedSize);
    buttonsLayout->setSpacing(6);
    QButtonGroup *groupBut = new QButtonGroup();
    groupBut->setExclusive(false);
    for(int i = 0; i < 5; ++i)
    {
        QRadioButton *but = new QRadioButton(this);
        but->setObjectName("Skill_"+ nameOfSkill +"_Button" + QString::number(i));
        but->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        //but->setStyleSheet(ui->disciplineButton1_1->styleSheet());
        but->setAutoExclusive(false);
        buttonsLayout->insertWidget(i, but);
        groupBut->addButton(but,i);
        but->adjustSize();
    }
    connect(groupBut, &QButtonGroup::buttonClicked, this, &Skills::dynamicRemoveDots);
    return buttonsLayout;
}
void Skills::dynamicRemoveDots(QAbstractButton *bt)
{
    if(bt->group()->id(bt) < -1)//automatycznie
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
    else if(bt->group()->id(bt) == -1)
    {
        qDebug() << "COS POSZLO NIE TAK";
        return;
    }
    else//recznie przydzielione
    {
        qDebug() << "RECZNIE";
        if(bt->isChecked())
        {
            for(int i = 0; i < bt->group()->buttons().size(); i++)
            {
                if(bt->group()->button(i)->objectName() == bt->objectName())
                    break;
                bt->group()->button(i)->setChecked(true);
            }
        }
        else
        {
            int del = 0;
            for(int i = 0; i < bt->group()->buttons().size(); i++)
            {
                if(bt->group()->button(i)->objectName() == bt->objectName())
                {
                    del = i;
                    break;
                }
            }
            for(int i = del; i < bt->group()->buttons().size(); i++)
            {
                bt->group()->button(i)->setChecked(false);
            }
        }
    }
}
void Skills::on_lockButton_toggled(bool checked)
{
    if(checked)
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/lock20x20.png")));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/unlock20x20.png")));
    }

    QList<QRadioButton *> allRadioButtons = this->findChildren<QRadioButton *>();
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    for(auto line: allLineEdits)
    {
        line->setEnabled(!checked);
    }
    for(auto but: allRadioButtons)
    {
        but->setEnabled(!checked);
    }
}



void Skills::lineEditHandling()
{
    QLineEdit *line = static_cast<QLineEdit *>(sender());
    line->clearFocus();
}
