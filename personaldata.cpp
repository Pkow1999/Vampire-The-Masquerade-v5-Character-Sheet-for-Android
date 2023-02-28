#include "personaldata.h"
#include "QScreen"
#include "ui_personaldata.h"

PersonalData::PersonalData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalData)
{
    ui->setupUi(this);
    connectAllLineEdits();
}

void PersonalData::connectAllLineEdits(){
    connect(ui->lineEdit,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_2,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_3,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_4,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_5,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_6,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);
    connect(ui->lineEdit_7,&QLineEdit::editingFinished, this, &PersonalData::lineEditHandling);

    connect(ui->spinBox,&QAbstractSpinBox::editingFinished, this, &PersonalData::spinBoxHandling);
    connect(ui->spinBox_2,&QAbstractSpinBox::editingFinished, this, &PersonalData::spinBoxHandling);

}

PersonalData::~PersonalData()
{
    delete ui;
}

void PersonalData::lineEditHandling()
{
    QLineEdit *line = static_cast<QLineEdit *>(sender());
    line->clearFocus();
}

void PersonalData::spinBoxHandling()
{
    QSpinBox *spin = static_cast<QSpinBox *>(sender());
    spin->clearFocus();
}
