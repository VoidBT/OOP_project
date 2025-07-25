#pragma once
#include <string>
#include "Time.h"

//Done by Brendan Tjung Yew Wang 2401765

using namespace std;

class Cargo {
private:
    string id;
    Time time;
    string dest;
    int size; // NEW: Add size variable for  Cargo

public:
    Cargo();
	// NEW: Update constructor to accept size
    Cargo(const string& id, int time, const string& dest, int size);
    string getID() const;
    Time getTime() const;
    string getDest() const;
    int getSize() const; // NEW: Add getter for size
    void setTime(int newTime);
    void setDest(const string& newDest);
    void setSize(int newSize); // NEW: Add setter for size
};