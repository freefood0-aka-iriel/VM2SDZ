#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::display()
{
    ui->textEdit->clear();
    buffer.str(std::string());
    buffer << "[Meta]\n"
           << "title = " << mainData.SongInfo.title << "\n"
           << "level = " << mainData.SongInfo.level << "\n"
           << "author = " << mainData.SongInfo.author << "\n"
           << "mapper = " << mainData.SongInfo.mapper << "\n"
           << "offset = " << mainData.SongInfo.offset << "\n"
           << "bg_offset = " << mainData.SongInfo.bg_offset << "\n"
           << "bpm = " << mainData.SongInfo.bpm << "\n"
           << "[Data]\n";
    if (mainData.sort_state)
        stack.sortByTotalBeat();
    if (mainData.gcd_state)
        stack.gcd();
    stack.bpmAdjust();
    stack.output(buffer);
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
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    const std::string content = ba.toStdString();

    stack = ShidunziStack();
    int i;
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
    for (const auto& beat : mainData.exs)
        stack.countAdjust(beat);
    for (const auto& beat : mainData.ext)
        stack.positionAdjust(beat);

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
