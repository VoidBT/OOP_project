#pragma once
#include <string>

class Cargo {
private:
    std::string id;
    int time;
    std::string dest;
    int size; // NEW: Add size member

public:
	// NEW: Update constructor to accept size
    Cargo(const std::string& id, int time, const std::string& dest, int size);
    std::string getID() const;
    int getTime() const;
    std::string getDest() const;
    int getSize() const; // NEW: Add getter for size
    void setTime(int newTime);
    void setDest(const std::string& newDest);
    void setSize(int newSize); // NEW: Add setter for size
};