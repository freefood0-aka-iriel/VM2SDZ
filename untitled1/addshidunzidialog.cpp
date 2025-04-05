#include "addshidunzidialog.h"
#include "ui_addshidunzidialog.h"

AddShidunziDialog::AddShidunziDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddShidunziDialog)
{
    ui->setupUi(this);
}

AddShidunziDialog::~AddShidunziDialog()
{
    delete ui;
}

float AddShidunziDialog::rate(const int &i,const int &mode)
{
    float rate = float(i) / (rule.number-1);
    float fixrate = 0;
    switch (mode)
    {
    case 0:
        fixrate = rate;
        break;
    case 1:
        fixrate = - rate * (rate - 2);
        break;
    case 2:
        fixrate = rate * rate;
        break;
    case 3:
        fixrate = sqrt(1 - (rate - 1) * (rate - 1));
        break;
    case 4:
        fixrate = 1 - sqrt(1 - rate * rate);
        break;
    }
    return fixrate;
}

void AddShidunziDialog::on_timeBeginLineEdit_textEdited(const QString &arg1)
{
    QStringList list = arg1.split(',');
    for (int i = 0; i < 3; i++)
    {
        QString imp = list.first();
        rule.time_begin[i] = imp.toInt();
        list.pop_front();
        if(list.isEmpty()) break;
    }
}

void AddShidunziDialog::on_generateButton_clicked()
{
    int step = rule.time_begin[2]*4 / rule.split;
    if (((rule.time_begin[2]*4) % rule.split) != 0 || rule.time_begin[2]==0)
    {
        QMessageBox::warning(this,"警告","请将分音数与间隔设置为同一数值/n"
                                  "（或者，分音数的四倍能被间隔整除）");
        return;
    }
    buffer.str(std::string());
    stones.clear();
    qDebug()<<QString::fromStdString(buffer.str());
    for (int i = 0; i < rule.number; i++)
    {
        int beat = rule.time_begin[0];
        int denominator = rule.time_begin[2];
        int numerator = rule.time_begin[1] + step*i;
        beat += int(numerator / denominator);
        numerator = numerator % denominator;
        float track = rule.track_begin + (rule.track_end - rule.track_begin) * rate(i,rule.mode[0]);
        float yOffset = rule.yOffset_begin + (rule.yOffset_end - rule.yOffset_begin) * rate(i,rule.mode[1]);
        float size = rule.size_begin + (rule.size_end - rule.size_begin) * rate(i,rule.mode[2]);
        float total_beat = float(beat) + float(numerator) / float(denominator);
        Shidunzi s = {denominator, total_beat, numerator, track, rule.isBreak};
        s.setTrack(track);
        s.setYOffset(yOffset);
        s.setSize(size);
        stones.push_back(s);
        qDebug()<<stones.size();

        buffer << s.getType() << "," << beat << ","
               << numerator << "," << denominator << ","
               << track << ",1,0," << size << ","
               << yOffset << "\n";
    }
    ui->textEdit->setText(QString::fromStdString(buffer.str()));
}

void AddShidunziDialog::on_saveButton_clicked()
{
    emit sendShidunzi(stones);
}
