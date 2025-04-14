#include "mainwindow.h"
#include "ui_mainwindow.h"

TouchSet nowTouchSet[33]={};

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
    setWindowTitle("VM2SDZ 1.8.2 by Iriel");
    player.setMedia(QUrl::fromLocalFile("./boom.mp3"));

    ui->graphicsView->setStyleSheet("background-color: rgba(255, 255, 255, 192);");

    for (int i = 0; i < 33; i++)
        nowTouchSet[i] = defaultTouchSet[i];
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

    connect(ui->settingAct, &QAction::triggered, this, &MainWindow::slot_setting);
    connect(ui->settingButton, &QAbstractButton::clicked, this, &MainWindow::slot_setting);

    connect(ui->helpAct, &QAction::triggered, this, &MainWindow::slot_help);
    connect(ui->dailyAct, &QAction::triggered, this, &MainWindow::slot_updateDialog);

    connect(ui->zoomIn, &QAbstractButton::clicked, this, &MainWindow::slot_zoomIn);
    connect(ui->zoomOut, &QAbstractButton::clicked, this, &MainWindow::slot_zoomOut);

    connect(ui->quitAct, &QAction::triggered, this, &MainWindow::slot_quit);
    ui->comboBox->setCurrentIndex(4);
    this->importDiff = ui->comboBox->currentIndex();
    this->gcd_state = ui->checkBox->checkState();
}
