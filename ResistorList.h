/* 
 * File:   ResistorList.h
 * Author: songshu3
 *
 * Created on October 29, 2013, 2:31 PM
 */

#ifndef RESISTORLIST_H
#define	RESISTORLIST_H

#include "Resistor.h"

class ResistorList
{
private:
    int numRes;
    Resistor *head;
    
public:
    ResistorList();
    ~ResistorList();
    int getNumRes();
    void setNumRes(int numRes_);
    Resistor* getHead();
    Resistor* insertResistor(string name, double resistance, int nodeid1, int nodeid2);
    Resistor* findResistor(string name);
    void printResistor();
    bool deleteResistor(string name);
};

#endif	/* RESISTORLIST_H */

