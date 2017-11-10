/* 
 * File:   NodeList.h
 * Author: songshu3
 *
 * Created on October 29, 2013, 2:31 PM
 */

#ifndef NODELIST_H
#define	NODELIST_H

#include <string>
#include "Node.h"
#include "Resistor.h"
using namespace std;

class NodeList
{
private:
    Node *head;
    
public:
    NodeList();
    ~NodeList();
    Node* getHead();    
    Node* findNode(int nodeid);
    Node* findOrInsertNode(int nodeid);
    Resistor* findResistor(string name);
    void printNode();
    void printNode(int nodeid);
    bool modifyR(string name, double resistance);
    bool deleteR(string name);
    void deleteAll();
    int countNode();
    int countRes();
};

#endif	/* NODELIST_H */

