#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>
#include <chrono>
//#include <QDebug>

void MainWindow::display()
{
    ui->textEdit->clear();
    buffer.str(std::string());
    // 石墩子栈预处理
    if (mainData.sort_state)
        stack.sortByTotalBeat();
    stack.bpmAdjust();
    // 将等级转换为质量
    float level = 0.0f;
    try {
        level = std::stof(mainData.SongInfo.level);
    } catch (const std::invalid_argument & e) {
        hint.append("导入失败");
        QMessageBox::information(this, "警告", "请确保等级是一个数字" );
    }
    int mass = mainData.massRule ? level : (level*100000/15);
    // 输出
    buffer << "[Meta]\n"
           << "title = " << mainData.SongInfo.title
           << "\nmass = " << mass
           << "\ndifficulty = " << mainData.SongInfo.difficulty
           << "\nauthor = " << mainData.SongInfo.author
           << "\nmapper = " << mainData.SongInfo.mapper
           << "\noffset = " << mainData.SongInfo.offset
           << "\nbg_offset = " << mainData.SongInfo.bg_offset
           << "\nbpm = " << mainData.SongInfo.bpm
           << "\n";
    stack.output(buffer);
    // 显示
    ui->textEdit->setText(QString::fromStdString(buffer.str()));
}

QString MainWindow::findPath(QString& path, const std::string& file)
{
    QFileInfo fileInfo(path);
    QString directoryPath = fileInfo.absolutePath();

    // 查找同一目录下的文件
    QString targetFilePath = directoryPath + QString::fromStdString(file);
    return targetFilePath;
}

void MainWindow::setHint(QString hintMessage)
{
    hint.clear();
    hint = hintMessage;
    ui->hintLabel->setText(hint);
}

void MainWindow::setHint(QString prefix, QStringList messages)
{
    hint.clear();
    hint = prefix;
    for (const auto &msg : messages)
    {
        hint.append(msg);
    }
    ui->hintLabel->setText(hint);
}

bool MainWindow::importFile(QFile& file)
{
    auto start = std::chrono::high_resolution_clock::now(); // 开始计时

    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    const std::string content = ba.toStdString();

    stack = ShidunziStack();
    int i;
    try {
        if (content.find("songName") != std::string::npos)
        {
            i = stack.read_as_VM(content, mainData);
        }
        else if (content.find("&inote_") != std::string::npos)
        {
            i = stack.read_as_Simai(content, mainData);
        }
        else if (content.find("osu file") != std::string::npos)
        {
            i = stack.read_as_OSU(content, mainData);
        }
        else
        {
            hint.append("导入失败");
            QMessageBox::information(this, "导入失败", "请检查文件格式是否正确" );
            return false;
        }
        if (i == -1)
        {
            hint.append("导入失败");
            QMessageBox::information(this, "导入失败", "谱面格式可能出现了一些异常");
            return false;
        }
    }
    catch(std::string& s) {
        if(s == "Division by zero")
        {
            hint.append("导入失败");
            QMessageBox::information(this, "导入失败", "出现了未被预料的除以0的情况，尽管这不太可能发生。请联系作者" );
            return false;
        }
    }
    for (const auto& beat : mainData.exs)
        stack.countAdjust(beat);
    for (const auto& beat : mainData.ext)
        stack.positionAdjust(beat);
    //镜像；排序请见display函数
    if (mainData.mirror_state)
        stack.flip();

    hint.append("已导入：");
    hint.append(importPath);

    QString bgFile_str = (findPath(importPath,"/bg.png"));
    QFile bgFile(bgFile_str);
    if (bgFile.exists())
    {
        if (background.load(bgFile_str))
        {
            ui->background->setPixmap(background);
        }
    }
    else
    {
        hint.append("，但未找到png背景图片");
        ui->background->setPixmap(QPixmap());
    }
    setHint(hint);

    display();
    file.close();

    auto end = std::chrono::high_resolution_clock::now(); // 结束计时
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // 转换为毫秒
    qDebug() << "本次导入共花费" << duration.count() << "毫秒";

    return true;
}

bool MainWindow::importFile(QString& prefile)
{
    importPath = prefile;
    QFile file(prefile);
    return importFile(file);
}

void MainWindow::saveFile(QFile& file)
{
    if (savePath.isEmpty())
    {
        setHint("保存失败");
        return;
    }
    file.open(QIODevice::WriteOnly);
    QByteArray ba;
    ba.append(ui->textEdit->toPlainText());
    file.write(ba);
    file.close();
    setHint("已保存至", QStringList() << savePath);
}

void MainWindow::saveFile(QString& prefile)
{
    QFile file(prefile);
    saveFile(file);
}
