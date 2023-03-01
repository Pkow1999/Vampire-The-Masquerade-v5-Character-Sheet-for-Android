#ifndef INDICATORS_H
#define INDICATORS_H

#include <QWidget>

namespace Ui {
class Indicators;
}

class Indicators : public QWidget
{
    Q_OBJECT

public:
    explicit Indicators(QWidget *parent = nullptr);
    ~Indicators();
public slots:
    void createWillpower();
    void createHealth();
private slots:
    void on_lockButton_toggled(bool checked);

private:
    Ui::Indicators *ui;
    void deleteHealth(int size_);
    void deleteWP(int size_);
    QPair<int, int> countIndicators(QLayout *layout, int size_);
    void humanityChanged();
    void humanityGenerator();

};

#endif // INDICATORS_H
