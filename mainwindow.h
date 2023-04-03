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
#include "options.h"
#include "personaldata.h"
#include "disciplines.h"
#include "loresheets.h"
#include "skills.h"
#include "advantagesanddisadvantages.h"

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
    static QMap<QString, int> mapOfSkillsWithValue;
    static QMap<QString, int> ALLmapOfSkillsWithValue;
    static bool checkFilesPermission();
    ~MainWindow();
    void bolding(QAbstractButton *bt, bool state, int index, int type = 0);
    static void dynamicRemoveDots(QAbstractButton *bt);
    static int countDots(QButtonGroup *grp);
    static QLayout *findParentLayout(QWidget *w);
    static QLayout *findParentLayout(QWidget *w, QLayout *topLevelLayout);

    QJsonObject getSaveData();
    void readSaveData(const QJsonObject &json);
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
    Disciplines *disciplineWindow;
    Loresheets *loresheetsWindow;
    AdvantagesAndDisadvantages *meritsAndFlawsWindow;
    Skills *physicalSkillsWindow;
    Skills *socialSkillsWindow;
    Skills *mentalSkillsWindow;
    Options *optionsWindow;
    QStackedWidget *widgetStack;

    void generatePages();
    bool eventFilter(QObject *obj, QEvent *event);
    bool swipeAction(QTouchEvent *begin, QTouchEvent *end);
    bool swipeRight();
    bool swipeLeft();
    void changeBackground(QString backgroundURL);
    void clearBackground();
    void lockedData(const bool &checked);
};
#endif // MAINWINDOW_H
