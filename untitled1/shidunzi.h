#pragma once
#include <string>
#include <sstream>
#include <QMainWindow>
#include <QDebug>
#include "fraction.h"

const float EPSILON = 0.0001f;

class Shidunzi
{
private:
    char type;      //属性
    Fraction beat;  //拍数，是一个分数类
    float track;
    int count = 1;
    int deleteCount = 0;
    float size = 1;
    float yOffset = 0;
    //状态变量
    bool isNew = true;
    bool isNoMulti = false;
    //set func
    void Set(const int dom, const int b, const float t, const char type);
    //static vars
    static int tag_index;
public:
    //construtor & operator=
    Shidunzi(const int, const int, const float, const bool);
    Shidunzi(const int, const int, const float, const char);
    Shidunzi(const Shidunzi&);
    const Shidunzi& operator=(const Shidunzi & s);
    //getters
    inline char getType() const { return type; }
    inline float getBeat() const { return beat.tofloat(); }
    inline float getTrack() const { return track; }
    inline int getCount() const { return count; }
    inline int getDeleteCount() const { return deleteCount; }
    inline float getSize() const { return size; }
    inline float getYOffset() const { return yOffset; }
    //setters
    inline void setType(const char C) { type = C; }
    inline void setTrack(const float F) { track = F; }
    inline void setYOffset(const float F) { yOffset = F; }
    inline void setSize(const float F) { size = F; }
    static inline void resetTag_index() { tag_index = 0; }
    //methods
    inline void filp() { track = 4 - track; }
    void input (const QString&);
    void output (std::stringstream&);
    //friends
    friend class ShidunziStack;
    friend bool operator<(const Shidunzi& s1,const Shidunzi& s2);
    friend bool operator>(const Shidunzi& s1,const Shidunzi& s2);
};

struct ExStone
{
    Fraction beat;
    bool isEx;
};

struct TouchSet
{
    std::string name;
    float xOffset;
    float yOffset;
    float size;
};

struct ExTouch
{
    Fraction beat;
    TouchSet ts;
};

int extractValue(const std::string& s, const std::string& key);
bool extractBool(const std::string& s, const std::string& key);
float extractFloat(const std::string& s, const std::string& key);
std::string extractString(const std::string& s, const std::string& key);
std::string extractString(const std::string& s, const std::string& key, const char&);
