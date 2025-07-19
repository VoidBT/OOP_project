#include "Freight.h"
#include <string>

using namespace std;

Freight::Freight(const string& id, int t, const string& d) : Logistic(id), time(t), dest(d) {}

int Freight::getTime() const {
    return time;
}

string Freight::getDest() const {
    return dest;
}

void Freight::setTime(int t) {
    time = t;
}

void Freight::setDest(const string& d) {
    dest = d;
}

void Freight::setTimeAndDest(int t2, const string& d2) {
    time = t2;
    dest = d2;
}