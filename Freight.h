#pragma once // Include guard
#include <string>

class Freight {
private:
    std::string id;
    int time;
    std::string dest;

public:
    Freight(const std::string& id, int time, const std::string& dest);
    std::string getID() const;
    int getTime() const;
    std::string getDest() const;
    void setTime(int newTime);
    void setDest(const std::string& newDest);
};