#ifndef SHIDUNZISTACK_H
#define SHIDUNZISTACK_H

#include "shidunzi.h"

class ShidunziStack
{
private:
    std::vector<Shidunzi> stones;

public:
    ShidunziStack() = default;
    inline bool empty() const { return stones.empty(); }
    inline void clear() { stones.clear(); }
    inline void push(const Shidunzi& s) { stones.push_back(s); }
    inline const std::vector<Shidunzi>& getStones() const { return stones; }
    void mergeShidunzi(std::vector<Shidunzi>&);
    void incrementCountForBeat(ExStone exs);
    void incrementPositionForBeat(ExTouch ext);
    void sortByTotalBeat();
    void resetBeatsAfterChange();
    void read_as_VM(const std::string& content, std::vector<ExStone>& exs, std::vector<ExTouch>& ext, SongInfomation& SongInfo, int d=4);
};

inline int gcd(int, int);
inline int lcm(int, int);
inline int mod(int, int);

#endif // SHIDUNZISTACK_H
