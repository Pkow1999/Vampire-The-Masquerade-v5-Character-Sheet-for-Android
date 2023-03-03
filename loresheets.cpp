#include "loresheets.h"
#include "mainwindow.h"
#include "ui_loresheets.h"


#include <QAbstractButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QJsonArray>

Loresheets::Loresheets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Loresheets)
{
    ui->setupUi(this);


    connect(ui->loresheetsButtonGroup, &QButtonGroup::buttonClicked, this, &Loresheets::dynamicRemoveDots);

    connect(ui->loresheetLine, &QLineEdit::editingFinished, this, &Loresheets::lineEditHandling);
    listOfLoresheets.append(ui->Loresheet1);

}

Loresheets::~Loresheets()
{
    for(auto wid: listOfLoresheets)
        wid->deleteLater();
    delete ui;
}


QWidget *Loresheets::createLoreSheetWidget()
{
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSizeConstraint(ui->Loresheet1->layout()->sizeConstraint());
    mainLayout->setAlignment(ui->Loresheet1->layout()->alignment());
    mainLayout->setSpacing(ui->Loresheet1->layout()->spacing());
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSizeConstraint(ui->LoresheetDetails1->sizeConstraint());
    horizontalLayout->setAlignment(ui->LoresheetDetails1->alignment());
    horizontalLayout->setSpacing(ui->LoresheetDetails1->spacing());
    QLineEdit *lineEdit = new QLineEdit(mainWidget);
    lineEdit->setSizePolicy(ui->loresheetLine->sizePolicy());
    lineEdit->setFont(ui->loresheetLine->font());
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
        but->setObjectName("Loresheet_"+ QString::number(listOfLoresheets.count())+"_Button" + QString::number(i));
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

    connect(groupBut, &QButtonGroup::buttonClicked, this, &Loresheets::dynamicRemoveDots);
    connect(lineEdit, &QLineEdit::editingFinished, this, &Loresheets::lineEditHandling);

    mainWidget->adjustSize();
    return mainWidget;
}

void Loresheets::dynamicRemoveDots(QAbstractButton *bt)
{
    MainWindow::dynamicRemoveDots(bt);
}


void Loresheets::on_addNewWidgetButton_clicked()
{
    QWidget *newLoresheet = createLoreSheetWidget();
    listOfLoresheets.append(newLoresheet);
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(this->layout());
    layout->insertWidget(layout->count() - 2, newLoresheet);
}


void Loresheets::on_deleteWidgetButton_clicked()
{
    if(listOfLoresheets.count() > 1)
    {
        listOfLoresheets.back()->deleteLater();
        listOfLoresheets.pop_back();
    }
}


void Loresheets::on_lockButton_toggled(bool checked)
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
    ui->addNewWidgetButton->setEnabled(!checked);
    ui->deleteWidgetButton->setEnabled(!checked);
}

void Loresheets::lineEditHandling()
{
    QLineEdit *line = static_cast<QLineEdit *>(sender());
    line->clearFocus();
}

QJsonObject Loresheets::write() const
{
    QJsonObject json;
    int counter = 1;
    for(auto widgetOfLoresheet : listOfLoresheets)
    {
        QLineEdit *lineEdit = widgetOfLoresheet->findChild<QLineEdit *>();
        QString const NameOfLoresheet = lineEdit->text();
        QJsonObject loresheet;
        QLayout *layoutButton = MainWindow::findParentLayout(lineEdit)->itemAt(1)->layout();
        QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
        loresheet["dots"] = MainWindow::countDots(but->group());
        loresheet["content"] = widgetOfLoresheet->findChild<QTextEdit *>()->toPlainText();
        if(json.contains(NameOfLoresheet))
        {
            json[NameOfLoresheet + QString::number(counter)] = loresheet;
            ++counter;
        }
        else
        {
            json[NameOfLoresheet] = loresheet;
        }
    }
    return json;
}
