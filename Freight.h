#pragma once // Include guard
#include <string>
using namespace std;

class Freight {
private:
    string id;
    int time;
    string dest;
    string type; // NEW: Freight type

public:
    Freight(const string& id, int time, const string& dest, const string& type);

    string getID() const;
    int getTime() const;
    string getDest() const;
    string getType() const; // NEW: Getter for type

    void setTime(int newTime);
    void setDest(const string& newDest);
    void setType(const string& newType); // Optional: Setter for type
};
