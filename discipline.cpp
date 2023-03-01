#include "discipline.h"
#include "ui_discipline.h"

Discipline::Discipline(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Discipline)
{
    ui->setupUi(this);
}

Discipline::~Discipline()
{
    delete ui;
}
