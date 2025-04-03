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
    for (int i = 0; i < 33; i++)
        nowTouchSet[i] = defaultTouchSet[i];
    connect(ui->newAct, &QAction::triggered, this, &MainWindow::on_newButton_clicked);
    connect(ui->openAct, &QAction::triggered, this, &MainWindow::on_openButton_clicked);
    connect(ui->inportAct, &QAction::triggered, this, &MainWindow::on_inportButton_clicked);
    connect(ui->refreshAct, &QAction::triggered, this, &MainWindow::on_refreshButton_clicked);
    connect(ui->saveAct, &QAction::triggered, this, &MainWindow::on_saveButton_clicked);
    connect(ui->saveAsAct, &QAction::triggered, this, &MainWindow::on_saveButton_2_clicked);
}

void MainWindow::inportFile(QFile file)
{
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    const std::string content = ba.toStdString();

    stack = ShidunziStack();

    if (content.find("songName") != std::string::npos)
    {
        stack.read_as_VM(content,exs,ext,SongInfo);
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

    std::stringstream buffer;
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
            if(s.getType()!='H')
                buffer << s.getType() << "," << static_cast<int>(s.getBeat()) << ","
                       << s.getNumerator() << "," << s.getDenominator() << ","
                       << s.getTrack() << "," << s.getCount() << ","
                       << s.getDeleteCount() << "," << s.getSize() << "," << s.getYOffset() << "\n";
            else
                buffer << s.getType() << "," << static_cast<int>(s.getBeat()) << ","
                       << s.getNumerator() << "," << s.getDenominator() << "\n";
        }
    ui->textEdit->setText(QString::fromStdString(buffer.str()));
    file.close();
}

void MainWindow::saveFile(QFile file)
{
    file.open(QIODevice::WriteOnly);
    QByteArray ba;
    ba.append(ui->textEdit->toPlainText());
    file.write(ba);
    file.close();
}
//slots
void MainWindow::on_newButton_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_openButton_clicked()
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

void MainWindow::on_inportButton_clicked()
{
    inportPath = QFileDialog::getOpenFileName(this, "导入文件", "", "JavaScript数据文件 (*.json);;所有格式(*.*)");
    if (inportPath.isEmpty())
        return;
    inportFile(inportPath);
}

void MainWindow::on_saveButton_clicked()
{
    savePath = QFileDialog::getSaveFileName(this, "保存文件", "data.sdz", "石墩子快跑谱面文件 (*.sdz)");
    if (savePath.isEmpty())
        return;
    saveFile(savePath);
}

void MainWindow::on_helpButton_clicked()
{
    QMessageBox::information(this, "帮助文档",
            "Visual Maimai创作石墩子谱面文件教程：\n"
            "放置石墩子：在4、5、6号键放置note，对应1、2、3轨道；可放置绝赞\n"
            "堆叠石墩子：在其它按键放置note，每放置一个多堆叠一个\n"
            "削去石墩子：在其它按键放置Ex note（保护套），每有一个保护套削去一个\n"
            "背景前显音符：放置touchHold，前显的时间段和touchHold时间段相同（请勿在放置的过程中改变分音）\n"
            "注意：转谱器尚未实现Simai语法转换，且尚未支持石墩子快跑1.5.2以上版本的功能（对于石墩子大小和Y轴偏移，仅装填默认参数），新功能请手动编辑谱面实现");
}

void MainWindow::on_refreshButton_clicked()
{
    if (inportPath.isEmpty())
        return;
    inportFile(inportPath);
}

void MainWindow::on_saveButton_2_clicked()
{
    if (savePath.isEmpty())
        savePath = QFileDialog::getSaveFileName(this, "保存文件", "data.sdz", "石墩子快跑谱面文件 (*.sdz)");
    if (savePath.isEmpty())
        return;
    saveFile(savePath);
}

void MainWindow::on_action_Q_triggered()
{
    MainWindow::close();
}

void MainWindow::on_settingButton_clicked()
{
    SettingWindow *sw = new SettingWindow;
    sw->show();
}

void MainWindow::on_zoomIn_clicked()
{
    ui->textEdit->zoomIn();
}

void MainWindow::on_zoomOut_clicked()
{
    ui->textEdit->zoomOut();
}
