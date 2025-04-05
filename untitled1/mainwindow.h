#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include "shidunzistack.h"
#include "settingwindow.h"
#include "addshidunzidialog.h"

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
    void slot_new();
    void slot_open();

    void slot_inport();
    void slot_refresh();

    void slot_save();
    void slot_saveAs();

    void slot_addShidunzi();
    void slot_setting();

    void slot_help();
    void slot_updateDialog();

    void slot_zoomIn();
    void slot_zoomOut();

    void slot_quit();

    void slot_addShidunzi_save(std::vector<Shidunzi> &newShidunzi);

    void on_comboBox_activated(int index);

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

    std::stringstream buffer;

    Ui::MainWindow *ui;

    void menuInit();
    void display();
    void inportFile(QFile&);
    void saveFile(QFile&);

    int inportDiff=4;
};
#endif // MAINWINDOW_H
