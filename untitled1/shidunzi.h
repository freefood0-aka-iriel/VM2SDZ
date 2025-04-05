#ifndef SHIDUNZI_H
#define SHIDUNZI_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <cmath>

const float EPSILON = 0.0001f;

class Shidunzi
{
private:
    char type;
    float beat;
    int numerator;
    int denominator;
    float track;
    int count = 1;
    int deleteCount = 0;
    float size = 1;
    float yOffset = 0;
public:
    //construtor
    Shidunzi(int, float, int, float, bool);
    //getters
    inline char getType() const { return type; }
    inline float getBeat() const { return beat; }
    inline int getNumerator() const { return numerator; }
    inline int getDenominator() const { return denominator; }
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
    //friends
    friend class ShidunziStack;
};

struct SongInfomation {
    std::string title;
    std::string level;
    std::string author;
    std::string mapper;
    float offset = 0;
    float bg_offset = -0.54f;
    float bpm = 125;
};

struct ExStone
{
    float beat;
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
    float beat;
    TouchSet ts;
};

const TouchSet defaultTouchSet[33]{
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
int extractValue(const std::string& s, const std::string& key);
bool extractBool(const std::string& s, const std::string& key);
float extractFloat(const std::string& s, const std::string& key);
std::string extractString(const std::string& s, const std::string& key);

#endif // SHIDUNZI_H
