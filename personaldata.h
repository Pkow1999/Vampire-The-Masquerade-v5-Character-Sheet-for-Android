#ifndef PERSONALDATA_H
#define PERSONALDATA_H

#include <QAbstractButton>
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
    QLineEdit* getClanLineEdit();

    ~PersonalData();

    QJsonObject write() const;
    void read(const QJsonObject& json);

    void clear();
private slots:
    void lineEditHandling();
    void spinBoxHandling();
    void clanHandle();
    void calculateBlood();
    void dynamicRemoveDots(QAbstractButton *bt);
private:
    Ui::PersonalData *ui;
    QList<QString> listOfClans;

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
