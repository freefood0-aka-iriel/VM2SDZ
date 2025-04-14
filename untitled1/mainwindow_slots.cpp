#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::slot_new()
{
    importPath = nullptr;
    ui->textEdit->clear();
    buffer.str(std::string());
    stack.clear();
    exs.clear();
    ext.clear();
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
    QFile file(importPath);
    importFile(file);
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
            "关于Osu! Mania转谱：只能实现堆叠，长条变绝赞等基础功能，不支持变速，不推荐使用");
}

void MainWindow::slot_updateDialog()
{
    QMessageBox::information(this, "更新日志",
            "1.8.1\n　　修复了一些bug\n"
            "1.8.1\n　　美化页面，背景贴上曲绘，写石墩子更舒适\n　　添加了提示信息\n　　添加了一个神秘小按钮\n"
            "1.8.0\n　　更新了Simai语法转换，Osu! Mania谱面转换功能\n　　更新了约分功能，做成可选项，因为会消耗额外算力（真的有必要吗）\n"
            "1.7.1\n　　修复了反复导入重复计算的问题\n"
            "1.7.0\n　　更新了批量放置石墩子功能\n　　更新了次难度导入功能\n　　修改了部分UI\n　　修复了把BPM当常规石墩子处理的问题\n"
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

void MainWindow::slot_addShidunzi_save(std::vector<Shidunzi> &newShidunzi)
{
    ui->textEdit->clear();
    buffer.str(std::string());
    stack.mergeShidunzi(newShidunzi);
    display();
    ui->textEdit->setText(QString::fromStdString(buffer.str()));
}
