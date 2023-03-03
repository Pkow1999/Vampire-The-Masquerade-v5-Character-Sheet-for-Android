#include "disciplines.h"
#include "ui_disciplines.h"

#include <QAbstractButton>
#include <QJsonArray>
#include <QPushButton>
#include "mainwindow.h"

Disciplines::Disciplines(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Disciplines)
{

    mainGroupButton = new QButtonGroup(this);
    mainGroupButton->setExclusive(false);
    ui->setupUi(this);
    connect(ui->disciplineButtonGroup1, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicRemoveDots);
    connect(ui->disciplineButtonGroup1, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicPowersCreation);

    mainGroupButton->addButton(ui->checkBox);
    connect(mainGroupButton,&QButtonGroup::buttonToggled,this,&Disciplines::callBoldingFromParent);


    connect(ui->lineEdit_25, &QLineEdit::editingFinished, this, &Disciplines::lineEditHandling);
    listOfDysciplines.append(ui->Discipline1);
}

Disciplines::~Disciplines()
{
    for(auto wid: listOfDysciplines)
        wid->deleteLater();
    mainGroupButton->deleteLater();
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
            QLineEdit *lineEdit = new QLineEdit();
            lay->addWidget(lineEdit);
            connect(lineEdit, &QLineEdit::editingFinished, this, &Disciplines::lineEditHandling);
        }
    }
}

QWidget *Disciplines::createDysciplineWidget()
{
    QWidget *mainWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSizeConstraint(ui->Discipline1->layout()->sizeConstraint());
    mainLayout->setAlignment(ui->Discipline1->layout()->alignment());
    mainLayout->setSpacing(ui->Discipline1->layout()->spacing());
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSizeConstraint(ui->Discipline1_0->sizeConstraint());
    horizontalLayout->setAlignment(ui->Discipline1_0->alignment());
    horizontalLayout->setSpacing(ui->Discipline1_0->spacing());
    QCheckBox *checkBox = new QCheckBox(mainWidget);
    checkBox->setStyleSheet(ui->checkBox->styleSheet());
    checkBox->setSizePolicy(ui->checkBox->sizePolicy());
    checkBox->adjustSize();
    mainGroupButton->addButton(checkBox);
    QLineEdit *lineEdit = new QLineEdit(mainWidget);
    lineEdit->setSizePolicy(ui->lineEdit_25->sizePolicy());
    lineEdit->setFont(ui->lineEdit_25->font());
    lineEdit->adjustSize();
    horizontalLayout->addWidget(checkBox);
    horizontalLayout->addWidget(lineEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSizeConstraint(ui->DisciplineButtonsLayout_1->sizeConstraint());
    buttonsLayout->setAlignment(ui->DisciplineButtonsLayout_1->alignment());
    buttonsLayout->setSpacing(ui->DisciplineButtonsLayout_1->spacing());
    QButtonGroup *groupBut = new QButtonGroup(this);
    groupBut->setExclusive(false);
    for(int i = 0; i < 5; ++i)
    {
        QRadioButton *but = new QRadioButton(mainWidget);
        but->setObjectName("Discipline_"+ QString::number(listOfDysciplines.count())+"_Button" + QString::number(i));
        but->setSizePolicy(ui->disciplineButton1_1->sizePolicy());
        but->setStyleSheet(ui->disciplineButton1_1->styleSheet());
        but->setAutoExclusive(false);
        buttonsLayout->insertWidget(i, but);
        groupBut->addButton(but,i);
        but->adjustSize();
    }
    horizontalLayout->addLayout(buttonsLayout);

    QVBoxLayout *powersLayout = new QVBoxLayout();
    powersLayout->setSizeConstraint(ui->DisciplinePowers1->sizeConstraint());
    powersLayout->setAlignment(ui->DisciplinePowers1->alignment());
    powersLayout->setSpacing(ui->DisciplinePowers1->spacing());

    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(powersLayout);

    connect(groupBut, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicRemoveDots);
    connect(groupBut, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicPowersCreation);
    connect(lineEdit, &QLineEdit::editingFinished, this, &Disciplines::lineEditHandling);


    mainWidget->adjustSize();
    return mainWidget;
}

void Disciplines::dynamicRemoveDots(QAbstractButton *bt)//naprawienie sposobem tasmy klejacej
{
   MainWindow::dynamicRemoveDots(bt);
}


void Disciplines::on_addNewWidgetButton_clicked()
{
    QWidget *newDiscipline = createDysciplineWidget();
    listOfDysciplines.append(newDiscipline);
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(this->layout());
    layout->insertWidget(layout->count() - 2, newDiscipline);
}


void Disciplines::on_deleteWidgetButton_clicked()
{
    if(listOfDysciplines.count() > 1)
    {
        QWidget *widgetToDelete = listOfDysciplines.back();
        QLineEdit *lineEdit = widgetToDelete->findChild<QLineEdit *>();
        MainWindow::mapOfSkillsWithValue.remove(lineEdit->text());
        listOfDysciplines.back()->deleteLater();
        listOfDysciplines.pop_back();
    }
}


void Disciplines::on_lockButton_toggled(bool checked)
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
    ui->addNewWidgetButton->setEnabled(!checked);
    ui->deleteWidgetButton->setEnabled(!checked);
}

