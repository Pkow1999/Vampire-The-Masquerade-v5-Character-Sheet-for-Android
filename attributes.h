#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <QAbstractButton>
#include <QButtonGroup>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include "indicators.h"
#include <QJsonObject>

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
    QJsonObject write() const;
    void read(const QJsonObject& json);
    void clear();

    ~Attributes();

private slots:
    void dynamicRemoveDots(QAbstractButton *bt);
    void calculateHealth();
    void calculateWP();
    void on_lockButton_toggled(bool checked);
    void callBoldingFromParent(QAbstractButton *bt, bool state);
private:
    int counter = 0;
    int hunger = 0;
    QRect geometryStack;
    Ui::Attributes *ui;
    Indicators *indicatorsPointer;
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
