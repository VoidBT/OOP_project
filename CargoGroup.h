#include "Cargo.h"
#include <vector>
#include <string>

using namespace std;

class CargoGroup {
private:
    string groupId;
    string destination;
    int maxSize;
    vector<Cargo> cargos;
    int timeWindow; // in minutes

public:
    CargoGroup(const string& id, const string& dest, int maxSize = 10, int timeWindow = 15);

    bool addCargo(const Cargo& cargo);
    bool removeCargo(const string& cargoId);
    bool containsCargo(const string& cargoId) const;
    bool isFull() const;
    bool isEmpty() const;
    bool canMergeWith(const CargoGroup& other) const;

    string getGroupId() const;
    string getDestination() const;
    int getSize() const;
    int getMaxSize() const;
    int getTimeWindow() const;
    const vector<Cargo>& getCargos() const;

    void setDestination(const string& newDest);
    void setTimeWindow(int newWindow);
};