void Disciplines::lineEditHandling()
{
    QLineEdit *line = static_cast<QLineEdit *>(sender());
    line->clearFocus();
}

void Disciplines::callBoldingFromParent(QAbstractButton *bt, const bool& state)
{
    MainWindow *w = dynamic_cast<MainWindow *> (this->nativeParentWidget());
    if (0 != w)
        w->bolding(bt, state, 2, 2);
}


QJsonObject Disciplines::write() const
{
    QJsonObject json;
    int counter = 1;
    for(auto widgetOfDiscipline : listOfDysciplines)
    {
        QLineEdit *lineEdit = widgetOfDiscipline->findChild<QLineEdit *>();
        QString const NameOfDiscipline = lineEdit->text();
        QJsonObject discipline;
        QLayout *layoutButton = MainWindow::findParentLayout(lineEdit)->itemAt(2)->layout();

        QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
        discipline["dots"] = QString::number(MainWindow::countDots(but->group()));
        QJsonArray powers;
        for(auto powersLineEdits : widgetOfDiscipline->findChildren<QLineEdit *>())
        {
            if(powersLineEdits != lineEdit)
            {
                powers.append(powersLineEdits->text());
            }
        }
        discipline["powers"] = powers;
        if(json.contains(NameOfDiscipline))
        {
            json[NameOfDiscipline + QString::number(counter)] = discipline;
            ++counter;
        }
        else
        {
            json[NameOfDiscipline] = discipline;
        }

    }
    return json;
}

void Disciplines::read(const QJsonObject &json)
{
    int size_ = json.count();
    for(int i = 1; i < size_; ++i)
    {
        on_addNewWidgetButton_clicked();
    }
    int counter = 0;
    for (auto key : json.keys())
    {
        qDebug() << "Lecimy po kejach";
        QLineEdit *lineEdit = listOfDysciplines.at(counter)->findChild<QLineEdit *>();
        lineEdit->setText(key);
        QJsonObject details = json[key].toObject();
        if(details.contains("dots") && details["dots"].isString())
        {
            int dots = details["dots"].toString().toInt();
            qDebug() << "LayoutButton";
            QLayout *layoutButton = MainWindow::findParentLayout(lineEdit)->itemAt(2)->layout();
            if(dots > 0)
            {
                if(lineEdit == ui->lineEdit_25)
                {
                    dots = -dots - 1;
                }
                else
                {
                    --dots;
                }
                QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
                but->group()->button(dots)->click();
                qDebug() << "Kliknalem w odpowiednie buttony";
                if(details.contains("powers") && details["powers"].isArray())
                {
                    QJsonArray powers = details["powers"].toArray();
                    int counterPowers = 0;
                    for(auto powersLineEdits : listOfDysciplines.at(counter)->findChildren<QLineEdit *>())
                    {
                        if(powersLineEdits != lineEdit)
                        {
                            powersLineEdits->setText(powers.at(counterPowers).toString());
                            ++counterPowers;
                        }

                    }
                }
            }

        }
        ++counter;
    }
}

void Disciplines::clear()
{
    qDebug() << "Clear begin";

   for(auto radioBut : this->findChildren<QRadioButton *>())
   {
       radioBut->setChecked(false);
   }
   for(auto lineEdit : this->findChildren<QLineEdit *>())
   {
       lineEdit->clear();
   }
   for(int i = 1; i < listOfDysciplines.count(); ++i)
   {
       on_deleteWidgetButton_clicked();
   }
   qDebug() <<"Clear end";
}
