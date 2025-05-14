#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::menuInit()
{
    setWindowTitle(MainTitle);
    player.setMedia(QUrl::fromLocalFile("./boom.mp3"));

    ui->background_2->setStyleSheet("background-color: rgba(255, 255, 255, 192);");

    connect(ui->newAct, &QAction::triggered, this, &MainWindow::slot_new);

    connect(ui->openAct, &QAction::triggered, this, &MainWindow::slot_open);
    connect(ui->openButton, &QAbstractButton::clicked, this, &MainWindow::slot_open);

    connect(ui->importAct, &QAction::triggered, this, &MainWindow::slot_import);
    connect(ui->importButton, &QAbstractButton::clicked, this, &MainWindow::slot_import);

    connect(ui->refreshAct, &QAction::triggered, this, &MainWindow::slot_refresh);
    connect(ui->refreshButton, &QAbstractButton::clicked, this, &MainWindow::slot_refresh);

    connect(ui->saveAct, &QAction::triggered, this, &MainWindow::slot_save);
    connect(ui->saveButton, &QAbstractButton::clicked, this, &MainWindow::slot_save);

    connect(ui->saveAsAct, &QAction::triggered, this, &MainWindow::slot_saveAs);
    connect(ui->chartViewAct, &QAction::triggered, this, &MainWindow::slot_chartView);

    connect(ui->addShidunziAct, &QAction::triggered, this, &MainWindow::slot_addShidunzi);

    //进入石墩子偏移设置窗口
    connect(ui->settingAct, &QAction::triggered, this, [this](){ui->tabWidget->setCurrentWidget(ui->settingTab);});
    connect(ui->st_saveButton, &QAbstractButton::clicked, this, &MainWindow::slot_st_save);
    connect(ui->st_helpButton, &QAbstractButton::clicked, this, &MainWindow::slot_st_help);

    connect(ui->previewAct, &QAction::triggered, this, &MainWindow::slot_preview);
    connect(ui->playAct, &QAction::triggered, this, &MainWindow::slot_play);

    connect(ui->helpAct, &QAction::triggered, this, &MainWindow::slot_help);
    connect(ui->dailyAct, &QAction::triggered, this, &MainWindow::slot_updateDialog);

    connect(ui->zoomIn, &QAbstractButton::clicked, this, &MainWindow::slot_zoomIn);
    connect(ui->zoomOut, &QAbstractButton::clicked, this, &MainWindow::slot_zoomOut);

    connect(ui->quitAct, &QAction::triggered, this, &MainWindow::slot_quit);
    ui->comboBox->setCurrentIndex(4);
    mainData.importDiff = ui->comboBox->currentIndex();
    mainData.gcd_state = ui->checkBox->checkState();
    mainData.sort_state = ui->checkBox_sort->checkState();

    ui->st_textEdit->setText(QString::fromStdString(mainData.setting));
    ui->tabWidget->setCurrentWidget(ui->editTab);
}
