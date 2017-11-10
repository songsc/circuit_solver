#include <iostream>
#include "Node.h"
#include "Resistor.h"

using namespace std;

Node::Node()
{

}

Node::Node(int nodeid_)
{
    nodeid = nodeid_;
    next = NULL;
    set = false;
    voltage = 0;
    change = 1;    
}

Node::~Node()
{
    resL.~ResistorList();
}

int Node::getNodeID()
{
    return nodeid;
}

Node* Node::getNext()
{
    return next;
}

void Node::setNext(Node* next_)
{
    next = next_;
}

ResistorList* Node::getResList()
{
    return &resL;
}

bool Node::addResistor (string name_, double resistance_, int endpoints_1, int endpoints_2)
{
    resL.insertResistor(name_, resistance_, endpoints_1, endpoints_2);
    return true;
}

Resistor* Node::findResistor(string name)
{
    return (resL.findResistor(name));
}

void Node::printNode()
{
    if(resL.getNumRes() == 0)
        return;
    cout << "Connections at node " << nodeid << ": " <<
            resL.getNumRes() << " resistor(s)" << endl;
    resL.printResistor();
}

bool Node::deleteResistor(string name)
{
    resL.deleteResistor(name);
    return true;
}

bool Node::getSet()
{
    return set;
}

void Node::setSet(bool s)
{
    set = s;
}

double Node::getVoltage()
{
    return voltage;
}

void Node::setVoltage(double v)
{
    voltage = v;
}

double Node::getChange()
{
    return change;
}

void Node::setChange(double change_)
{
    change = change_;
}

int Node::getNumRes()
{
    return resL.getNumRes();
}

int Node::getX()
{
    return x;
}

void Node::setX(int x_)
{
    x = x_;
}
