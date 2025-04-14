#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QVBoxLayout>
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

    void slot_import();
    void slot_refresh();

    void slot_save();
    void slot_saveAs();
    void slot_chartView();

    void slot_addShidunzi();
    void slot_setting();

    void slot_help();
    void slot_updateDialog();

    void slot_zoomIn();
    void slot_zoomOut();

    void slot_quit();

    void slot_addShidunzi_save(std::vector<Shidunzi> &newShidunzi);

    void on_comboBox_activated(int index);

    inline void on_checkBox_stateChanged(int arg1)  {gcd_state = arg1;}

    inline void on_boomButton_pressed() {player.stop(); player.play();}

private:

    QString savePath;
    QString importPath;
    QString backgroundPath;
    QString trackPath;
    QPixmap background;
    QMediaPlayer player;

    SongInfomation SongInfo;
    ShidunziStack stack;
    std::vector<ExStone> exs;
    std::vector<ExTouch> ext;
    std::string currentContent;

    std::stringstream buffer;
    QString hint;

    Ui::MainWindow *ui;

    void menuInit();
    void display();
    void setHint(const QString);
    void setHint(const QString, const QStringList);
    QString findPath(QString&, const std::string&);
    void importFile(QFile&);
    void importFile(QString&);
    void saveFile(QFile&);
    void saveFile(QString&);

    int importDiff;
    bool gcd_state;
};
#endif // MAINWINDOW_H
