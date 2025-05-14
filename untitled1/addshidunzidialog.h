#pragma once

#include <cmath>
#include <QDialog>
#include "shidunzi.h"

namespace Ui {
class AddShidunziDialog;
}

class AddShidunziDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddShidunziDialog(QWidget *parent = nullptr);
    ~AddShidunziDialog();

signals:
    void sendShidunzi(std::vector<Shidunzi> &newShidunzi);

private slots:
    inline void on_trackBeginSpinBox_valueChanged(double arg1)      {rule.track_begin = arg1;}
    inline void on_trackEndSpinBox_valueChanged(double arg1)        {rule.track_end = arg1;}
    inline void on_yOffsetBeginSpinBox_valueChanged(double arg1)    {rule.yOffset_begin = arg1;}
    inline void on_yOffsetEndSpinBox_valueChanged(double arg1)      {rule.yOffset_end = arg1;}
    inline void on_sizeBeginSpinBox_valueChanged(double arg1)       {rule.size_begin = arg1;}
    inline void on_sizeEndSpinBox_valueChanged(double arg1)         {rule.size_end = arg1;}
    inline void on_trackRuleComboBox_activated(int index)           {rule.mode[0] = index;}
    inline void on_yOffsetRuleComboBox_activated(int index)         {rule.mode[1] = index;}
    inline void on_sizeRuleComboBox_activated(int index)            {rule.mode[2] = index;}
    inline void on_splitSpinBox_valueChanged(int arg1)           {rule.split = arg1;}
    inline void on_numberSpinBox_valueChanged(int arg1)          {rule.number = arg1;}
    inline void on_lineEdit_textEdited(const QString &arg1)
    {
        rule.type = arg1.front().toLatin1();
    }

    void on_timeBeginLineEdit_textEdited(const QString &arg1);

    void on_generateButton_clicked();

    void on_saveButton_clicked();

private:
    std::vector<Shidunzi> stones;

    std::stringstream buffer;

    struct GenerateRule
    {
        float track_begin = 1;
        float track_end = 3;
        float yOffset_begin = 0;
        float yOffset_end = 0;
        float size_begin = 1;
        float size_end = 1;
        int mode[3] = {};
        int time_begin[3] = {100,4,16};
        int split = 16;
        int number = 15;
        char type = 'D';
    } rule;

    Ui::AddShidunziDialog *ui;

    float rate(const int i, const int mode = 0);
};
