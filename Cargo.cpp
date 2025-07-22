#include "Cargo.h"

// NEW: Update constructor implementation
Cargo::Cargo(const std::string& id, int time, const std::string& dest, int size)
    : id(id), time(time), dest(dest), size(size) {
} // Initialize size

std::string Cargo::getID() const { return id; }
int Cargo::getTime() const { return time; }
std::string Cargo::getDest() const { return dest; }
int Cargo::getSize() const { return size; } // NEW: Implement getSize

void Cargo::setTime(int newTime) { time = newTime; }
void Cargo::setDest(const std::string& newDest) { dest = newDest; }
void Cargo::setSize(int newSize) { size = newSize; } // NEW: Implement setSize