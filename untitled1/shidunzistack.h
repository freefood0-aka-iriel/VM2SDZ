#ifndef SHIDUNZISTACK_H
#define SHIDUNZISTACK_H

#include "shidunzi.h"

class ShidunziStack
{
private:
    std::vector<Shidunzi> stones;
    std::vector<Shidunzi> stonesFound;

public:
    ShidunziStack() = default;
    inline bool empty() const
        { return stones.empty(); }
    inline void clear()
        { stones.clear(); }
    inline void push(const Shidunzi& s)
        { stones.push_back(s); }

    inline const std::vector<Shidunzi>& getStones() const
        { return stones; }
    inline float getMaxBeat()
        { return (stones.empty() ? 0 : stones.back().beat) ;}
    int getNumber();
    int getNumber(char);
    int getPureNumber();
    int getPureNumber(char);

//    void find(int,int);
//    void find(float,int);
//    void find(char,int);

    void mergeShidunzi(std::vector<Shidunzi>&);
    void countAdjust(ExStone exs);
    void positionAdjust(ExTouch ext);
    void sortByTotalBeat();
    void gcd();
    void bpmAdjust();

    void read_as_VM(const std::string& content, std::vector<ExStone>& exs, std::vector<ExTouch>& ext, SongInfomation& SongInfo, int d);
    void read_as_OSU(const std::string& precontent, std::vector<ExStone>& exs, std::vector<ExTouch>& ext, SongInfomation& SongInfo);
    int read_as_Simai(const std::string& precontent, std::vector<ExStone>& exs, std::vector<ExTouch>& ext, SongInfomation& SongInfo, int d);

    void output(std::stringstream & buffer);
};

inline int gcd(int, int);
inline int lcm(int, int);
inline int mod(int, int);

#endif // SHIDUNZISTACK_H
