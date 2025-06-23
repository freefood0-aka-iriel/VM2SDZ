#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::slot_st_help()
{
    QMessageBox::information(this, "帮助文档", mainData.settingHelp);
}

void MainWindow::slot_st_save()
{
    QByteArray ba;
    ba.append(ui->st_textEdit->toPlainText());
    std::string setting = ba.toStdString();

    size_t pos = 0;
    int i = 0;
    while ((pos = setting.find('{', pos)) != std::string::npos)
    {
        size_t end = setting.find("}", pos);
        if (end == std::string::npos)
            break;

        std::string note = setting.substr(pos, end - pos + 1);
        pos = end + 1;

        mainData.nowTouchSet[i].xOffset = extractFloat(note, "xOffset:");
        mainData.nowTouchSet[i].yOffset = extractFloat(note, "yOffset:");
        mainData.nowTouchSet[i].size = extractFloat(note, "size:");
        i++;
    }
    setHint("已保存设置");
}
