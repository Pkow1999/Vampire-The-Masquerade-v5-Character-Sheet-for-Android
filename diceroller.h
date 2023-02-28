#ifndef DICEROLLER_H
#define DICEROLLER_H

#include "QPainter"
#include <QStyleOption>
#include <QWidget>

namespace Ui {
class DiceRoller;
}

class DiceRoller : public QWidget
{
    Q_OBJECT

public:
    DiceRoller(QWidget *parent = nullptr);
    DiceRoller(QWidget *parent = nullptr, const QStringList &listOfAttributes = QStringList());
    ~DiceRoller();

private:
    Ui::DiceRoller *ui;
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // DICEROLLER_H
