#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

private:
    Ui::Options *ui;
};

#endif // OPTIONS_H
