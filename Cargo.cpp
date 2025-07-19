#include "Cargo.h"
#include <string>

using namespace std;

Cargo::Cargo(const string& id, int t, const string& d) : Logistic(id), time(t), dest(d) {}

int Cargo::getTime() const {
    return time;
}

string Cargo::getDest() const {
    return dest;
}

void Cargo::setTime(int t) {
    time = t;
}

void Cargo::setDest(const string& d) {
    dest = d;
}