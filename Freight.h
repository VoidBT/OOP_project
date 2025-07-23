#pragma once
#include <string>
using namespace std;

enum class FreightType { MINI_MOVER, CARGO_CRUISER, MEGA_CARRIER };

class Freight {
private:
    string id;
    int time;
    string dest;
    FreightType type;

public:
    Freight(const string& id, int time, const string& dest, FreightType type);

    string getID() const;
    int getTime() const;
    string getDest() const;
    FreightType getType() const;
    string getTypeString() const;

    void setTime(int newTime);
    void setDest(const string& newDest);
    void setType(FreightType newType);

    static string typeToString(FreightType type);
};
