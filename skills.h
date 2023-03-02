#ifndef SKILLS_H
#define SKILLS_H

#include <QAbstractButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QHBoxLayout>


namespace Ui {
class Skills;
}

class Skills : public QWidget
{
    Q_OBJECT

public:
    explicit Skills(QWidget *parent = nullptr, const QStringList &listOfSkills = QStringList());
    ~Skills();
private slots:
    void on_lockButton_toggled(bool checked);
private:
    void dynamicRemoveDots(QAbstractButton *bt);
    void lineEditHandling();
private:
    Ui::Skills *ui;
    void createDynamicWidget(const QList<QString> &listOfSkills);
    QHBoxLayout *createButtons(QString nameOfSkill);
    QHBoxLayout *createSkill(QString nameOfSkill);
    void callBoldingFromParent(QAbstractButton *bt, bool state);
    QButtonGroup *mainGroupButton;

};

#endif // SKILLS_H
