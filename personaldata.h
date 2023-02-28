#ifndef PERSONALDATA_H
#define PERSONALDATA_H

#include <QLineEdit>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>

namespace Ui {
class PersonalData;
}

class PersonalData : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalData(QWidget *parent = nullptr);
    ~PersonalData();

private slots:
    void lineEditHandling();
    void spinBoxHandling();
private:
    Ui::PersonalData *ui;
    void connectAllLineEdits();
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // PERSONALDATA_H