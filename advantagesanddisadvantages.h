#ifndef ADVANTAGESANDDISADVANTAGES_H
#define ADVANTAGESANDDISADVANTAGES_H

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
class AdvantagesAndDisadvantages;
}

class AdvantagesAndDisadvantages : public QWidget
{
    Q_OBJECT

public:
    explicit AdvantagesAndDisadvantages(QWidget *parent = nullptr);
    ~AdvantagesAndDisadvantages();
    QJsonObject write() const;
    void read(const QJsonObject& json);
    void clear();
public slots:
    void on_lockButton_toggled(bool checked);
private slots:
    void dynamicRemoveDots(QAbstractButton *bt);
    void lineEditHandling();

    void on_addNewWidgetButton_clicked();

    void on_deleteWidgetButton_clicked();

    void on_addNewWidgetButton_2_clicked();

    void on_deleteWidgetButton_2_clicked();


private:
    Ui::AdvantagesAndDisadvantages *ui;
    QWidget *createWidget(bool isMerit);
    QList<QWidget *> listOfFlaws;
    QList<QWidget *> listOfAdvatanges;
};

#endif // ADVANTAGESANDDISADVANTAGES_H
