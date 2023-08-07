#ifndef CLANS_H
#define CLANS_H

#include <QWidget>

namespace Ui {
class Clans;
}

class Clans : public QWidget
{
    Q_OBJECT

public:
    explicit Clans(QWidget *parent = nullptr);
    ~Clans();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_clicked();

private:
    Ui::Clans *ui;
    QPixmap pic;
};

#endif // CLANS_H
