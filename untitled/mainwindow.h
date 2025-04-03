#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "shidunzistack.h"
#include "settingwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openButton_clicked();

    void on_inportButton_clicked();

    void on_saveButton_clicked();

    void on_helpButton_clicked();

    void on_newButton_clicked();

    void on_refreshButton_clicked();

    void on_saveButton_2_clicked();

    void on_action_Q_triggered();

    void on_settingButton_clicked();

    void on_zoomIn_clicked();

    void on_zoomOut_clicked();

private:

//    QTextEdit *textEdit;
//    QPushButton *openButton;
//    QPushButton *importButton;
//    QPushButton *saveButton;
//    QPushButton *helpButton;

    QString savePath;
    QString inportPath;

    SongInfomation SongInfo;
    ShidunziStack stack;
    std::vector<ExStone> exs;
    std::vector<ExTouch> ext;
    std::string currentContent;

    Ui::MainWindow *ui;

    void menuInit();
    void inportFile(QFile);
    void saveFile(QFile);
};
#endif // MAINWINDOW_H
