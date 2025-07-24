#pragma once
#include <string>
#include "Time.h"

using namespace std;

enum class FreightType { MINI_MOVER, CARGO_CRUISER, MEGA_CARRIER };

class Freight {
private:
    string id;
    Time time;
    string dest;
    FreightType type;

public:
    Freight();
    Freight(const string& id, int time, const string& dest, FreightType type);

    string getID() const;
    Time getTime() const;
    string getDest() const;
    FreightType getType() const;
    string getTypeString() const;

    void setTime(int newTime);
    void setDest(const string& newDest);
    void setType(FreightType newType);

    static string typeToString(FreightType type);
};
