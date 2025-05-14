#include "maindata.h"

MainData::MainData()
{
    for (int i = 0; i < 33; i++)
    {
        nowTouchSet[i] = defaultTouchSet[i];
        setting = setting+"{name:"+nowTouchSet[i].name
                +" xOffset:"+std::to_string(nowTouchSet[i].xOffset)
                +" yOffset:"+std::to_string(nowTouchSet[i].yOffset)
                +" size:"+std::to_string(nowTouchSet[i].size)+"}\n";
    }
}
