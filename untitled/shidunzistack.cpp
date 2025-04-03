#include "shidunzistack.h"

extern TouchSet nowTouchSet[];

int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b)
{
    return (a / gcd(a, b)) * b;
}

int mod(int a, int b)
{
    return (a % b + b) % b;
}


void ShidunziStack::incrementCountForBeat(ExStone exs)
{
    for (auto& s : stones)
    {
        if (fabs(s.beat - exs.beat) < EPSILON)
        {
            s.count++;
            if (exs.isEx)
                s.deleteCount++;
        }
    }
}

void ShidunziStack::incrementPositionForBeat(ExTouch ext)
{
    for (auto& s : stones)
    {
        if (fabs(s.beat - ext.beat) < EPSILON)
        {
            s.track += ext.ts.xOffset;
            s.yOffset += ext.ts.yOffset;
            s.size += ext.ts.size;
        }
    }
}

void ShidunziStack::sortByTotalBeat()
{
    sort
    (
        stones.begin(), stones.end(), [](const Shidunzi& a, const Shidunzi& b)
        {
            return a.beat < b.beat;
        }
    );
}

void ShidunziStack::resetBeatsAfterChange()
{
    float lastBeat = 0;
    int lastNumerator = 0;
    int lastDenominator = 1;
    int currentDenominator;
    float swapBeat = 0;
    int swapNumerator;
    int swapDenominator;

    for (auto& s : stones)
    {
        if (s.type == 'B')
        {
            // 遇到BPM变化音符，更新当前拍子和分音数
            swapBeat = s.beat; // 记录当前拍数
            swapNumerator = s.numerator;
            swapDenominator = s.denominator;
        }
        // 对于非BPM变化音符，重新计算拍子数和分音数
        s.beat -= lastBeat;
        currentDenominator = lcm(lastDenominator , s.denominator);
        s.numerator = mod((s.numerator * currentDenominator / s.denominator - lastNumerator * currentDenominator / lastDenominator) , currentDenominator);
        s.denominator = currentDenominator;
        if (swapBeat !=0 )
        {
            lastBeat = swapBeat;
            lastNumerator = swapNumerator;
            lastDenominator = swapDenominator;
            swapBeat = 0;
        }
    }
}

