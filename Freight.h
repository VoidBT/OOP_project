#pragma once
#include <string>
#include "Logistic.h"

using namespace std;

class Freight : public Logistic {
private:
    int time;
    string dest;
public:
    Freight(const string& id, int t, const string& d);
    int getTime() const;
    string getDest() const;
    void setTime(int t);
    void setDest(const string& d);
    void setTimeAndDest(int t2, const string& d2);
};