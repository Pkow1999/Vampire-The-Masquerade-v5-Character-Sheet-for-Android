#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <QAbstractButton>
#include <QButtonGroup>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>

namespace Ui {
class Attributes;
}

class Attributes : public QWidget
{
    Q_OBJECT

public:
    explicit Attributes(QWidget *parent = nullptr);
    QStringList getAttributesList();
    ~Attributes();

private slots:
    void dynamicRemoveDots(QAbstractButton *bt);
    void calculateHealth();
    void calculateWP();
    void bolding(QAbstractButton *bt, bool state);
    void humanityChanged();

private:
    int counter = 0;
    int hunger = 0;
    int healthPool = 0;
    int willpowerPool = 0;
    QRect geometryStack;
    Ui::Attributes *ui;
    int countDots(QButtonGroup *grp);
    void deleteHealth(int size_);
    void deleteWP(int size_);
    QLayout *findParentLayout(QWidget *w, QLayout *topLevelLayout);
    QLayout *findParentLayout(QWidget *w);
    void connectAllButtonGroup();
    QPair<int, int> countIndicators(QLayout *layout, int size_);
    void humanityGenerator();
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // ATTRIBUTES_H
