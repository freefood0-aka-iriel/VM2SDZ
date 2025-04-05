#include "shidunzi.h"

Shidunzi::Shidunzi(int dom, float tb, int b, float t, bool iB = false)
{
    type = iB ? 'X' : 'D';
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
    bool positive = true;
    if (pos < s.size() && s[pos] == '-')
    {
        positive = false;
        pos++;
    }
    while (pos < s.size() && isdigit(s[pos]))
    {
        value = value * 10 + (positive ? (s[pos] - '0') : -(s[pos] - '0'));
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
            value += positive ? dig : -dig;
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
    return s.substr(pos, end - pos);
}
