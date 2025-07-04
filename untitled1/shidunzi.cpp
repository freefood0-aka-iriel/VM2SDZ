#include "shidunzi.h"
//#include <QDebug>

int Shidunzi::tag_index = 0;

void Shidunzi::Set(const int dom, const int b, const float t, const char type)
{
    beat = Fraction(b,dom);
    track = t;
    Shidunzi::type = type;
}

Shidunzi::Shidunzi(const int dom, const int b, const float t, const bool iB = false)
    :beat(b,dom)
{
    type = iB ? 'X' : 'D';
    track = t;
}

Shidunzi::Shidunzi(const int dom, const int b, const float t, const char type)
    :beat(b,dom)
{
    track = t;
    Shidunzi::type = type;
}

Shidunzi::Shidunzi(const Shidunzi& s)
    :beat(s.beat)
{
    track = s.track;
    type = s.type;
    count = s.count;
    deleteCount = s.deleteCount;
    yOffset = s.yOffset;
    size = s.size;
}

const Shidunzi& Shidunzi::operator=(const Shidunzi & s)
{
    beat = s.beat;
    track = s.track;
    type = s.type;
    count = s.count;
    deleteCount = s.deleteCount;
    yOffset = s.yOffset;
    size = s.size;
    return *this;
}

bool operator<(const Shidunzi& s1,const Shidunzi& s2)
{
    return s1.beat < s2.beat;
}

bool operator>(const Shidunzi& s1,const Shidunzi& s2)
{
    return s1.beat > s2.beat;
}

// 从字符串提取整数值
int extractValue(const std::string& s, const std::string& key)
{
    size_t pos = s.find(key);
    if (pos == std::string::npos)
        return -1;
    pos += key.length();

    while (pos < s.size() && (s[pos] == ':' || s[pos] == ' ' || s[pos] == '"' || s[pos] == ','))
        pos++;

    int value = 0;
    while (pos < s.size() && isdigit(s[pos]))
    {
        value = value * 10 + (s[pos] - '0');
        pos++;
    }
    return value;
}

// 从字符串提取布尔值
bool extractBool(const std::string& s, const std::string& key)
{
    size_t pos = s.find(key);
    if (pos == std::string::npos)
        return false;
    pos += key.length();

    while (pos < s.size() && (s[pos] == ':' || s[pos] == ' ' || s[pos] == '"' || s[pos] == ','))
        pos++;

    return s.substr(pos, 4) == "true";
}

// 从字符串提取浮点值
float extractFloat(const std::string& s, const std::string& key)
{
    size_t pos = s.find(key);
    if (pos == std::string::npos)
        return 3.4E+38*2*0;
    pos += key.length();

    while (pos < s.size() && (s[pos] == ':' || s[pos] == ' ' || s[pos] == '"' || s[pos] == ','))
        pos++;

    float value = 0.0f;
    int positive = 1;
    if (pos < s.size() && s[pos] == '-')
    {
        positive = -1;
        pos++;
    }
    while (pos < s.size() && isdigit(s[pos]))
    {
        value = value * 10 + (positive * (s[pos] - '0'));
        pos++;
    }
    if (pos < s.size() && s[pos] == '.')
    {
        pos++;
        int dec = 1;
        while (pos < s.size() && isdigit(s[pos]))
        {
            float dig = (s[pos] - '0');
            for (int i = 0; i<dec; i++)
                dig /= 10;
            value += positive * dig;
            pos++;
            dec++;
        }
    }
    return value;
}

// 从字符串提取字符串值
std::string extractString(const std::string& s, const std::string& key)
{
    size_t pos = s.find(key);
    if (pos == std::string::npos)
        return "";
    pos += key.length();

    while (pos < s.size() && (s[pos] == ':' || s[pos] == ' ' || s[pos] == '"' || s[pos] == ','))
        pos++;

    size_t end = s.find('"', pos);
    if (end == std::string::npos)
        return "";
    if (end - pos > 100)
        end = s.find('\n', pos);
    return s.substr(pos, end - pos);
}
// 从字符串提取字符串值，设置结束标志
std::string extractString(const std::string& s, const std::string& key, const char& end_target='"')
{
    size_t pos = s.find(key);
    if (pos == std::string::npos)
        return "";
    pos += key.length();

    while (pos < s.size() && (s[pos] == ':' || s[pos] == ' ' || s[pos] == '"' || s[pos] == ','))
        pos++;

    size_t end = s.find(end_target, pos);
    if (end == std::string::npos)
        return "";
    return s.substr(pos, end - pos);
}

//static void checkDefault(QString & Qstr, const QChar && defaultValue)
//{
//    if(Qstr.isEmpty())
//        Qstr.append(defaultValue);
//}

// 从文本中读取石墩子
void Shidunzi::input(const QString & Qstr)
{
    QStringList QstrList = Qstr.split(',');
    // 如果有数据是空的，填入默认值
    // 填入值
    type = QstrList[0].toStdString()[0];
    beat = Fraction(QstrList[1].toInt() * QstrList[3].toInt() + QstrList[2].toFloat(), QstrList[3].toInt());
    if (type=='H')
        return;
    track = QstrList[4].toFloat();
    if (type=='B')
        return;
    count = QstrList[5].toInt();
    deleteCount = QstrList[6].toInt();
    size = QstrList[7].toFloat();
    yOffset = QstrList[8].toFloat();
}

// 输出为文本
void Shidunzi::output(std::stringstream & buffer)
{
    // 输出标记类型
    if (type == 'T') {
        buffer << "//Tag " << tag_index << " at " << beat.toMixedFraction() << "\n";
        tag_index++;
        return;
    }
    // 通用输出
    buffer << type << "," << beat.toMixedFraction();
    // 输出表演类型
    if (type == 'H') {
        buffer << "\n";
        return;
    }
    // 输出BPM变化类型
    else if (type == 'B') {
        buffer << "," << track << "\n";
        return;
    }
    // 输出石墩子类型
    else {
        buffer << "," << track << "," << count << ","
               << deleteCount << "," << size << "," << yOffset << "\n";
    }
}
