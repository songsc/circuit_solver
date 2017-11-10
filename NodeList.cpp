#include <string>

#include "NodeList.h"


using namespace std;

NodeList::NodeList()
{
    head = NULL;
}

NodeList:: ~NodeList()
{
    Node* p;
    while(head != NULL)  // While the list is not empty
    {
        p = head; 
        head = p->getNext();  // Move forward
        delete p;  // Delete the resistor list at this node
    }
}

Node* NodeList::getHead()
{
    return head;
}

Node* NodeList::findNode(int nodeid)
{
    Node* p = head;
    
    while((p != NULL) && (p->getNodeID() < nodeid))  // If p have not yet reached the end of the list, 
                                                     // and the current node ID is smaller than the node ID given
        p = p->getNext();  // Commence
    
    
    if((p != NULL) && (p->getNodeID() == nodeid))  // If the current node's nodeID is the node ID given
        return p;  // Node found, return the pointer;
    
    return NULL;  // The given node ID does not exist in this list
}

Node* NodeList::findOrInsertNode(int nodeid)
{
    if(head == NULL)  // If the list is empty
    {
        head = new Node(nodeid);  // Let head point to the new node;
        return head;
    }
    
    Node* p = head;
    Node* prev = NULL;
    
    while((p != NULL) && (p->getNodeID() < nodeid))  // If p have not yet reached the end of the list, 
                                                     // and the current node ID is smaller than the node ID given
    {
        prev = p;
        p = p->getNext();  // Proceed to the next node
    }
    
    if((p != NULL) && (p->getNodeID() == nodeid))  // If the current node's nodeID is the node ID given
        return p;  // Node found, return the pointer;

    // Node with node ID given is not in the list
    Node* n = new Node(nodeid);  // Create new node with the given node ID
    n->setNext(p);  // Link the new node with the next node
    if(prev == NULL)  // If the list is initially empty
        head = n;  // Let the new node to be the head
    else
        prev->setNext(n);  // Link the previous node with the new node
    return n;  // Return the pointer
}

Resistor* NodeList::findResistor(string name)
{
    Node* p = head;
    
    while((p != NULL) && (p->findResistor(name) == NULL))  // If p have not yet reached the end of the list, 
                                                           // and the resistor with given name is not connected to this node
        p = p->getNext();  // Proceed to the next node
    
    if(p == NULL)  // If p is at the end of the list
        return NULL;  // Return NULL
    return (p->findResistor(name));  // Return the resistor    
}

void NodeList::printNode()
{
    Node* p = head;
    while(p != NULL)
    {
        p->printNode();
        p = p->getNext();
    }        
}

void NodeList::printNode(int nodeid)
{
    if(head != NULL)
    {
        Node* p;
        p = this->findNode(nodeid);
        if(p != NULL)
            p->printNode();
    }    
}

bool NodeList::modifyR(string name, double resistance)
{
    Node* p = head;
    
    // modify the first copy of the resistor;
    while((p != NULL) && (p->findResistor(name) == NULL))  // If p have not yet reached the end of the list, 
                                                           // and the resistor with given name is not connected to this node
        p = p->getNext();  // Proceed to the next node
    
    if(p == NULL)  // If p is at the end of the list
        return false;  // Return NULL
    p->findResistor(name)->setResistance(resistance);
    
    // Modify the second copy of the resistor;
    p = p->getNext();
    while((p != NULL) && (p->findResistor(name) == NULL))  // If p have not yet reached the end of the list, 
                                                           // and the resistor with given name is not connected to this node
        p = p->getNext();  // Proceed to the next node
    
    if(p == NULL)  // If p is at the end of the list
        return false;  // Return NULL
    p->findResistor(name)->setResistance(resistance);
    return true;
}

bool NodeList::deleteR(string name)
{
    Node* p = head;
    
    // Delete the first copy of the resistor;
    while((p != NULL) && (p->findResistor(name) == NULL))  // If p have not yet reached the end of the list, 
                                                           // and the resistor with given name is not connected to this node
        p = p->getNext();  // Proceed to the next node
    
    if(p == NULL)  // If p is at the end of the list
        return false;  // Return NULL
    p->deleteResistor(name);
    
    // Delete the second copy of the resistor;
    p = p->getNext();
    while((p != NULL) && (p->findResistor(name) == NULL))  // If p have not yet reached the end of the list, 
                                                           // and the resistor with given name is not connected to this node
        p = p->getNext();  // Proceed to the next node
    
    if(p == NULL)  // If p is at the end of the list
        return false;  // Return NULL
    return p->deleteResistor(name);
}

void NodeList::deleteAll()
{
    Node* p = head;
    while(p != NULL)  // While the list is not empty
    {
        p->getResList()->~ResistorList();  
        p = p->getNext();  // Move forward
    }    
}

int NodeList::countNode()
{
    int count = 0;
    Node* p = head;
    while(p != NULL)  // While the list is not empty
    {
        if(p->getNumRes() != 0)
            count++;
        p = p->getNext();  // Move forward
    }
    return count;
}

int NodeList::countRes()
{
    int count = 0;
    Node* p = head;
    while(p != NULL)  // While the list is not empty
    {
        count = count + p->getNumRes();
        p = p->getNext();  // Move forward
    }
    return count / 2;
}
