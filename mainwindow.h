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
#include "personaldata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
private slots:
    void on_drawerButton_toggled(bool checked);
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    QTouchEvent *touchBegin = nullptr;
    QTouchEvent *touchEnd = nullptr;
    Ui::MainWindow *ui;
    Attributes *attributesWindow;
    DiceRoller *diceWindow;
    PersonalData *personalWindow;
    QStackedWidget *widgetStack;

    int counter = 0;
    int hunger = 0;
    int healthPool = 0;
    int willpowerPool = 0;
    QRect geometryStack;
    bool eventFilter(QObject *obj, QEvent *event);
    bool swipeAction(QTouchEvent *begin, QTouchEvent *end);

    bool swipeRight();
    bool swipeLeft();
};
#endif // MAINWINDOW_H
