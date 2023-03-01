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
    void on_addNewWidgetButton_clicked();

    void on_deleteWidgetButton_clicked();

    void on_lockButton_toggled(bool checked);

    void lineEditHandling();
private:
    Ui::Disciplines *ui;
    QWidget* createDysciplineWidget();
    QList<QWidget *> listOfDysciplines;
};

#endif // DISCIPLINES_H
