#ifndef LORESHEETS_H
#define LORESHEETS_H

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
class Loresheets;
}

class Loresheets : public QWidget
{
    Q_OBJECT

public:
    explicit Loresheets(QWidget *parent = nullptr);
    ~Loresheets();
    QJsonObject write() const;
private slots:
    void dynamicRemoveDots(QAbstractButton *bt);
    void on_addNewWidgetButton_clicked();

    void on_deleteWidgetButton_clicked();

    void on_lockButton_toggled(bool checked);

    void lineEditHandling();
private:
    Ui::Loresheets *ui;
    QWidget* createLoreSheetWidget();
    QList<QWidget *> listOfLoresheets;

};

#endif // LORESHEETS_H
