#ifndef INDICATORS_H
#define INDICATORS_H

#include <QWidget>
#include <QAbstractButton>
namespace Ui {
class Indicators;
}

class Indicators : public QWidget
{
    Q_OBJECT

public:
    explicit Indicators(QWidget *parent = nullptr);
    ~Indicators();
    QJsonObject write();
    void clear();
    void read(const QJsonObject& json);
    int getHumanity();
    int getWillpower();
public slots:
    void createWillpower();
    void createHealth();
private slots:
    void on_lockButton_toggled(bool checked);

    void dynamicRemoveDots(QAbstractButton *bt);
private:
    Ui::Indicators *ui;
    void deleteHealth(int size_);
    void deleteWP(int size_);
    QPair<int, int> countIndicators(QLayout *layout, int size_) const;
    void humanityChanged();
    void humanityGenerator();

};

#endif // INDICATORS_H
