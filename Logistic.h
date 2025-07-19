#pragma once
#include <string>

using namespace std;

class Logistic {
protected:
    string ID;
public:
    Logistic(const string& id);
    virtual ~Logistic();
    string getID() const;
};