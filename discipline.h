#ifndef DISCIPLINE_H
#define DISCIPLINE_H

#include <QWidget>

namespace Ui {
class Discipline;
}

class Discipline : public QWidget
{
    Q_OBJECT

public:
    explicit Discipline(QWidget *parent = nullptr);
    ~Discipline();

private:
    Ui::Discipline *ui;
};

#endif // DISCIPLINE_H