void ShidunziStack::read_as_VM(const std::string& content, std::vector<ExStone>& exs, std::vector<ExTouch>& ext, SongInfomation& SongInfo)
{
    // 读取歌曲信息
    SongInfo.title = extractString(content, "\"songName\":");
    SongInfo.author = extractString(content, "\"composer\":");
    SongInfo.offset = extractFloat(content, "\"offset\":");
    SongInfo.bg_offset = SongInfo.offset;
    SongInfo.level = extractString(content, "\"level\":");
    SongInfo.mapper = extractString(content, "\"charter\":");
    /*SetConsoleOutputCP(CP_UTF8);
    cout << SongInfo.title << SongInfo.author << SongInfo.offset << SongInfo.level << SongInfo.mapper << endl;
    string filepath;
    getline(cin, filepath);*/

    size_t bpm_start = content.find("\"bpmList\":");    //寻找bpm列表
    if (bpm_start != std::string::npos)
    {
        bpm_start += 10;
        size_t bpm_end = content.find(']', bpm_start);
        std::string bpm_str = content.substr(bpm_start, bpm_end - bpm_start);

        size_t pos = 0;
        while ((pos = bpm_str.find('{', pos)) != std::string::npos)
        {
            size_t end = bpm_str.find("e\":0}", pos);
            if (end == std::string::npos) break;

            std::string bpm = bpm_str.substr(pos, end - pos + 1);
            pos = end + 1;

            int split = extractValue(bpm, "\"split\":");   //分音
            int beat = extractValue(bpm, "\"beat\":");     //拍数
            float button = extractFloat(bpm, "\"bpm\":"); //这里是BPM的值

            if (beat == 0)
                SongInfo.bpm = button;
            if (split <= 0 || beat <= 0)
                continue;           //剔除无效项
            beat *= 4;
            float total_beat = static_cast<float>(beat) / split;

            Shidunzi s(split, total_beat, beat, button, false);
            s.setType('B');
            stones.push_back(s);
        }
    }

    size_t taps_start = content.find("\"taps\":[");         //寻找taps列表
    if (taps_start != std::string::npos)
    {
        taps_start += 8;
        size_t taps_end = content.find(']', taps_start);
        std::string taps_str = content.substr(taps_start, taps_end - taps_start);

        size_t pos = 0;
        while ((pos = taps_str.find('{', pos)) != std::string::npos)
        {
            size_t end = taps_str.find("e}", pos);
            if (end == std::string::npos) break;

            std::string note = taps_str.substr(pos, end - pos + 1);
            pos = end + 1;

            int split = extractValue(note, "\"split\":");   //分音
            int beat = extractValue(note, "\"beat\":");     //拍数
            int button = extractValue(note, "\"button\":"); //按键
            bool isBreak = extractBool(note, "\"isBreak\":");   //是否为绝赞
            bool isEx = extractBool(note, "\"isEx\":");     //是否为保护
            if (split <= 0 || beat < 0)
                continue;           //剔除无效项

            beat *= 4;
            float total_beat = static_cast<float>(beat) / split;

            //如果按键为4到6，压入石墩子，否则压入ex列表
            if (button >= 4 && button <= 6)
            {
                Shidunzi s(split, total_beat, beat, button-3, isBreak);
                stones.push_back(s);
            }
            else
            {
                exs.push_back({ total_beat, isEx });
            }
        }
    }

    size_t touch_start = content.find("\"touches\":[");         //寻找touches列表
    if (touch_start != std::string::npos)
    {
        touch_start += 8;
        size_t touch_end = content.find(']', touch_start);
        std::string touch_str = content.substr(touch_start, touch_end - touch_start);

        size_t pos = 0;
        while ((pos = touch_str.find('{', pos)) != std::string::npos)
        {
            size_t end = touch_str.find("e}", pos);
            if (end == std::string::npos) break;

            std::string note = touch_str.substr(pos, end - pos + 1);
            pos = end + 1;

            int split = extractValue(note, "\"split\":");   //分音
            int beat = extractValue(note, "\"beat\":");     //拍数
            std::string buttonName = extractString(note, "\"button\":"); //touch的区域名称

            if (split <= 0 || beat < 0)
                continue;           //剔除无效项

            int button = 0;
            //将touch区域名称映射为数组标
            switch (buttonName[0])
            {
            case 'A': break;
            case 'B': { button+=8; break; }
            case 'C': { button+=16; break; }
            case 'D': { button+=17; break; }
            case 'E': { button+=25; break; }
            }
            if (buttonName[0]!='C')
                button += (int)(buttonName[1]-'1');

            beat *= 4;
            float total_beat = static_cast<float>(beat) / split;

            ext.push_back({total_beat,nowTouchSet[button]});
        }
    }

    size_t touchHold_start = content.find("\"toucheHolds\":");    //寻找touchHold列表
    if (touchHold_start != std::string::npos)
    {
        touchHold_start += 10;
        size_t touchHold_end = content.find(']', touchHold_start);
        std::string touchHold_str = content.substr(touchHold_start, touchHold_end - touchHold_start);

        size_t pos = 0;
        while (pos != std::string::npos)
        {
            pos = touchHold_str.find("\"hitTime\":", pos);
            size_t end = touchHold_str.find("}", pos);
            if (end == std::string::npos)
                break;

            std::string touchHold = touchHold_str.substr(pos, end - pos + 1);
            pos = end + 1;

            int split = extractValue(touchHold, "\"split\":");   //分音
            int beat = extractValue(touchHold, "\"beat\":");     //拍数

            if (split <= 0 || beat <= 0)
                continue;           //剔除无效项
            beat *= 4;
            float total_beat = static_cast<float>(beat) / split;

            Shidunzi s(split, total_beat, beat, 0, false);
            s.setType('H');
            stones.push_back(s);

            pos = touchHold_str.find("\"holdTime\":", pos);
            end = touchHold_str.find("}", pos);
            if (end == std::string::npos)
                break;

            touchHold = touchHold_str.substr(pos, end - pos + 1);
            pos = end + 1;

            beat += 4*extractValue(touchHold, "\"beat\":");     //新增拍数

            if (split <= 0 || beat <= 0)
                continue;           //剔除无效项（应该没有吧？）
            total_beat = static_cast<float>(beat) / split;

            Shidunzi s2(split, total_beat, beat, 0, false);
            s2.setType('H');
            stones.push_back(s2);
        }
    }
}
