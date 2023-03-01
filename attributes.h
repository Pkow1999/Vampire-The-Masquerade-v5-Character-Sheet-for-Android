#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <QAbstractButton>
#include <QButtonGroup>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include "indicators.h"

namespace Ui {
class Attributes;
}

class Attributes : public QWidget
{
    Q_OBJECT

public:
    explicit Attributes(QWidget *parent = nullptr, Indicators *anotherWindowPointer = nullptr);
    QStringList getAttributesList();
    void setIndicatorsPointer(Indicators *ind);
    ~Attributes();

private slots:
    void dynamicRemoveDots(QAbstractButton *bt);
    void calculateHealth();
    void calculateWP();
    void bolding(QAbstractButton *bt, bool state);
    void on_lockButton_toggled(bool checked);

private:
    int counter = 0;
    int hunger = 0;
    QRect geometryStack;
    Ui::Attributes *ui;
    Indicators *indicatorsPointer;
    int countDots(QButtonGroup *grp);
    QLayout *findParentLayout(QWidget *w, QLayout *topLevelLayout);
    QLayout *findParentLayout(QWidget *w);
    void connectAllButtonGroup();
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // ATTRIBUTES_H
