#include "Freight.h"

Freight::Freight(const std::string& id, int time, const std::string& dest)
    : id(id), time(time), dest(dest) {
}

std::string Freight::getID() const { return id; }
int Freight::getTime() const { return time; }
std::string Freight::getDest() const { return dest; }

void Freight::setTime(int newTime) { time = newTime; }
void Freight::setDest(const std::string& newDest) { dest = newDest; }