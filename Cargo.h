#pragma once
#include <string>

using namespace std;

class Cargo {
private:
    string id;
    int time;
    string dest;
    int size; // NEW: Add size member

public:
    Cargo();
	// NEW: Update constructor to accept size
    Cargo(const string& id, int time, const string& dest, int size);
    string getID() const;
    int getTime() const;
    string getDest() const;
    int getSize() const; // NEW: Add getter for size
    void setTime(int newTime);
    void setDest(const string& newDest);
    void setSize(int newSize); // NEW: Add setter for size
};