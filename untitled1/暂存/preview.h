#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include "shidunzi.h"

class Preview : public QWidget
{
    Q_OBJECT
public:
    Preview(QWidget *parent = nullptr);

    void paintShidunzi(Shidunzi) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 绘制多个圆
        for (int i = 0; i < 10; ++i) {
            int y = i * 200 + 50; // 圆的y坐标
            painter.drawEllipse(50, y, 100, 100); // 绘制圆
        }
    }
signals:

};
