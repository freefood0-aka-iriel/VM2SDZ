#include "settingwindow.h"
#include "ui_settingwindow.h"
#include "mainwindow.h"

extern TouchSet nowTouchSet[];

SettingWindow::SettingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);
    menuInit();
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::menuInit()
{
    std::string setting="";
    for (int i = 0; i < 33; i++)
    {
        setting = setting+"{name:"+nowTouchSet[i].name
                +" xOffset:"+std::to_string(nowTouchSet[i].xOffset)
                +" yOffset:"+std::to_string(nowTouchSet[i].yOffset)
                +" size:"+std::to_string(nowTouchSet[i].size)+"}\n";
    }
    ui->textEdit->setText(QString::fromStdString(setting));
}

void SettingWindow::on_helpButton_clicked()
{
    QMessageBox::information(this, "帮助文档",
            "该文本记录了对于每个区的touch，石墩子的偏移\n"
            "三个数值分别为x轴，y轴，大小的变化量\n"
            "例如：A1的数值是0,0.33,0，则x轴不偏移，y轴偏移0.33格，大小不变\n"
            "你可以在这里自定义touch偏移，但最好不要改变区域的名称（没有用）");
}

void SettingWindow::on_saveButton_clicked()
{
    QByteArray ba;
    ba.append(ui->textEdit->toPlainText());
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

        nowTouchSet[i].xOffset = extractFloat(note, "xOffset:");
        nowTouchSet[i].yOffset = extractFloat(note, "yOffset:");
        nowTouchSet[i].size = extractFloat(note, "size:");
        i++;
    }
}
