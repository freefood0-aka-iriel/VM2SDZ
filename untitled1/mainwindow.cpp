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
    setWindowTitle("VM2SDZ 1.7.0 by Iriel");
    for (int i = 0; i < 33; i++)
        nowTouchSet[i] = defaultTouchSet[i];
    connect(ui->newAct, &QAction::triggered, this, &MainWindow::slot_new);

    connect(ui->openAct, &QAction::triggered, this, &MainWindow::slot_open);
    connect(ui->openButton, &QAbstractButton::clicked, this, &MainWindow::slot_open);

    connect(ui->inportAct, &QAction::triggered, this, &MainWindow::slot_inport);
    connect(ui->inportButton, &QAbstractButton::clicked, this, &MainWindow::slot_inport);

    connect(ui->refreshAct, &QAction::triggered, this, &MainWindow::slot_refresh);
    connect(ui->refreshButton, &QAbstractButton::clicked, this, &MainWindow::slot_refresh);

    connect(ui->saveAct, &QAction::triggered, this, &MainWindow::slot_save);
    connect(ui->saveButton, &QAbstractButton::clicked, this, &MainWindow::slot_save);

    connect(ui->saveAsAct, &QAction::triggered, this, &MainWindow::slot_saveAs);

    connect(ui->addShidunziAct, &QAction::triggered, this, &MainWindow::slot_addShidunzi);

    connect(ui->settingAct, &QAction::triggered, this, &MainWindow::slot_setting);
    connect(ui->settingButton, &QAbstractButton::clicked, this, &MainWindow::slot_setting);

    connect(ui->helpAct, &QAction::triggered, this, &MainWindow::slot_help);

    connect(ui->dailyAct, &QAction::triggered, this, &MainWindow::slot_updateDialog);

    connect(ui->zoomIn, &QAbstractButton::clicked, this, &MainWindow::slot_zoomIn);

    connect(ui->zoomOut, &QAbstractButton::clicked, this, &MainWindow::slot_zoomOut);

    connect(ui->quitAct, &QAction::triggered, this, &MainWindow::slot_quit);
}

void MainWindow::display()
{
    buffer << "[Meta]\n"
           << "title = " << SongInfo.title << "\n"
           << "level = " << SongInfo.level << "\n"
           << "author = " << SongInfo.author << "\n"
           << "mapper = " << SongInfo.mapper << "\n"
           << "offset = " << SongInfo.offset << "\n"
           << "bg_offset = " << SongInfo.bg_offset << "\n"
           << "bpm = " << SongInfo.bpm << "\n"
           << "[Data]\n";
    stack.sortByTotalBeat();
    stack.resetBeatsAfterChange();
    for (const auto& s : stack.getStones())
    {
        buffer << s.getType() << "," << static_cast<int>(s.getBeat()) << ","
               << s.getNumerator() << "," << s.getDenominator();
        if(s.getType()=='H')
            buffer << "\n";
        else if(s.getType()=='B')
            buffer << "," << s.getTrack() << "\n";
        else
            buffer << "," << s.getTrack() << "," << s.getCount() << ","
                   << s.getDeleteCount() << "," << s.getSize() << "," << s.getYOffset() << "\n";
    }
    ui->textEdit->setText(QString::fromStdString(buffer.str()));
}

void MainWindow::inportFile(QFile& file)
{
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    const std::string content = ba.toStdString();

    stack = ShidunziStack();

    if (content.find("songName") != std::string::npos)
    {
        stack.read_as_VM(content,exs,ext,SongInfo,inportDiff);
    }
    else
    {
        QMessageBox::information(this, "导入失败", "请检查文件格式是否正确" );
        return;
    }
    for (const auto& beat : exs)
        stack.incrementCountForBeat(beat);
    for (const auto& beat : ext)
        stack.incrementPositionForBeat(beat);

    display();
    file.close();
}

void MainWindow::saveFile(QFile& file)
{
    file.open(QIODevice::WriteOnly);
    QByteArray ba;
    ba.append(ui->textEdit->toPlainText());
    file.write(ba);
    file.close();
}
//slots
void MainWindow::slot_new()
{
    inportPath = nullptr;
    ui->textEdit->clear();
    buffer.str(std::string());
    stack.clear();
    exs.clear();
    ext.clear();
}

