#pragma once // Include guard
#include <string>

class Cargo {
private:
    std::string id;
    int time;
    std::string dest;

public:
    Cargo(const std::string& id, int time, const std::string& dest);
    std::string getID() const;
    int getTime() const;
    std::string getDest() const;
    void setTime(int newTime);
    void setDest(const std::string& newDest);
};