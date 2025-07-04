#pragma once
#include <QMainWindow>
#include "shidunzi.h"

struct SongInfomation {
    std::string title = "Unknown";
    std::string level = "0";
    int mass = 0;
    std::string author = "Unknown";
    std::string mapper = "Unknown";
    std::string difficulty = "Powerful";
    float offset = 0;
    float bg_offset = 0;
    float bpm = 125;
};

// MainWindow的数据类，单例
class MainData
{
public:
    MainData();

    const QString help =           "Visual Maimai创作石墩子谱面文件教程：\n"
                                   "放置石墩子：在4、5、6号键放置note，对应1、2、3轨道；可放置绝赞\n"
                                   "堆叠石墩子：在其它按键放置note，每放置一个多堆叠一个\n"
                                   "削去石墩子：在其它按键放置Ex note（保护套），每有一个保护套削去一个\n"
                                   "背景前显音符：放置touchHold，前显的时间段和touchHold时间段相同（请勿在放置的过程中改变分音）\n"
                                   "修改小数轨道，高度和大小：放置touch，可以给予该时间点的石墩子一个数值偏移，详情请见设置\n"
                                   "表演墩子：将该墩子自身设为保护即可\n"
                                   "关于伪双押：Visual Maimai可按M开启，maidata则是以`替换/，在这里它可以不受当前时刻的叠墩子、偏移影响\n"
                                   "关于Osu! Mania转谱：只能实现堆叠，长条变绝赞等基础功能，不支持变速，不推荐使用";

    const QString updateDialog =   "1.8.5\n　　更多批量石墩子曲线，30个一套的缓动函数\n　　C区touch不再提供偏移，而是可以添加标记\n　　可设置伪双押躲过叠墩子和偏移\n"
                                   "1.8.4\n　　适配2.6.1版本石墩子快跑\n"
                                   "1.8.3\n　　添加了随机插值模式，修复了插值无法填入0和负数的问题\n"
                                   "1.8.2\n　　修复了一些bug\n"
                                   "1.8.1\n　　美化页面，背景贴上曲绘，写石墩子更舒适\n　　添加了提示信息\n　　添加了一个神秘小按钮\n"
                                   "1.8.0\n　　更新了Simai语法转换，Osu! Mania谱面转换功能\n　　更新了约分功能，做成可选项，因为会消耗额外算力（真的有必要吗）\n"
                                   "1.7.1\n　　修复了反复导入重复计算的问题\n"
                                   "1.7.0\n　　更新了批量放置石墩子功能\n　　更新了次难度导入功能\n　　修改了部分UI\n　　修复了把BPM当常规石墩子处理的问题\n"
                                   "1.6.0\n　　更新了刷新、背景前显音符、石墩子偏移功能，可兼容最新版石墩子快跑\n　　用cmake替换qmake重新构建了程序\n　　修复了非Qt环境下程序无法打开的问题\n"
                                   "1.5.0　　用Qt重新编写程序，添加了可视化界面\n"
                                   "1.4.1　　修复了无法添加不被4整除的split的问题\n"
                                   "1.4.0　　更新了修改BPM功能\n"
                                   "1.3.0　　更新了剔除数量功能\n"
                                   "1.2.0　　可自动填充歌曲信息\n"
                                   "1.1.0　　更新了堆叠石墩子功能\n"
                                   "1.0.0　　初版";

    const QString settingHelp =    "该文本记录了对于每个区的touch，石墩子的偏移\n"
                                   "三个数值分别为x轴，y轴，大小的变化量\n"
                                   "例如：A1的数值是0,0.33,0，则x轴不偏移，y轴偏移0.33格，大小不变\n"
                                   "你可以在这里自定义touch偏移，但最好不要改变区域的名称（没有用）\n"
                                   "当前版本C区touch的作用是添加标记";
    // 默认touch偏移列表
    const TouchSet defaultTouchSet[33]
    {
        {"A1",0,0.33f,0},
        {"A2",0.33f,0,0},
        {"A3",0.33f,0,0},
        {"A4",0,-0.33f,0},
        {"A5",0,-0.33f,0},
        {"A6",-0.33f,0,0},
        {"A7",-0.33f,0,0},
        {"A8",0,0.33f,0},
        {"B1",0,0.25f,0},
        {"B2",0.25f,0,0},
        {"B3",0.25f,0,0},
        {"B4",0,-0.25f,0},
        {"B5",0,-0.25f,0},
        {"B6",-0.25f,0,0},
        {"B7",-0.25f,0,0},
        {"B8",0,0.25f,0},
        {"C",0,0,0},
        {"D1",0,0.5f,0},
        {"D2",0.5f,0.5f,0},
        {"D3",0.5f,0,0},
        {"D4",0.5f,-0.5f,0},
        {"D5",0,-0.5f,0},
        {"D6",-0.5f,-0.5f,0},
        {"D7",-0.5f,0,0},
        {"D8",-0.5f,0.5f,0},
        {"E1",0,0.25f,0},
        {"E2",0.25f,0.25f,0},
        {"E3",0.25f,0,0},
        {"E4",0.25f,-0.25f,0},
        {"E5",0,-0.25f,0},
        {"E6",-0.25f,-0.25f,0},
        {"E7",-0.25f,0,0},
        {"E8",-0.25f,0.25f,0}
    };
    // 可修改的touch偏移列表
    TouchSet nowTouchSet[33]={};
    // touch偏移的文字显示
    std::string setting;
    // 导入难度
    int importDiff;
    // mass字段
    int massRule = 0;
    // 写谱模式
    int chartingMode = 0;
    // 标志位
    bool mirror_state = 0;
    bool sort_state = 1;
    bool difficulty_empty = 1;
    // 不知道这是什么东西
    std::string currentContent;

    SongInfomation SongInfo;
    std::vector<ExStone> exs;
    std::vector<ExTouch> ext;

    static const QString sdz_difficulty[6];
};
