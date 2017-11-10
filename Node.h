#ifndef NODE_H
#define	NODE_H

#include "Resistor.h"
#include "ResistorList.h"


class Node
{
private:
    int nodeid;
    Node* next;
    ResistorList resL;
    bool set;
    double voltage;
    double change;
    int x;

public:
    Node();
    Node(int nodeid_);
    ~Node();
    int getNodeID();
    Node* getNext();
    void setNext(Node* next_);    
    ResistorList* getResList();
    bool addResistor (string name_, double resistance_, int endpoints_1, int endpoints_2);
    Resistor* findResistor(string name);
    void printNode();
    bool deleteResistor(string name);
    bool getSet();
    void setSet(bool s);
    double getVoltage();
    void setVoltage(double v);
    double getChange();
    void setChange(double change_);
    int getNumRes();
    int getX();
    void setX(int x_);
    
};


#endif	/* NODE_H */

