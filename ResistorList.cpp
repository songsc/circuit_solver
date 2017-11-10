/*
 * File: ResistorList.cpp
 * Author: Shuangchen Song (999931052)
 * 
 * Created on October 29, 2013
 */

#include <string>
#include "Resistor.h"
#include "ResistorList.h"
using namespace std;

ResistorList::ResistorList()
{
    numRes = 0;
    head = NULL;
}

ResistorList::~ResistorList()
{
    Resistor* p;
    while(head != NULL)  // While the list is not empty
    {
        p = head;
        head = p->getNext();  // Move forward
        delete p;  // Delete the original head
        this->setNumRes(-1);
    }
}

int ResistorList::getNumRes()
{
    return numRes;
}

void ResistorList::setNumRes(int numRes_)
{
    if(numRes_ == 0)
        numRes = 0;
    else
        numRes = numRes + numRes_;
}

Resistor* ResistorList::getHead()
{
    return head;
}

Resistor* ResistorList::insertResistor(string name, double resistance, int nodeid1, int nodeid2)
{
    Resistor* p = head;
    Resistor* prev = NULL;
    Resistor* n = new Resistor(name, resistance, nodeid1, nodeid2);
    
    while((p != NULL) && (p->getName() < name))  // If p have not yet reached the end of the list, 
                                                 // and the current node ID is smaller than the node ID given
    {
        prev = p;
        p = p->getNext();  // Proceed to the next resistor
    }
    n->setNext(p);  // Link the new resistor with the next resistor
    if(prev == NULL)  // If the new resistor is the first resistor
        head = n;
    else
        prev ->setNext(n); // Link the previous resistor with the new resistor
    
    this->setNumRes(1);  // Update the resistor number in this list
    return n;   
}

Resistor* ResistorList::findResistor(string name)
{
    if(head == NULL)  // If the list is empty
        return head;
    
    Resistor* p = head;
    Resistor* prev = NULL;
    
    while((p != NULL) && !(p->compareName(name)))  // If p have not yet reached the end of the list, 
                                                     // and the current resistor name is not the given resistor name
    {
        prev = p;
        p = p->getNext();  // Proceed to the next resistor
    }
    return p;  // If found, p is the address to the resistor, otherwise, p is NULL;
    
}

void ResistorList::printResistor()
{
    Resistor* p = head;
    
    while(p != NULL)
    {
        p->print();
        p = p->getNext();        
    }
}

bool ResistorList::deleteResistor(string name)
{
    if(head == NULL)  // If the list is empty
        return head;
    
    Resistor* p = head;
    Resistor* prev = NULL;
    
    while((p != NULL) && !(p->compareName(name)))  // If p have not yet reached the end of the list, 
                                                     // and the current resistor name is not the given resistor name
    {
        prev = p;
        p = p->getNext();  // Proceed to the next resistor
    }
    
    if((p == NULL) || !(p ->compareName(name)))  // If p is at the end of the list, and resistor with given name is not found
        return false;
    
    if(prev == NULL)  // If head is being deleted
        head = p->getNext();
    else
        prev->setNext(p->getNext());  // Let the linked list "by-pass" this resistor
    
    delete p;
    this->setNumRes(-1);
    return true;
}

