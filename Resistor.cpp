#include <iomanip>
#include <iostream>
#include <string>
#include "Resistor.h"

using namespace std;

Resistor::Resistor()
{
    
}

Resistor::Resistor(string name_, double resistance_, int nodeid1_, int nodeid2_)
{
    name = name_;
    resistance = resistance_;
    nodeid1 = nodeid1_;
    nodeid2 = nodeid2_;
    if(nodeid1 < nodeid2)
    {
        nodeidL = nodeid1;
        nodeidH = nodeid2;
    }
    else
    {
        nodeidL = nodeid2;
        nodeidH = nodeid1;
    }
    next = NULL;    
}
Resistor::~Resistor()
{
    
}

string Resistor::getName()
{
    return name;
}

void Resistor::setName(string name_)
{
    name = name_;
}

double Resistor::getResistance()
{
    return resistance;
}

void Resistor::setResistance(double resistance_)
{
    resistance = resistance_;
}

int Resistor::getNodeID1()
{
    return nodeid1;
}

int Resistor::getNodeID2()
{
    return nodeid2;
}

int Resistor::getNodeIDL()
{
    return nodeidL;
}

int Resistor::getNodeIDH()
{
    return nodeidH;
}

void Resistor::setNext(Resistor* next_)
{
    next = next_;
}

Resistor* Resistor::getNext()
{
    return next;
}

bool Resistor::compareName(string n)  // Check if the resistor name is the same as string n
{
    return !name.compare(n);
}

void Resistor::print()  // Print this resistor in the desired format
{
    cout << left << setw(20) << name << " " << right << setw(8) << std::fixed <<
            std::setprecision(2) << resistance << " Ohms " << nodeid1 <<
            " -> " << nodeid2 << endl;
}
