#include "maindata.h"
#include <cmath>

const QString MainData::sdz_difficulty[6] = {"Fun","Easy","Normal","Hard","Powerful","So:Powerful"};

static std::string to_string2(double value) {
    double rounded = std::round(value * 100) / 100; // 四舍五入到2位小数
    std::string result = std::to_string(rounded);
    size_t dot_pos = result.find('.');
    if (dot_pos != std::string::npos) {
        result = result.substr(0, dot_pos + 3); // 保留2位小数
    }
    return result;
}

MainData::MainData()
{
    for (int i = 0; i < 33; i++)
    {
        nowTouchSet[i] = defaultTouchSet[i];
        setting = setting+"{name:"+nowTouchSet[i].name
                +" xOffset:"+to_string2(nowTouchSet[i].xOffset)
                +" yOffset:"+to_string2(nowTouchSet[i].yOffset)
                +" size:"+to_string2(nowTouchSet[i].size)+"}\n";
    }
}
