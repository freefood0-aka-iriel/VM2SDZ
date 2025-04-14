#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::display()
{
    ui->textEdit->clear();
    buffer.str(std::string());
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
    if (gcd_state)
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

void MainWindow::setHint(const QString hintMessage)
{
    hint.clear();
    hint = hintMessage;
    ui->hintLabel->setText(hint);
}

void MainWindow::setHint(const QString prefix, const QStringList messages)
{
    hint.clear();
    hint = prefix;
    for (const auto &msg : messages)
    {
        hint.append(msg);
    }
    ui->hintLabel->setText(hint);
}

void MainWindow::importFile(QFile& file)
{
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    const std::string content = ba.toStdString();

    stack = ShidunziStack();

    if (content.find("songName") != std::string::npos)
    {
        stack.read_as_VM(content,exs,ext,SongInfo,importDiff);
    }
    else if (content.find("&inote_") != std::string::npos)
    {
        int i = stack.read_as_Simai(content,exs,ext,SongInfo,importDiff);
        if (i == -1)
            QMessageBox::information(this, "导入失败", "导入Simai格式时，请确保谱面没有hold");
    }
    else if (content.find("osu file") != std::string::npos)
    {
        stack.read_as_OSU(content,exs,ext,SongInfo);
    }
    else
    {
        hint.append("导入失败");
        QMessageBox::information(this, "导入失败", "请检查文件格式是否正确" );
        return;
    }
    for (const auto& beat : exs)
        stack.countAdjust(beat);
    for (const auto& beat : ext)
        stack.positionAdjust(beat);

    hint.append("已导入：");
    hint.append(importPath);
    display();
    file.close();
}

void MainWindow::importFile(QString& prefile)
{
    QFile file(prefile);
    importFile(file);
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

void MainWindow::on_comboBox_activated(int index)
{
    importDiff = index;
}
