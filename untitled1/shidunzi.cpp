#include "shidunzi.h"

Shidunzi::Shidunzi(int dom, int b, float t, bool iB = false)
{
    type = iB ? 'X' : 'D';
    beat = static_cast<float>(b) / dom;
    numerator = b % dom;
    denominator = dom;
    track = t;
}

Shidunzi::Shidunzi(int dom, int b, float t, char type = 'D')
{
    Shidunzi::type = type;
    beat = static_cast<float>(b) / dom;
    numerator = b % dom;
    denominator = dom;
    track = t;
}

Shidunzi::Shidunzi(int dom, float tb, int b, float t, bool iB = false)
{
    type = iB ? 'X' : 'D';
    beat = tb;
    numerator = b % dom;
    denominator = dom;
    track = t;
}

Shidunzi::Shidunzi(int dom, float tb, int b, float t, char type = 'D')
{
    Shidunzi::type = type;
    beat = tb;
    numerator = b % dom;
    denominator = dom;
    track = t;
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
    if (end - pos > 30)
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

void Shidunzi::input(const QString & Qstr)
{
    QStringList QstrList = Qstr.split(',');
    type = QstrList[0].toStdString()[0];
    beat = QstrList[1].toInt() + QstrList[2].toFloat() / QstrList[3].toInt();
    numerator = QstrList[2].toInt();
    denominator = QstrList[3].toInt();
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

void Shidunzi::output(std::stringstream & buffer)
{
    buffer << type << "," << static_cast<int>(beat) << ","
           << numerator << "," << denominator;
    if (type=='H')
        buffer << "\n";
    else if (type=='B')
        buffer << "," << track << "\n";
    else
        buffer << "," << track << "," << count << ","
               << deleteCount << "," << size << "," << yOffset << "\n";
}
