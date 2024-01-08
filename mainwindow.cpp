#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScroller>
#include <QScreen>
#include <QLineEdit>
#include <QTimer>
#include <QScrollBar>
#include <QLabel>
#include <QCheckBox>

#include <QStandardPaths>
#include <QtCore/private/qandroidextras_p.h>

int MainWindow::counter = 0;
int MainWindow::hunger = 0;
int MainWindow::healthPool = 0;
int MainWindow::willpowerPool = 0;
int MainWindow::healthModifier = 0;
int MainWindow::healthFromAttributes = 0;
int MainWindow::willpowerModifier = 0;
int MainWindow::willpowerFromAttributes = 0;
int MainWindow::humanity = 0;
QString MainWindow::backgroundImageUrl = "";
QMap<QString, int> MainWindow::mapOfSkillsWithValue = QMap<QString, int>();
QMap<QString, int> MainWindow::ALLmapOfSkillsWithValue = QMap<QString, int>();



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->deleteLater();

    generatePages();

    ui->scrollArea->setWidget(widgetStack);
    ui->scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
    ui->gridLayout->addWidget(ui->drawerButton,0,1,Qt::AlignLeft);
    widgetStack->setMaximumHeight(ui->scrollArea->viewport()->maximumHeight());//making scrollable possible
    widgetStack->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto screenSize = qApp->screens().first()->size();
    ui->centralwidget->setMaximumSize(screenSize);

    ui->scrollArea->horizontalScrollBar()->setDisabled(true);

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    ui->listWidget->setCurrentItem(ui->listWidget->item(0));
    ui->listWidget->setVisible(false);

    ui->scrollArea->setVisible(true);

//    connect(personalWindow->getClanLineEdit(), &QLineEdit::editingFinished, this, &MainWindow::changeBackground);


//    qApp->setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents);
    qApp->installEventFilter(this);
}


MainWindow::~MainWindow()
{
    delete widgetStack;
    delete attributesWindow;
    delete diceWindow;
    delete indicatorsWindow;
    delete personalWindow;
    delete disciplineWindow;
    delete ui;
}

void MainWindow::changeBackground(QString backgroundURL)
{
    QString styleSheet = "QStackedWidget{background-image: url("+ backgroundURL + ");background-position: center;background-repeat: no-repeat;}";
    qDebug() << "Style Sheet: " << styleSheet;
    ui->scrollArea->setStyleSheet(styleSheet);
}

void MainWindow::clearBackground()
{
    ui->scrollArea->setStyleSheet("");
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
        //qDebug() << "Touch begin";
        touchBegin = static_cast<QTouchEvent *>(event)->clone();
    }
    else if(event->type() == QEvent::TouchEnd)
    {
        //qDebug() << "Touch begin";
        touchEnd = static_cast<QTouchEvent *>(event)->clone();
    }
    if(touchBegin != nullptr && touchEnd != nullptr)
    {
        //qDebug() << "=======Touch Begin=======";
        //qDebug() << "X: " << touchBegin->points().first().position().x() << "   Y: " << touchBegin->points().first().position().y();
        //qDebug() << "TimeStamp: " << touchBegin->timestamp();

        //qDebug() << "=======Touch End=======";
        //qDebug() << "X: " << touchEnd->points().first().position().x() << "   Y: " << touchEnd->points().first().position().y();
        //qDebug() << "TimeStamp: " << touchEnd->timestamp();

        return swipeAction(touchBegin, touchEnd);
    }
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Back)
        {
            ui->drawerButton->click();
            return true;
        }

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

