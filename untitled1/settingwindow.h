#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QMainWindow>

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();

private slots:
    void on_saveButton_clicked();

    void on_helpButton_clicked();

private:
    Ui::SettingWindow *ui;
    void menuInit();
};

#endif // SETTINGWINDOW_H
