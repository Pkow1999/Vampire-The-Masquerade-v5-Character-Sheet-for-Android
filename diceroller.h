#ifndef DICEROLLER_H
#define DICEROLLER_H

#include "QPainter"
#include <QStyleOption>
#include <QWidget>
#include "indicators.h"
namespace Ui {
class DiceRoller;
}

class DiceRoller : public QWidget
{
    Q_OBJECT

public:
    DiceRoller(QWidget *parent = nullptr);
    DiceRoller(QWidget *parent = nullptr, const QStringList &listOfAttributes = QStringList(), const QStringList &listOfSkills = QStringList(), Indicators *indicatorPointer = nullptr);
    void refreshText();
    ~DiceRoller();

private slots:
    void on_pushButton_clicked();

    void on_rollButton_clicked();

    void on_rerollButton_clicked();

    void spinBoxHandling();
    void on_frenzyRollButton_clicked();

    void on_rouseRollButton_clicked();

    void on_clearDices_clicked();

private:
    Ui::DiceRoller *ui;
    Indicators *indicatorPoint;
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    int lastFrenzyDiceRoll = 0;
    void deleteDices(int size_);
    void createDices(int size_);
    void deleteFrenzyRouseDices(int size_);
};

#endif // DICEROLLER_H
