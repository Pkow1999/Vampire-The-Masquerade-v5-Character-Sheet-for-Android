#ifndef OPTIONS_H
#define OPTIONS_H

#include <QButtonGroup>
#include <QDir>
#include <QStandardPaths>
#include <QWidget>
#include <QFile>
#include <QJsonDocument>

namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

private slots:
    void on_saveButton_clicked();

    void on_loadSaveButton_clicked();

    void on_deleteSavesButton_clicked();

    void on_saveClicked(QAbstractButton *bt);
private:
    Ui::Options *ui;
    void createSaveWidget(const QString& saveName);
    QList<QWidget *> listOfAllSaves;
    QButtonGroup *loadSaveButtonGroup;
    QButtonGroup *deleteSaveButtonGroup;
    void checkSaves();
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QDir dir = QDir(path);
};

#endif // OPTIONS_H
