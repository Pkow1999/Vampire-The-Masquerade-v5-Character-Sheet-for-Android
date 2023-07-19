#include "advantagesanddisadvantages.h"
#include "mainwindow.h"
#include "ui_advantagesanddisadvantages.h"


#include <QAbstractButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QJsonArray>

AdvantagesAndDisadvantages::AdvantagesAndDisadvantages(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvantagesAndDisadvantages)
{
    ui->setupUi(this);
    listOfAdvatanges.append(ui->Merit1);
    listOfFlaws.append(ui->Flaw1);
    connect(ui->FlawButtonGroup1, &QButtonGroup::buttonClicked, this, &AdvantagesAndDisadvantages::dynamicRemoveDots);
    connect(ui->MeritButtonGroup1, &QButtonGroup::buttonClicked, this, &AdvantagesAndDisadvantages::dynamicRemoveDots);

    connect(ui->meritLine, &QLineEdit::editingFinished, this, &AdvantagesAndDisadvantages::lineEditHandling);
    connect(ui->flawLine, &QLineEdit::editingFinished, this, &AdvantagesAndDisadvantages::lineEditHandling);


}

AdvantagesAndDisadvantages::~AdvantagesAndDisadvantages()
{
    delete ui;
}

void AdvantagesAndDisadvantages::on_lockButton_toggled(bool checked)
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
    for(auto but: allRadioButtons)
    {
        but->setEnabled(!checked);
    }
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    for(auto line: allLineEdits)
    {
        line->setEnabled(!checked);
    }
    QList<QTextEdit *> allTextEdits = this->findChildren<QTextEdit *>();
    for(auto text: allTextEdits)
    {
        text->setEnabled(!checked);
    }
    //Merits
    ui->addNewWidgetButton->setEnabled(!checked);
    ui->deleteWidgetButton->setEnabled(!checked);
    //Flaws
    ui->addNewWidgetButton_2->setEnabled(!checked);
    ui->deleteWidgetButton_2->setEnabled(!checked);

    ui->lockButton->setChecked(checked);

}


//Merits
void AdvantagesAndDisadvantages::on_addNewWidgetButton_clicked()
{
    QWidget *newMerit = createWidget(true);
    listOfAdvatanges.append(newMerit);
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(ui->Merits->layout());
    layout->insertWidget(layout->count(), newMerit);
}

void AdvantagesAndDisadvantages::on_deleteWidgetButton_clicked()
{
    if(listOfAdvatanges.count() > 1)
    {
        listOfAdvatanges.back()->deleteLater();
        listOfAdvatanges.pop_back();
    }
}
//~Merits

