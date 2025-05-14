#include "shidunzistack.h"
#include <numeric>
#include <cmath>
static const auto &npos = std::string::npos;

// 取余
static int mod(int a, int b)
{
    return (a % b + b) % b;
}

// get物量
int ShidunziStack::getNumber()
{
    int i = 0;
    for (auto& s : stones)
    {
        if (s.type == 'D' || s.type == 'X')
            i += s.count - s.deleteCount;
    }
    return i;
}

int ShidunziStack::getNumber(char C)
{
    int i = 0;
    for (auto& s : stones)
    {
        if (s.type == C)
            i += s.count - s.deleteCount;
    }
    return i;
}

// get净物量，不考虑堆叠石墩子
int ShidunziStack::getPureNumber()
{
    return std::count_if(stones.begin(), stones.end(), [](const Shidunzi& s) {
        return s.type == 'D' || s.type == 'X';
    });
}

int ShidunziStack::getPureNumber(char C) {
    return std::count_if(stones.begin(), stones.end(), [&C](const Shidunzi& s) {
        return s.type == C;
    });
}

void ShidunziStack::countAdjust(const ExStone& exs)
{
    for (auto& s : stones)
    {
        if (fabs(s.beat - exs.beat) < EPSILON)
        {
            if (!s.isNoMulti)
            {
                s.count++;
                if (exs.isEx)
                    s.deleteCount++;
            }
        }
    }
}