void MainWindow::lockedData(const bool& checked)
{
    attributesWindow->on_lockButton_toggled(checked);
    disciplineWindow->on_lockButton_toggled(checked);
    indicatorsWindow->on_lockButton_toggled(checked);
    loresheetsWindow->on_lockButton_toggled(checked);
    physicalSkillsWindow->on_lockButton_toggled(checked);
    socialSkillsWindow->on_lockButton_toggled(checked);
    mentalSkillsWindow->on_lockButton_toggled(checked);
    meritsAndFlawsWindow->on_lockButton_toggled(checked);
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
        for(int i = 0; i < widgetStack->count(); ++i)
        {
            if(widgetStack->widget(i)->objectName() == current->text())
            {
                widgetStack->setCurrentWidget(widgetStack->widget(i));
                qDebug() << "~~~~~~WIDGET STACK SIZE POLICY~~~~~~~~";
                qDebug() << widgetStack->sizePolicy();
                if(widgetStack->widget(i)->objectName() ==  "Disciplines" || widgetStack->widget(i)->objectName() ==  "Loresheets" || widgetStack->widget(i)->objectName() == "Merits and Flaws")
                {
                    widgetStack->setMaximumHeight(ui->scrollArea->viewport()->maximumHeight());//making scrollable possible
                }
                else
                {
                    widgetStack->setMaximumHeight(ui->scrollArea->viewport()->height());//making no scrollable
                }
                ui->drawerButton->setText(current->text());
                return;
            }
        }

//        if(current->text() == "Attributes")
//        {
//            widgetStack->setCurrentWidget(attributesWindow);
//            //widgetStack->setMaximumHeight(ui->scrollArea->viewport()->maximumHeight());//SCROLLUJ ILE KURWA MOZESZ

//        }
//        else if(current->text() == "Dice Roller")
//        {
//            widgetStack->setCurrentWidget(diceWindow);
//            //widgetStack->setMaximumHeight(ui->scrollArea->viewport()->height());//TRZEBA TO KURWA USTAWIC ZEBY NIE POJAWIAL SIE SCROLL
//        //czas ile na tym gownie skonczonym zmarnowalem? KURWA ZA DUZO JA PIERDOLE
//        }
//        else if(current->text() == "Personal Data")
//        {
//            widgetStack->setCurrentWidget(personalWindow);
//            //widgetStack->setMaximumHeight(ui->scrollArea->viewport()->height());
//        }
//        else if(current->text() == "Indicators")
//        {
//            widgetStack->setCurrentWidget(indicatorsWindow);
//            //widgetStack->setMaximumHeight(ui->scrollArea->viewport()->height());
//        }
//        ui->drawerButton->setText(current->text());

//        qDebug() <<"ScrollArea Size: "<<ui->scrollArea->size();
//        qDebug() <<"Viewport Size: " <<ui->scrollArea->viewport()->size();
//        qDebug() <<"StackedWidget Size: "<<widgetStack->size();
//        qDebug() <<"Widget Size: "<<widgetStack->currentWidget()->size();
    }
}



void MainWindow::on_clanSymbolToggler_toggled(bool checked)
{
    if(checked)
    {
        if(MainWindow::backgroundImageUrl != "")
            changeBackground(MainWindow::backgroundImageUrl);
    }
    else
    {
        clearBackground();
    }



}

