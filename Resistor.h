/* 
 * File:   Resistor.h
 * Author: songshu3
 *
 * Created on October 29, 2013, 2:30 PM
 */

#ifndef RESISTOR_H
#define	RESISTOR_H

#include <string>
using namespace std;

class Resistor
{
private:
    string name;
    double resistance;
    int nodeid1, nodeid2, nodeidL, nodeidH;
    Resistor *next;
        
public:
    Resistor();
    Resistor(string name_, double resistance_, int nodeid1_, int nodeid2_);
    ~Resistor();
    string getName();
    void setName(string name_);
    double getResistance();
    void setResistance(double resistance_);
    int getNodeID1();
    int getNodeID2();
    int getNodeIDL();
    int getNodeIDH();
    void setNext(Resistor* next);
    Resistor* getNext();    
    bool compareName(string n);
    void print();
};

#endif	/* RESISTOR_H */

