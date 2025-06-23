#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::slot_new()
{
    importPath = nullptr;
    ui->textEdit->clear();
    buffer.str(std::string());
    stack.clear();
    mainData.exs.clear();
    mainData.ext.clear();
    setHint("已新建并清空历史记录。新建文件请点击保存");
}

void MainWindow::slot_open()
{
    savePath = QFileDialog::getOpenFileName(this, "打开文件", "", "石墩子快跑谱面文件 (*.sdz);;所有格式 (*.*)");
    if (savePath.isEmpty())
    {
        setHint("打开失败");
        return;
    }
    QFile file(savePath);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    ui->textEdit->setText(QString(ba));
    file.close();
    setHint("已打开：", QStringList() << savePath);
}

void MainWindow::slot_import()
{
    hint.clear();
    importPath = QFileDialog::getOpenFileName(this, "导入文件", "", "Visual Maimai谱面文件 (chart.json);;"
                                                                    "Majdata谱面文件 (maidata.txt);;"
                                                                    "Osu! Mania谱面文件 (*.osu);;"
                                                                    "所有格式 (*.*)");
    if (importPath.isEmpty())
        return;
    if(importFile(importPath))
    {
        setWindowTitle(importPath + " - " + MainTitle);
    }
}

void MainWindow::slot_refresh()
{
    if (importPath.isEmpty())
    {
        setHint("刷新失败");
        return;
    }
    QFile file(importPath);
    importFile(file);
    setHint("已刷新：", QStringList() << importPath);
}
//保存
void MainWindow::slot_save()
{
    if (savePath.isEmpty())
        savePath = QFileDialog::getSaveFileName(this, "保存文件", "data.sdz", "石墩子快跑谱面文件 (*.sdz)");
    saveFile(savePath);
}
//另存为
void MainWindow::slot_saveAs()
{
    savePath = QFileDialog::getSaveFileName(this, "保存文件", "data.sdz", "石墩子快跑谱面文件 (*.sdz)");
    saveFile(savePath);
}
//谱面统计
void MainWindow::slot_chartView()
{
    QString str = "";
    hint.clear();
    hint.append("物量：　");
    hint.append(str.setNum(stack.getNumber()));
    hint.append(" 普通石墩子");
    hint.append(str.setNum(stack.getNumber('D')));
    hint.append("个，绝赞");
    hint.append(str.setNum(stack.getNumber('X')));
    hint.append("个\n净物量：");
    hint.append(str.setNum(stack.getPureNumber()));
    hint.append(" 普通石墩子");
    hint.append(str.setNum(stack.getPureNumber('D')));
    hint.append("个，绝赞");
    hint.append(str.setNum(stack.getPureNumber('X')));
    hint.append("个");
    setHint(hint);
}

void MainWindow::slot_addShidunzi()
{
    AddShidunziDialog *asd = new AddShidunziDialog;
    asd->show();
    connect(asd,&AddShidunziDialog::sendShidunzi,this,&MainWindow::slot_addShidunzi_save);
}

void MainWindow::slot_preview()
{
    ui->tabWidget->setCurrentWidget(ui->previewTab);
    // 设置滚动条的初始位置到左下角
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());

    // 连接按钮的点击信号到槽函数
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::slot_play);

    // 连接定时器的超时信号到槽函数
    connect(&timer, &QTimer::timeout, this, [this]() {
        int currentValue = ui->scrollArea->verticalScrollBar()->value();
        if (currentValue > 0) {
            ui->scrollArea->verticalScrollBar()->setValue(currentValue - 10); // 每次向上滚动10像素
        } else {
            timer.stop(); // 停止定时器
        }
    });
}

void MainWindow::slot_play()
{
    if (!timer.isActive()) {
        timer.start(50); // 每50毫秒更新一次滚动条位置
    }
}

void MainWindow::slot_help()
{
    QMessageBox::information(this, "帮助文档", mainData.help);
}

void MainWindow::slot_updateDialog()
{
    QMessageBox::information(this, "更新日志", mainData.updateDialog);
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

void MainWindow::slot_addShidunzi_save(std::vector<Shidunzi> &newShidunzi)
{
    ui->textEdit->clear();
    buffer.str(std::string());
    stack.append(newShidunzi);
    display();
    ui->textEdit->setText(QString::fromStdString(buffer.str()));
}
// 以下是设置选项改变函数，镜像、排序选项改变请见头文件
// 导入难度改变
void MainWindow::on_comboBox_activated(int index)
{
    mainData.importDiff = index;
    if (mainData.difficulty_empty)
        mainData.SongInfo.difficulty = MainData::sdz_difficulty[mainData.importDiff].toStdString();
}

// bg_offset改变
void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    mainData.SongInfo.bg_offset = arg1;
}

// 难度字段改变
void MainWindow::on_lineEdit_difficulty_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        mainData.difficulty_empty = true;
        mainData.SongInfo.difficulty = MainData::sdz_difficulty[mainData.importDiff].toStdString();
    }
    else
    {
        mainData.difficulty_empty = false;
        mainData.SongInfo.difficulty = arg1.toStdString();
    }
}

// mass字段改变
void MainWindow::on_comboBox_mass_activated(int index)
{
    mainData.massRule = index;
}

// 写谱模式改变
void MainWindow::on_comboBox_chartingMode_activated(int index)
{
    mainData.chartingMode = index;
}

// 以下是拖拽文件函数
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.empty())
        return;

    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())
        return;

    if(fileName.endsWith(".sdz"))
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray ba = file.readAll();
        ui->textEdit->setText(QString(ba));
        file.close();
        setHint("已打开：", QStringList() << fileName);
        setWindowTitle(fileName + " - " + MainTitle);
    }
    else if(importFile(fileName))
    {
        setWindowTitle(fileName + " - " + MainTitle);
    }
}
