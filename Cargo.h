#pragma once
#include <string>
#include "Logistic.h"

using namespace std;

class Cargo : public Logistic {
private:
    int time;
    string dest;
public:
    Cargo(const string& id, int t, const string& d);
    int getTime() const;
    string getDest() const;
    void setTime(int t);
    void setDest(const string& d);
};