void MainWindow::generatePages()
{
    QStringList physicalSkills = {"Athletics", "Brawl", "Craft", "Drive", "Firearms", "Larceny", "Melee", "Stealth", "Survival"};
    QStringList socialSkills = {"Animal Ken", "Etiquette", "Insight", "Intimidation", "Leadership", "Performance", "Persuasion", "Streetwise", "Subterfuge"};
    QStringList mentalSkills = {"Academics", "Awareness", "Finance", "Investigation", "Medicine", "Occult", "Politics", "Science", "Technology"};

    QStringList allSkills = {""};
    allSkills.append(physicalSkills);
    allSkills.append(socialSkills);
    allSkills.append(mentalSkills);

    attributesWindow = new Attributes(this);
    attributesWindow->setObjectName("Attributes");


    indicatorsWindow = new Indicators(this);
    indicatorsWindow->setObjectName("Indicators");
    attributesWindow->setIndicatorsPointer(indicatorsWindow);

    diceWindow = new DiceRoller(this, attributesWindow->getAttributesList(), allSkills, indicatorsWindow);
    diceWindow->setObjectName("Dice Roller");

    personalWindow = new PersonalData(this);
    personalWindow->setObjectName("Personal Data");


    disciplineWindow = new Disciplines(this);
    disciplineWindow->setObjectName("Disciplines");

    loresheetsWindow = new Loresheets(this);
    loresheetsWindow->setObjectName("Loresheets");

    meritsAndFlawsWindow = new AdvantagesAndDisadvantages(this);
    meritsAndFlawsWindow->setObjectName("Merits and Flaws");

    physicalSkillsWindow = new Skills(this, physicalSkills);
    physicalSkillsWindow->setObjectName("Physical Skills");

    socialSkillsWindow = new Skills(this, socialSkills);
    socialSkillsWindow->setObjectName("Social Skills");

    mentalSkillsWindow = new Skills(this, mentalSkills);
    mentalSkillsWindow->setObjectName("Mental Skills");

    clanWindow = new Clans(this);
    clanWindow->setObjectName("Clans");

    optionsWindow = new Options(this);
    optionsWindow->setObjectName("Options");



    widgetStack = new QStackedWidget;
    widgetStack->addWidget(attributesWindow);
    widgetStack->addWidget(physicalSkillsWindow);
    widgetStack->addWidget(socialSkillsWindow);
    widgetStack->addWidget(mentalSkillsWindow);
    widgetStack->addWidget(disciplineWindow);
    widgetStack->addWidget(diceWindow);
    widgetStack->addWidget(personalWindow);
    widgetStack->addWidget(indicatorsWindow);
    widgetStack->addWidget(loresheetsWindow);
    widgetStack->addWidget(meritsAndFlawsWindow);
    widgetStack->addWidget(clanWindow);
    widgetStack->addWidget(optionsWindow);

    for(int i = 0; i < widgetStack->count(); ++i)
    {
        QString name = widgetStack->widget(i)->objectName();
        ui->listWidget->addItem(name);
    }
    ui->listWidget->setCurrentItem(ui->listWidget->item(0));

}


QLayout* MainWindow::findParentLayout(QWidget* w)
{
    if (w->parentWidget() != nullptr)
        if (w->parentWidget()->layout() != nullptr)
            return findParentLayout(w, w->parentWidget()->layout());
    return nullptr;
}

QLayout* MainWindow::findParentLayout(QWidget* w, QLayout* topLevelLayout)
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

QJsonObject MainWindow::getSaveData()
{
    QJsonObject Mainjson;

    QJsonObject jsonDiscp = disciplineWindow->write();
    QJsonObject jsonAtr = attributesWindow->write();
    QJsonObject jsonPhys = physicalSkillsWindow->write();
    QJsonObject jsonMent = mentalSkillsWindow->write();
    QJsonObject jsonSoc = socialSkillsWindow->write();
    QJsonObject jsonInd = indicatorsWindow->write();
    QJsonObject jsonLore = loresheetsWindow->write();
    QJsonObject jsonPersonal = personalWindow->write();
    QJsonObject jsonMeritsAndFlaws = meritsAndFlawsWindow->write();

    Mainjson["Attributes"] = jsonAtr;
    Mainjson["Disciplines"] = jsonDiscp;
    Mainjson["Physical Skills"] = jsonPhys;
    Mainjson["Social Skills"] = jsonSoc;
    Mainjson["Mental Skills"] = jsonMent;
    Mainjson["Indicators"] = jsonInd;
    Mainjson["Loresheets"] = jsonLore;
    Mainjson["Personal Data"] = jsonPersonal;
    Mainjson["Merits and Flaws"] = jsonMeritsAndFlaws;
    qDebug() << Mainjson;
    return Mainjson;
}

