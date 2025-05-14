#pragma once
#include "shidunzi.h"
#include "maindata.h"
#include <queue>

class ShidunziStack
{
private:
    std::deque<Shidunzi> stones;
    std::vector<Shidunzi> stonesFound;

public:
    ShidunziStack() = default;
    inline bool empty() const
        { return stones.empty(); }
    inline void clear()
        { stones.clear(); }
    inline void push(const Shidunzi& s)
        { stones.push_back(s); }

    inline const std::deque<Shidunzi>& getStones() const
        { return stones; }
    inline float getMaxBeat()
        { return (stones.empty() ? 0 : stones.back().beat) ;}
    int getNumber();
    int getNumber(char);
    int getPureNumber();
    int getPureNumber(char);

    void append(std::vector<Shidunzi>&);
    void countAdjust(const ExStone& exs);
    void positionAdjust(const ExTouch& ext);
    void sortByTotalBeat();
    void gcd();
    void bpmAdjust();

    int read_as_VM(const std::string& content, MainData& m);
    int read_as_OSU(const std::string& precontent, MainData& m);
    int read_as_Simai(const std::string& precontent, MainData& m);

    void output(std::stringstream & buffer);
};