QWidget *AdvantagesAndDisadvantages::createWidget(bool isMerit)
{
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    //nie ma znaczenie od kogo wezmie, byle by było, obie sa takie samo
    mainLayout->setSizeConstraint(ui->Merit1->layout()->sizeConstraint());
    mainLayout->setAlignment(ui->Merit1->layout()->alignment());
    mainLayout->setSpacing(ui->Merit1->layout()->spacing());
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSizeConstraint(ui->MeritsDetail1->sizeConstraint());
    horizontalLayout->setAlignment(ui->MeritsDetail1->alignment());
    horizontalLayout->setSpacing(ui->MeritsDetail1->spacing());

    QLineEdit *lineEdit = new QLineEdit(mainWidget);
    lineEdit->setSizePolicy(ui->meritLine->sizePolicy());
    lineEdit->setFont(ui->meritLine->font());
    lineEdit->adjustSize();
    horizontalLayout->addWidget(lineEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSizeConstraint(ui->LoresheetButtonsLayout_1->sizeConstraint());
    buttonsLayout->setAlignment(ui->LoresheetButtonsLayout_1->alignment());
    buttonsLayout->setSpacing(ui->LoresheetButtonsLayout_1->spacing());
    QButtonGroup *groupBut = new QButtonGroup(this);
    groupBut->setExclusive(false);
    for(int i = 0; i < 5; ++i)
    {
        QRadioButton *but = new QRadioButton(mainWidget);
        if(isMerit)
        {
            but->setObjectName("Merits_"+ QString::number(listOfAdvatanges.count())+"_Button" + QString::number(i));
        }
        else
        {
            but->setObjectName("Flaws_"+ QString::number(listOfFlaws.count())+"_Button" + QString::number(i));
        }
        but->setSizePolicy(ui->loresheetButton->sizePolicy());
        but->setStyleSheet(ui->loresheetButton->styleSheet());
        but->setAutoExclusive(false);
        buttonsLayout->insertWidget(i, but);
        groupBut->addButton(but,i);
        but->adjustSize();
    }
    horizontalLayout->addLayout(buttonsLayout);

    QTextEdit *text = new QTextEdit(mainWidget);
    text->setSizePolicy(ui->textEdit->sizePolicy());
    text->setFont(ui->textEdit->font());
    text->setMinimumSize(ui->textEdit->minimumSize());
    text->setMaximumSize(ui->textEdit->maximumSize());
    text->adjustSize();

    mainLayout->addLayout(horizontalLayout);
    mainLayout->addWidget(text);

    connect(groupBut, &QButtonGroup::buttonClicked, this, &AdvantagesAndDisadvantages::dynamicRemoveDots);
    connect(lineEdit, &QLineEdit::editingFinished, this, &AdvantagesAndDisadvantages::lineEditHandling);

    mainWidget->adjustSize();
    return mainWidget;
}

void AdvantagesAndDisadvantages::lineEditHandling()
{
    QLineEdit *line = static_cast<QLineEdit *>(sender());
    line->clearFocus();
}

void AdvantagesAndDisadvantages::dynamicRemoveDots(QAbstractButton *bt)
{
    MainWindow::dynamicRemoveDots(bt);
}




//Flaws
void AdvantagesAndDisadvantages::on_addNewWidgetButton_2_clicked()
{
    QWidget *newFlaw = createWidget(false);
    listOfFlaws.append(newFlaw);
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(ui->Flaws->layout());
    layout->insertWidget(layout->count(), newFlaw);
}


void AdvantagesAndDisadvantages::on_deleteWidgetButton_2_clicked()
{
    if(listOfFlaws.count() > 1)
    {
        listOfFlaws.back()->deleteLater();
        listOfFlaws.pop_back();
    }
}

//~Flaws

QJsonObject AdvantagesAndDisadvantages::write() const
{
    QJsonObject merits, flaws;
    int counter = 1;
    for(auto widgetOfMerit : listOfAdvatanges)
    {
        QLineEdit *lineEdit = widgetOfMerit->findChild<QLineEdit *>();
        QString const NameOfMerit = lineEdit->text();
        QJsonObject merit;
        QLayout *layoutButton = MainWindow::findParentLayout(lineEdit)->itemAt(1)->layout();
        QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
        merit["dots"] = QString::number(MainWindow::countDots(but->group()));
        merit["content"] = widgetOfMerit->findChild<QTextEdit *>()->toPlainText();
        if(merits.contains(NameOfMerit))
        {
            merits[NameOfMerit + QString::number(counter)] = merit;
            ++counter;
        }
        else
        {
            merits[NameOfMerit] = merit;
        }
    }

    for(auto widgetOfFlaw : listOfFlaws)
    {
        QLineEdit *lineEdit = widgetOfFlaw->findChild<QLineEdit *>();
        QString const NameOfLoresheet = lineEdit->text();
        QJsonObject flaw;
        QLayout *layoutButton = MainWindow::findParentLayout(lineEdit)->itemAt(1)->layout();
        QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
        flaw["dots"] = QString::number(MainWindow::countDots(but->group()));
        flaw["content"] = widgetOfFlaw->findChild<QTextEdit *>()->toPlainText();
        if(flaws.contains(NameOfLoresheet))
        {
            flaws[NameOfLoresheet + QString::number(counter)] = flaw;
            ++counter;
        }
        else
        {
            flaws[NameOfLoresheet] = flaw;
        }
    }
    QJsonObject json;
    json["Merits"] = merits;
    json["Flaws"] = flaws;
    return json;
}
void AdvantagesAndDisadvantages::read(const QJsonObject &json)
{
    if(json.contains("Merits") && json["Merits"].isObject())
    {
        QJsonObject merits = json["Merits"].toObject();
        int sizeOfMerits = merits.count();
        for(int i = 1; i < sizeOfMerits; ++i)
        {
            on_addNewWidgetButton_clicked();
        }
        int counter = 0;
        for (auto key : merits.keys())
        {
            qDebug() << "Lecimy po kejach" << key;
            QLineEdit *lineEdit = listOfAdvatanges.at(counter)->findChild<QLineEdit *>();
            lineEdit->setText(key);
            lineEdit->setCursorPosition(0);
            QJsonObject details = merits[key].toObject();
            if(details.contains("dots") && details["dots"].isString())
            {
                int dots = details["dots"].toString().toInt();
                qDebug() << "LayoutButton";
                QLayout *layoutButton = MainWindow::findParentLayout(lineEdit)->itemAt(1)->layout();
                if(dots > 0)
                {
                    if(lineEdit == ui->meritLine)
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
                }

            }
            if(details.contains("content") && details["content"].isString())
            {
                QTextEdit *textEdit = listOfAdvatanges.at(counter)->findChild<QTextEdit *>();
                textEdit->setText(details["content"].toString());
            }
            ++counter;
        }
    }
    if(json.contains("Flaws") && json["Flaws"].isObject())
    {
        QJsonObject flaws = json["Flaws"].toObject();
        int sizeOfFlaws = flaws.count();
        for(int i = 1; i < sizeOfFlaws; ++i)
        {
            on_addNewWidgetButton_2_clicked();
        }
        int counter = 0;
        for (auto key : flaws.keys())
        {
            qDebug() << "Lecimy po kejach" << key;
            QLineEdit *lineEdit = listOfFlaws.at(counter)->findChild<QLineEdit *>();
            lineEdit->setText(key);
            lineEdit->setCursorPosition(0);
            QJsonObject details = flaws[key].toObject();
            if(details.contains("dots") && details["dots"].isString())
            {
                int dots = details["dots"].toString().toInt();
                qDebug() << "LayoutButton";
                QLayout *layoutButton = MainWindow::findParentLayout(lineEdit)->itemAt(1)->layout();
                if(dots > 0)
                {
                    if(lineEdit == ui->flawLine)
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
                }

            }
            if(details.contains("content") && details["content"].isString())
            {
                QTextEdit *textEdit = listOfFlaws.at(counter)->findChild<QTextEdit *>();
                textEdit->setText(details["content"].toString());
            }
            ++counter;
        }
    }
}


void AdvantagesAndDisadvantages::clear()
{
   for(auto radioBut : this->findChildren<QRadioButton *>())
   {
       radioBut->setChecked(false);
   }
   for(auto lineEdit : this->findChildren<QLineEdit *>())
   {
       lineEdit->clear();
   }
   for(auto textEdit : this->findChildren<QTextEdit *>())
   {
       textEdit->clear();
   }
   for(int i = 1; i < listOfAdvatanges.count(); ++i)
   {
       on_deleteWidgetButton_clicked();
   }
   for(int i = 1; i < listOfFlaws.count(); ++i)
   {
       on_deleteWidgetButton_2_clicked();
   }
}