void MainWindow::readSaveData(const QJsonObject &json)
{
    lockedData(false);
    if(json.contains("Attributes") && json["Attributes"].isObject())
    {
        qDebug() << "JSON MA ATRYBUTY!!!";
        qDebug() <<json["Attributes"];
        attributesWindow->clear();
        attributesWindow->read(json["Attributes"].toObject());
    }
    if(json.contains("Physical Skills") && json["Physical Skills"].isObject())
    {
        qDebug() << "Json ma fizyczne skilles";
        qDebug() <<json["Physical Skills"];
        physicalSkillsWindow->clear();
        physicalSkillsWindow->read(json["Physical Skills"].toObject());

    }
    if(json.contains("Mental Skills") && json["Mental Skills"].isObject())
    {
        qDebug() << "Json ma Mentalne skilles";
        qDebug() <<json["Mental Skills"];
        mentalSkillsWindow->clear();
        mentalSkillsWindow->read(json["Mental Skills"].toObject());

    }
    if(json.contains("Social Skills") && json["Social Skills"].isObject())
    {
        qDebug() << "Json ma Social skilles";
        qDebug() <<json["Social Skills"];
        socialSkillsWindow->clear();
        socialSkillsWindow->read(json["Social Skills"].toObject());

    }
    if(json.contains("Personal Data") && json["Personal Data"].isObject())
    {
        qDebug() << "Json ma Personal Data";
        qDebug() <<json["Personal Data"];
        personalWindow->clear();
        personalWindow->read(json["Personal Data"].toObject());

    }
    if(json.contains("Indicators") && json["Indicators"].isObject())
    {
        qDebug() << "Json ma Indicators";
        qDebug() <<json["Indicators"];
        indicatorsWindow->clear();
        indicatorsWindow->read(json["Indicators"].toObject());

    }
    if(json.contains("Disciplines") && json["Disciplines"].isObject())
    {
        qDebug() << "Json ma Disciplines";
        qDebug() <<json["Disciplines"];
        disciplineWindow->clear();
        disciplineWindow->read(json["Disciplines"].toObject());
    }
    if(json.contains("Loresheets") && json["Loresheets"].isObject())
    {
        qDebug() << "Json ma Loresheets";
        qDebug() <<json["Loresheets"];
        loresheetsWindow->clear();
        loresheetsWindow->read(json["Loresheets"].toObject());
    }
    if(json.contains("Merits and Flaws") && json["Merits and Flaws"].isObject())
    {
        qDebug() << "Json ma Merits and Flaws";
        qDebug() <<json["Merits and Flaws"];
        meritsAndFlawsWindow->clear();
        meritsAndFlawsWindow->read(json["Merits and Flaws"].toObject());
    }
    lockedData(true);
}

void MainWindow::bolding(QAbstractButton *bt, bool state, int index, int type)
{
    int indexOfText = 0;
    if(type != 1)
        ++indexOfText;
    QWidget *widget = findParentLayout(bt)->itemAt(indexOfText)->widget();
    QFont font = widget->font();
    font.setBold(state);
    widget->setFont(font);
    QString text;
    qDebug() << "BEFORE TYPE BOLDING";
    if(type == 0)
    {
        QLabel *label = static_cast<QLabel *>(widget);
        text = label->text();
    }
    else if(type == 1)
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(widget);
        text = checkBox->text();
    }
    else if(type == 2)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit *>(widget);
        text = lineEdit->text();
    }
    else
    {
        qWarning() << "WRONG TYPE VALUE!!!!!!";
        return;
    }
    qDebug() << "BEFORE STATE BOLDING";
    if(state)
    {
        QLayout *layoutButton = findParentLayout(bt)->itemAt(index)->layout();
        QAbstractButton *but = static_cast<QAbstractButton *>(layoutButton->itemAt(0)->widget());
        MainWindow::mapOfSkillsWithValue.insert(text, MainWindow::countDots(but->group()));
    }
    else
    {
        MainWindow::mapOfSkillsWithValue.remove(text);
    }
    diceWindow->refreshText();
}

int MainWindow::countDots(QButtonGroup *grp)
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
void MainWindow::dynamicRemoveDots(QAbstractButton *bt)//naprawienie sposobem tasmy klejacej
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

bool MainWindow::checkFilesPermission()
{
    auto r = QtAndroidPrivate::checkPermission(QtAndroidPrivate::Storage).result();
    qDebug() << "PERMISJA: " << r;
    if (r == QtAndroidPrivate::Denied)
    {

        r = QtAndroidPrivate::requestPermission(QtAndroidPrivate::Storage).result();
        qDebug() << "PRÓBA NR 2: " << r;

        if (r == QtAndroidPrivate::Denied)
            return false;
    }
    return true;
}
