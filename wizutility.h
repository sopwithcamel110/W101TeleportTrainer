#ifndef WIZUTILITY_H
#define WIZUTILITY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class WizUtility; }
QT_END_NAMESPACE

class WizUtility : public QMainWindow
{
    Q_OBJECT

public:
    WizUtility(QWidget *parent = nullptr);
    ~WizUtility();

private slots:
    void on_attachButton_clicked();

    void on_questPortButton_clicked();

    void updateCoords();

private:
    Ui::WizUtility *ui;
};
#endif // WIZUTILITY_H
