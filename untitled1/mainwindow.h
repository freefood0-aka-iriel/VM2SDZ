#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QVBoxLayout>
#include "shidunzistack.h"
#include "addshidunzidialog.h"
#include "maindata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const QString MainTitle = "vm2sdz 1.9.0 beta by Iriel";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_new();
    void slot_open();

    void slot_import();
    void slot_refresh();

    void slot_save();
    void slot_saveAs();
    void slot_chartView();

    void slot_addShidunzi();

    void slot_preview();
    void slot_play();
    void slot_help();
    void slot_updateDialog();

    void slot_zoomIn();
    void slot_zoomOut();

    void slot_quit();

    //石墩子偏移设置
    void slot_st_help();
    void slot_st_save();

    void slot_addShidunzi_save(std::vector<Shidunzi> &newShidunzi);

    void on_comboBox_activated(int index);
    inline void on_boomButton_pressed() {player.stop(); player.play();}

    inline void on_checkBox_sort_clicked (bool checked) {mainData.sort_state = checked;}
    inline void on_checkBox_mirror_clicked(bool checked) {mainData.mirror_state = checked;}

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

    void on_doubleSpinBox_valueChanged(double arg1);
    void on_lineEdit_difficulty_textChanged(const QString &arg1);
    void on_comboBox_mass_activated(int index);
    void on_comboBox_chartingMode_activated(int index);


private:

    MainData mainData;
    QString savePath;
    QString importPath;
    QString backgroundPath;
    QString trackPath;
    QPixmap background;
    QMediaPlayer player;
    QTimer timer;

    ShidunziStack stack;

    std::stringstream buffer;
    QString hint;

    Ui::MainWindow *ui;

    void menuInit();
    void display();
    void setHint(QString);
    void setHint(QString, QStringList);
    QString findPath(QString&, const std::string&);
    bool importFile(QFile&);
    bool importFile(QString&);
    void saveFile(QFile&);
    void saveFile(QString&);
};
