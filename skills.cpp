#include "skills.h"
#include "qboxlayout.h"
#include "ui_skills.h"
#include "mainwindow.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QLineEdit>
#include <QRadioButton>

Skills::Skills(QWidget *parent, const QList<QString> &listOfSkills) :
    QWidget(parent),
    ui(new Ui::Skills)
{
    ui->setupUi(this);
    mainGroupButton = new QButtonGroup(this);
    mainGroupButton->setExclusive(false);

    createDynamicWidget(listOfSkills);
    connect(mainGroupButton,&QButtonGroup::buttonToggled,this,&Skills::callBoldingFromParent);
}




Skills::~Skills()
{
    mainGroupButton->deleteLater();
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
    mainGroupButton->addButton(skillNameCheck);
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
    QButtonGroup *groupBut = new QButtonGroup(this);
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
    MainWindow::dynamicRemoveDots(bt);

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

void Skills::callBoldingFromParent(QAbstractButton *bt, bool state)
{
    MainWindow *w = dynamic_cast<MainWindow *> (this->nativeParentWidget());
    if (0 != w)
        w->bolding(bt, state, 2, 1);
}

QJsonObject Skills::write() const
{
    QList<QCheckBox *> listOfSkills = this->findChildren<QCheckBox *>();
    QJsonObject json;
    for(auto checkbox : listOfSkills)
    {
        QJsonObject details;
        QLayout *layoutButton = MainWindow::findParentLayout(checkbox)->itemAt(2)->layout();
        QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
        details["dots"] = MainWindow::countDots(but->group());

        QLineEdit *lineEdit = static_cast<QLineEdit *>(MainWindow::findParentLayout(checkbox)->itemAt(1)->widget());
        details["specializations"] = lineEdit->text();

        json[checkbox->text()] = details;
    }
    return json;
}
