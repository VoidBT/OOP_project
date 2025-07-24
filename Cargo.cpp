#include "Cargo.h"

Cargo::Cargo() : id(), time(), dest(), size() {

}
// NEW: Update constructor implementation
Cargo::Cargo(const string& id, int time, const string& dest, int size)
    : id(id), time(time), dest(dest), size(size) {
} // Initialize size

string Cargo::getID() const { return id; }
Time Cargo::getTime() const { return time; }
string Cargo::getDest() const { return dest; }
int Cargo::getSize() const { return size; } // NEW: Implement getSize

void Cargo::setTime(int newTime) { time = newTime; }
void Cargo::setDest(const string& newDest) { dest = newDest; }
void Cargo::setSize(int newSize) { size = newSize; } // NEW: Implement setSize