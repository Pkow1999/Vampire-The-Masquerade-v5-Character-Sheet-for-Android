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
    DiceRoller(QWidget *parent = nullptr, const QStringList &listOfAttributes = QStringList(), const QStringList &listOfSkills = QStringList());
    ~DiceRoller();

private slots:
    void on_pushButton_clicked();

    void on_rollButton_clicked();

    void on_rerollButton_clicked();

private:
    Ui::DiceRoller *ui;
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    void deleteDices(int size_);
    void createDices(int size_);
};

#endif // DICEROLLER_H
