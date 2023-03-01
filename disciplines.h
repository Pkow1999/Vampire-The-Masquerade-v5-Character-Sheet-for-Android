#ifndef DISCIPLINES_H
#define DISCIPLINES_H

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
class Disciplines;
}

class Disciplines : public QWidget
{
    Q_OBJECT

public:
    explicit Disciplines(QWidget *parent = nullptr);
    ~Disciplines();

private slots:
    void dynamicRemoveDots(QAbstractButton *bt);
    void dynamicPowersCreation(QAbstractButton *bt);
    void dynamicDysciplinesCreation(QAbstractButton *bt);
private:
    Ui::Disciplines *ui;
    QWidget* createDysciplineWidget();
};

#endif // DISCIPLINES_H
