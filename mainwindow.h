#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractButton>
#include <QButtonGroup>
#include <QGestureEvent>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QStackedWidget>
#include "attributes.h"
#include "diceroller.h"
#include "indicators.h"
#include "personaldata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    static int counter;
    static int hunger;
    static int healthPool;
    static int willpowerPool;
    static int healthModifier;
    static int healthFromAttributes;
    static int willpowerModifier;
    static int willpowerFromAttributes;
    static int humanity;
    static QString backgroundImageUrl;

    ~MainWindow();

private slots:
    void on_drawerButton_toggled(bool checked);
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_clanSymbolToggler_toggled(bool checked);

private:
    QTouchEvent *touchBegin = nullptr;
    QTouchEvent *touchEnd = nullptr;
    Ui::MainWindow *ui;
    Attributes *attributesWindow;
    DiceRoller *diceWindow;
    PersonalData *personalWindow;
    Indicators *indicatorsWindow;
    QStackedWidget *widgetStack;

    QRect geometryStack;
    bool eventFilter(QObject *obj, QEvent *event);
    bool swipeAction(QTouchEvent *begin, QTouchEvent *end);

    bool swipeRight();
    bool swipeLeft();
    void changeBackground(QString backgroundURL);
    void clearBackground();
};
#endif // MAINWINDOW_H