void MainWindow::slot_open()
{
    savePath = QFileDialog::getOpenFileName(this, "打开文件", "", "石墩子快跑谱面文件 (*.sdz);;所有格式(*.*)");
    if (savePath.isEmpty())
        return;
    QFile file(savePath);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    ui->textEdit->setText(QString(ba));
    file.close();
}

void MainWindow::slot_inport()
{
    inportPath = QFileDialog::getOpenFileName(this, "导入文件", "", "JavaScript数据文件 (*.json);;所有格式(*.*)");
    if (inportPath.isEmpty())
        return;
    QFile file(inportPath);
    inportFile(file);
}

void MainWindow::slot_refresh()
{
    if (inportPath.isEmpty())
        return;
    QFile file(inportPath);
    inportFile(file);
}
//保存
void MainWindow::slot_save()
{
    if (savePath.isEmpty())
        savePath = QFileDialog::getSaveFileName(this, "保存文件", "data.sdz", "石墩子快跑谱面文件 (*.sdz)");
    if (savePath.isEmpty())
        return;
    QFile file(savePath);
    saveFile(file);
}
//另存为
void MainWindow::slot_saveAs()
{
    savePath = QFileDialog::getSaveFileName(this, "保存文件", "data.sdz", "石墩子快跑谱面文件 (*.sdz)");
    if (savePath.isEmpty())
        return;
    QFile file(savePath);
    saveFile(file);
}

void MainWindow::slot_addShidunzi()
{
    AddShidunziDialog *asd = new AddShidunziDialog;
    asd->show();
    connect(asd,&AddShidunziDialog::sendShidunzi,this,&MainWindow::slot_addShidunzi_save);
}

void MainWindow::slot_setting()
{
    SettingWindow *sw = new SettingWindow;
    sw->show();
}

void MainWindow::slot_help()
{
    QMessageBox::information(this, "帮助文档",
            "Visual Maimai创作石墩子谱面文件教程：\n"
            "放置石墩子：在4、5、6号键放置note，对应1、2、3轨道；可放置绝赞\n"
            "堆叠石墩子：在其它按键放置note，每放置一个多堆叠一个\n"
            "削去石墩子：在其它按键放置Ex note（保护套），每有一个保护套削去一个\n"
            "背景前显音符：放置touchHold，前显的时间段和touchHold时间段相同（请勿在放置的过程中改变分音）\n"
            "修改小数轨道，高度和大小：放置touch，可以给予该时间点的石墩子一个数值偏移，详情请见设置\n"
            "注意：转谱器尚未实现Simai语法转换");
}

void MainWindow::slot_updateDialog()
{
    QMessageBox::information(this, "更新日志",
            "1.7.0\n　　更新了批量放置石墩子功能\n　　更新了次难度导入功能\n　　修改了部分UI\n　　修复了把BPM当常规石墩子处理的问题\n　　修复了反复导入重复计算的问题\n"
            "1.6.0\n　　更新了刷新、背景前显音符、石墩子偏移功能，可兼容最新版石墩子快跑\n　　用cmake替换qmake重新构建了程序\n　　修复了非Qt环境下程序无法打开的问题\n"
            "1.5.0\n　　用Qt重新编写程序，添加了可视化界面\n"
            "1.4.1\n　　修复了无法添加不被4整除的split的问题\n"
            "1.4.0\n　　更新了修改BPM功能\n"
            "1.3.0\n　　更新了剔除数量功能\n"
            "1.2.0\n　　可自动填充歌曲信息\n"
            "1.1.0\n　　更新了堆叠石墩子功能\n"
            "1.0.0\n　　初版\n"
            "目前已知bug\n　　修改石墩子偏移只能在导入谱面之前进行，否则没有效果\n");
}

void MainWindow::slot_zoomIn()
{
    ui->textEdit->zoomIn();
}

void MainWindow::slot_zoomOut()
{
    ui->textEdit->zoomOut();
}

void MainWindow::slot_quit()
{
    MainWindow::close();
}

void MainWindow::on_comboBox_activated(int index)
{
    inportDiff = index;
}

void MainWindow::slot_addShidunzi_save(std::vector<Shidunzi> &newShidunzi)
{
    ui->textEdit->clear();
    buffer.str(std::string());
    stack.mergeShidunzi(newShidunzi);
    display();
    ui->textEdit->setText(QString::fromStdString(buffer.str()));
}
