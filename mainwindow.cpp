#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScroller>
#include <QScreen>
#include <QLineEdit>
#include <QTimer>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->scrollAreaWidgetContents->deleteLater();
    attributesWindow = new Attributes(nullptr);
    attributesWindow->setObjectName("Attibutes");
    diceWindow = new DiceRoller(nullptr, attributesWindow->getAttributesList());
    diceWindow->setObjectName("Dice Roller");\
    personalWindow = new PersonalData(nullptr);
    widgetStack = new QStackedWidget;
    widgetStack->addWidget(attributesWindow);
    widgetStack->addWidget(diceWindow);
    widgetStack->addWidget(personalWindow);

    widgetStack->setCurrentIndex(-1);
    ui->scrollArea->setWidget(widgetStack);
    ui->scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
    ui->gridLayout->addWidget(ui->drawerButton,0,1,Qt::AlignLeft);

    auto screenSize = qApp->screens().first()->size();
    ui->centralwidget->setMaximumSize(screenSize);

    ui->scrollArea->horizontalScrollBar()->setDisabled(true);

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    ui->listWidget->addItem("Attributes");
    ui->listWidget->addItem("Dice Roller");
    ui->listWidget->addItem("Personal Data");
    ui->listWidget->setCurrentItem(ui->listWidget->item(0));
    ui->listWidget->setVisible(false);

    ui->scrollArea->setVisible(true);

//    qApp->setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents);
    qApp->installEventFilter(this);
}


MainWindow::~MainWindow()
{
    delete widgetStack;
    delete attributesWindow;
    delete diceWindow;
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
//    if(event->type() == QEvent::KeyPress)
//    {
//        qDebug() <<"KEY PRESSSSS!";
//        QLineEdit *line = obj->findChild<QLineEdit *>();
//        if(line != nullptr){
//            line->clearFocus();
//            return true;
//        }
//        return false;
//    }
    if(event->type() == QEvent::TouchBegin)
    {
        qDebug() << "Touch begin";
        touchBegin = static_cast<QTouchEvent *>(event)->clone();
    }
    else if(event->type() == QEvent::TouchEnd)
    {
        qDebug() << "Touch begin";
        touchEnd = static_cast<QTouchEvent *>(event)->clone();
    }
    if(touchBegin != nullptr && touchEnd != nullptr)
    {
        qDebug() << "=======Touch Begin=======";
        qDebug() << "X: " << touchBegin->points().first().position().x() << "   Y: " << touchBegin->points().first().position().y();
        qDebug() << "TimeStamp: " << touchBegin->timestamp();

        qDebug() << "=======Touch End=======";
        qDebug() << "X: " << touchEnd->points().first().position().x() << "   Y: " << touchEnd->points().first().position().y();
        qDebug() << "TimeStamp: " << touchEnd->timestamp();

        return swipeAction(touchBegin, touchEnd);
    }
    return false;
}

bool MainWindow::swipeAction(QTouchEvent *begin, QTouchEvent *end)
{

    auto x_1 = begin->points().first().position().x();
    auto x_2 = end->points().first().position().x();
    auto y_1 = begin->points().first().position().y();
    auto y_2 = end->points().first().position().y();

    touchBegin = nullptr;
    touchEnd = nullptr;


    auto absY = qAbs(y_1 - y_2);
    auto absX = qAbs(x_1 - x_2);//odleglosc

    if(x_2 - x_1 > 0) // swipe w prawo
    {
        if(absY < 1) absY = 1;

        if(absX >= ui->centralwidget->width() / absY && absX >absY)
        {
            qDebug() << "~~~~~~SWIPE RIGHT DZIAŁA!!!!!~~~~~~";
            return swipeRight();
        }
    }
    else if(x_2 - x_1 < 0) // swipe w lewo
    {
        if(absY < 1) absY = 1;

        if(absX >= ui->centralwidget->width() / absY && absX >absY)
        {
            qDebug() << "~~~~~~SWIPE RIGHT DZIAŁA!!!!!~~~~~~";
            return swipeLeft();
        }
    }
    return false;
}

bool MainWindow::swipeRight()
{
    if(!ui->listWidget->isVisible())
    {
        ui->drawerButton->toggle();
        return true;
    }
    return false;
}

bool MainWindow::swipeLeft()
{
    if(ui->listWidget->isVisible())
    {
        ui->drawerButton->toggle();
        ui->scrollArea->adjustSize();

//        ui->scrollArea->horizontalScrollBar()->setValue(0);
        return true;
    }
    return false;
}

void MainWindow::on_drawerButton_toggled(bool checked)
{

    if(checked)
    {
        ui->gridLayout->addWidget(ui->drawerButton,0,0,Qt::AlignLeft);
        ui->listWidget->setVisible(true);
    }
    else
    {
        ui->gridLayout->addWidget(ui->drawerButton,0,1,Qt::AlignLeft);
        ui->listWidget->setVisible(false);
        //widgetStack->currentWidget()->adjustSize();
        ui->scrollArea->adjustSize();
    }

}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(!ui->scrollArea->isVisible())
        ui->scrollArea->setVisible(true);
    if(current != previous)
    {

        ui->scrollArea->verticalScrollBar()->setValue(0);//resetujemy wartosc scrollbara

        if(current->text() == "Attributes")
        {
            widgetStack->setCurrentWidget(attributesWindow);
            widgetStack->setMaximumHeight(ui->scrollArea->viewport()->maximumHeight());//SCROLLUJ ILE KURWA MOZESZ

        }
        else if(current->text() == "Dice Roller")
        {
            widgetStack->setCurrentWidget(diceWindow);
            widgetStack->setMaximumHeight(ui->scrollArea->viewport()->height());//TRZEBA TO KURWA USTAWIC ZEBY NIE POJAWIAL SIE SCROLL
        //czas ile na tym gownie skonczonym zmarnowalem? KURWA ZA DUZO JA PIERDOLE
        }
        else if(current->text() == "Personal Data")
        {
            widgetStack->setCurrentWidget(personalWindow);
            widgetStack->setMaximumHeight(ui->scrollArea->viewport()->height());
        }
        ui->drawerButton->setText(current->text());

        qDebug() <<"ScrollArea Size: "<<ui->scrollArea->size();
        qDebug() <<"Viewport Size: " <<ui->scrollArea->viewport()->size();
        qDebug() <<"StackedWidget Size: "<<widgetStack->size();
        qDebug() <<"Widget Size: "<<widgetStack->currentWidget()->size();
    }
}