void ShidunziStack::positionAdjust(const ExTouch& ext)
{
    for (auto& s : stones)
    {
        if (fabs(s.beat - ext.beat) < EPSILON)
        {
            if (!s.isNoMulti)
            {
                s.track += ext.ts.xOffset;
                s.yOffset += (ext.ts.yOffset*2);
                s.size += ext.ts.size;
            }
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

// 约分
void ShidunziStack::gcd()
{
    int i = 0;
    for (auto& s : stones)
    {
        i = std::gcd(s.numerator,s.denominator);
        s.numerator /= i;
        s.denominator /= i;
    }
}

void ShidunziStack::bpmAdjust()
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
        currentDenominator = std::lcm(lastDenominator , s.denominator);
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

// 扩充石墩子，注意参数是石墩子数组
void ShidunziStack::append(std::vector<Shidunzi>& stones2)
{
    for(auto& s : stones2)
        push(s);
    sortByTotalBeat();
}

// 以Visual Maimai格式读取，非常史，我维护不了
int ShidunziStack::read_as_VM(const std::string& precontent, MainData& m)
{
    stones.clear();
    m.exs.clear();
    m.ext.clear();
    // 读取歌曲信息
    m.SongInfo.title = extractString(precontent, "\"songName\":");
    m.SongInfo.author = extractString(precontent, "\"composer\":", '"');
    m.SongInfo.offset = extractFloat(precontent, "\"offset\":");

    std::string content;
    size_t start = 0, end = 0;
    start = precontent.find(("\"difficulty\":"+std::to_string(m.importDiff)));

    if (start != npos)
        content = precontent.substr(start, npos);
    else
        content = precontent;

    m.SongInfo.level = extractString(content, "\"level\":");
    m.SongInfo.mapper = extractString(content, "\"charter\":");

    start = content.find("\"keyframes\":");    //寻找bpm列表
    if (start == npos)
        return -1;

    start += 10;
    end = content.find(']', start);
    std::string bpm_str = content.substr(start, end - start + 2);

    size_t pos = 0;
    while ((pos = bpm_str.find('{', pos)) != npos)
    {
        size_t end = bpm_str.find("},{", pos);
        if (end == npos)
            end = bpm_str.find("}]}", pos);
        if (end == npos)
            break;

        std::string bpm = bpm_str.substr(pos, end - pos + 1);
        pos = end + 1;

        int split = extractValue(bpm, "\"split\":");   //分音
        int beat = extractValue(bpm, "\"beat\":");     //拍数
        float button = extractFloat(bpm, "\"bpm\":"); //这里是BPM的值

        if (beat == 0)
            m.SongInfo.bpm = button;
        if (split <= 0 || beat <= 0)
            continue;           //剔除无效项
        beat *= 4;
        float total_beat = static_cast<float>(beat) / split;

        Shidunzi s(split, total_beat, beat, button, 'B');
        push(s);
    }


    start = content.find("\"taps\":[");         //寻找taps列表
    if (start == npos)
        return -1;

    start += 8;
    end = content.find(']', start);
    std::string taps_str = content.substr(start, end - start);

    pos = 0;
    while ((pos = taps_str.find('{', pos)) != npos)
    {
        size_t end = taps_str.find("e}", pos);
        if (end == npos) break;

        std::string note = taps_str.substr(pos, end - pos + 1);
        pos = end + 1;

        int split = extractValue(note, "\"split\":");   //分音
        int beat = extractValue(note, "\"beat\":");     //拍数
        int button = extractValue(note, "\"button\":"); //按键
        bool isNoMulti = extractBool(note, "\"isNoMulti\":");   //是否为伪双押
        bool isBreak = extractBool(note, "\"isBreak\":");   //是否为绝赞
        bool isEx = extractBool(note, "\"isEx\":");     //是否为保护
        if (split <= 0 || beat < 0)
            continue;           //剔除无效项

        beat *= 4;
        float total_beat = static_cast<float>(beat) / split;

        char C = isEx ? 'S' : (isBreak ? 'X' : 'D');

        //如果按键为4到6，压入石墩子，否则压入ex列表
        if (button >= 4 && button <= 6)
        {
            Shidunzi s(split, total_beat, beat, button-3, C);
            push(s);
            stones.back().isNoMulti = isNoMulti;
        }
        else
        {
            m.exs.push_back({ total_beat, isEx });
        }
    }


    start = content.find("\"touches\":[");         //寻找touches列表
    if (start == npos)
        return -1;

    start += 8;
    end = content.find(']', start);
    std::string touch_str = content.substr(start, end - start);

    pos = 0;
    while ((pos = touch_str.find('{', pos)) != npos)
    {
        size_t end = touch_str.find("e}", pos);
        if (end == npos) break;

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

        beat *= 4;
        float total_beat = static_cast<float>(beat) / split;

        if (buttonName[0]=='C') {
            Shidunzi s(split, total_beat, beat, 0, 'T');
            push(s);
        }
        else {
            button += (int)(buttonName[1]-'1');
            m.ext.push_back({total_beat,m.nowTouchSet[button]});
        }
    }


    start = content.find("\"toucheHolds\":");    //寻找touchHold列表
    if (start == npos)
        return -1;

    start += 10;
    end = content.find(']', start);
    std::string touchHold_str = content.substr(start, end - start);

    pos = 0;
    while (pos != npos)
    {
        pos = touchHold_str.find("\"hitTime\":", pos);
        size_t end = touchHold_str.find("}", pos);
        if (end == npos)
            break;

        std::string touchHold = touchHold_str.substr(pos, end - pos + 1);
        pos = end + 1;

        int split = extractValue(touchHold, "\"split\":");   //分音
        int beat = extractValue(touchHold, "\"beat\":");     //拍数

        if (split <= 0 || beat <= 0)
            continue;           //剔除无效项
        beat *= 4;
        float total_beat = static_cast<float>(beat) / split;

        Shidunzi s(split, total_beat, beat, 0, 'H');
        push(s);

        pos = touchHold_str.find("\"holdTime\":", pos);
        end = touchHold_str.find("}", pos);
        if (end == npos)
            break;

        touchHold = touchHold_str.substr(pos, end - pos + 1);
        pos = end + 1;

        beat += 4 * extractValue(touchHold, "\"beat\":");     //新增拍数

        if (split <= 0 || beat <= 0)
            continue;           //剔除无效项（应该没有吧？）
        total_beat = static_cast<float>(beat) / split;

        Shidunzi s2(split, total_beat, beat, 0, 'H');
        push(s2);
    }

    return 0;
}

// 这是待会要用到的神秘妙妙工具
static std::string note_str;
static void erase (int && i){
    note_str.erase(0,i);
}

// 以Simai格式读取
int ShidunziStack::read_as_Simai(const std::string& precontent, MainData& m)
{
    constexpr int denominator = 1920;
    stones.clear();
    m.exs.clear();
    m.ext.clear();
    // 读取歌曲信息
    std::string d_str = std::to_string(m.importDiff+1)+"=";
    m.SongInfo.title = extractString(precontent, "&title=", '\n');
    m.SongInfo.author = extractString(precontent, "&artist=", '\n');
    m.SongInfo.offset = extractFloat(precontent, "&first=");
    m.SongInfo.level = extractString(precontent, "&lv_" + d_str, '\n');
    m.SongInfo.mapper = extractString(precontent, "&des_" + d_str, '\n');

    // 提取note数组
    size_t start = 0, end = 0;
    start = precontent.find("&inote_" + d_str);
    d_str = std::to_string(m.importDiff+2)+"=";
    end = precontent.find("E\n");

    if (start == npos)
        return -1;

    start += 9;
    QString content = QString::fromStdString(precontent.substr(start, end - start + 1));
    QStringList cut_content = content.split(',');
    int step = 0;       // 设置步进
    int beat = 0;       // 设置初始拍子
    char now = 0;
    bool isNoMulti = false;   //伪双押标志位
    for (auto & notes : cut_content)
    {
        note_str = notes.toStdString();
        while (!note_str.empty())
        {
            end = 0;
            now = note_str[0];
            // 如果第一个字符是1到8，说明是一个按键
            if (now >= '1' && now <= '8')
            {
                bool isBreak = (note_str[1] == 'b');
                bool isEx = (note_str[1] == 'x' || note_str[2] == 'x');
                int button = (now - '0');
                if (button >= 4 && button <= 6)
                {
                    char C = isEx ? 'S' : (isBreak ? 'X' : 'D');
                    Shidunzi s (denominator, beat, button - 3, C);
                    push(s);
                    stones.back().isNoMulti = isNoMulti;
                    isNoMulti = false;
                }
                else
                {
                    m.exs.push_back({ (float)beat / denominator, isEx });
                }
                erase(isBreak + isEx + 1);
                continue;
            }
            // 如果第一个字符是A到E，说明是一个touch
            if (now >= 'A' && now <= 'E')
            {
                // 由于一个难度的谱面以E结尾，所以只剩E的时候要返回，否则数组越界
                if (now == 'E' && note_str.length()==1)
                    return 0;
                // 如果第二个字符是h，说明是touchHold，由于他吃厚的总是最后一个元素，所以可以直接break
                if (note_str.length()>1 && note_str[1] == 'h')
                {
                    Shidunzi s(denominator, beat, 0, 'H');
                    push(s);
                    // 差不多得了，用英语还没拼音看得懂，什么denominator，numerator的
                    int FenMu = extractValue(note_str,"[");
                    int FenZi = extractValue(note_str,":");
                    Shidunzi s2(denominator, beat + (denominator * 4 * FenZi / FenMu), 0, 'H');
                    push(s2);
                    break;
                }
                int button = 0;
                switch (now)
                {
                case 'A': break;
                case 'B': { button+=8; break; }
                case 'C': { button+=16; break; }
                case 'D': { button+=17; break; }
                case 'E': { button+=25; break; }
                }
                if (now=='C')
                {
                    Shidunzi s(denominator, beat, 0, 'T');
                    push(s);
                }
                else
                {
                    button += (int)(note_str[1]-'1');
                    end = 1;
                    m.ext.push_back({ (float)beat / denominator,m.nowTouchSet[button] });
                }
                erase(end + 1);
                continue;
            }
            // 如果第一个字符是左大括号，说明改变了分音
            if (now == '{')
            {
                end = note_str.find('}');
                step = denominator * 4 / std::stoi(note_str.substr(1, end));
                erase(end + 1);
                continue;
            }
            // 如果第一个字符是左小括号，说明是一个BPM
            if (now == '(')
            {
                end = note_str.find(')');
                float bpm = std::stof(note_str.substr(1, end));
                if (beat == 0)
                    m.SongInfo.bpm = bpm;
                else
                {
                    Shidunzi s (denominator, beat, bpm, 'B');
                    push(s);
                }
                erase(end + 1);
                continue;
            }
            // 如果第一个字符是h或[，说明谱面里有hold，异常退出
            if (now == 'h' || now == '[')
            {
                return -1;
            }
            // 如果第一个字符是`，说明下一个note是伪双押
            if (now == '`')
            {
                isNoMulti = true;
            }
            // 如果都不是，那就直接跳过该字符
            erase(1);
        }
        beat += step;       // 直到note_str为空，则只剩逗号，步进
    }
    return 0;
}

// 以Osu!Mania格式读取
int ShidunziStack::read_as_OSU(const std::string& precontent, MainData& m)
{
    constexpr int denominator = 1920;

    stones.clear();
    m.exs.clear();
    m.ext.clear();
    // 解析元数据
    m.SongInfo.title = extractString(precontent, "Title:");
    m.SongInfo.author = extractString(precontent, "Artist:");
    m.SongInfo.mapper = extractString(precontent, "Creator:");

    size_t start = 0;

    start = precontent.find("[TimingPoints]");

    QString first_bpm = QString::fromStdString(precontent.substr(start+16,start+50));
    QStringList cut_first_bpm = first_bpm.split(',');

    m.SongInfo.offset = cut_first_bpm[0].toFloat() / 1000;
    m.SongInfo.bpm = 60000.0f / cut_first_bpm[1].toFloat();

    int key_mode = extractValue(precontent, "CircleSize:");

    // 解析HitObjects
    start = precontent.find("[HitObjects]");         //寻找note列表
    if (start == npos)
        return -1;

    start += 14;
    QString notes_str = QString::fromStdString(precontent.substr(start));
    QStringList notes_list = notes_str.split("\r\n");

    for(auto & notes : notes_list)
    {
        if (!notes.isEmpty())
        {
            QStringList cut_notes = notes.split(',');
            int x = cut_notes[0].toInt();
            float time = cut_notes[2].toFloat() / 1000 ;
            int obj_type = cut_notes[3].toInt();

            int track = x / (512.0f / key_mode) + 1;

            float total_beat = (time - m.SongInfo.offset) * (m.SongInfo.bpm / 60);
            float beat = total_beat * denominator;
            beat = (int)beat % denominator;
            if (beat > 1910 || beat < 10)
                beat = 0;
            total_beat = std::round(total_beat) + beat / denominator;

            if (track >= 1 && track <= 3)
            {
                Shidunzi s(denominator, total_beat, beat, track, obj_type==128);
                push(s);
            }
            else
            {
                m.exs.push_back({ total_beat, false });
            }
        }
    }

    return 0;
}

// 输出
void ShidunziStack::output(std::stringstream & buffer)
{
    Shidunzi::resetTag_index();
    std::for_each(stones.begin(), stones.end(), [&buffer](Shidunzi& s){s.output(buffer);});
}
