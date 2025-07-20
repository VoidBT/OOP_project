#include "Cargo.h"

Cargo::Cargo(const std::string& id, int time, const std::string& dest)
    : id(id), time(time), dest(dest) {
}

std::string Cargo::getID() const { return id; }
int Cargo::getTime() const { return time; }
std::string Cargo::getDest() const { return dest; }

void Cargo::setTime(int newTime) { time = newTime; }
void Cargo::setDest(const std::string& newDest) { dest = newDest; }