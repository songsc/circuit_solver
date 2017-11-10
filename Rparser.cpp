#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
#include "easygl.h"
#include "Node.h"
#include "NodeList.h"
#include "Resistor.h"
#include "ResistorList.h"
#include "Rparser.h"

using namespace std;

# define MIN_ITERATION_CHANGE 0.0001

easygl window("Resistor display", WHITE);
NodeList nodeL;



void invalidCmd()  // Prints out error message for invalid command
{
    cout << "Error: invalid command" << endl;
    return;
}

void invalidArg()  // Prints out error message for invalid argument
{
    cout << "Error: invalid argument" << endl;
}

bool negativeR(double resistance)  // Check if the resistance is negative
{
    if(resistance < 0)
    {
        cout << "Error: negative resistance" << endl;
        return false;
    }
    return true;
}

// Out of range error is no longer required to be checked, therefore it always returns true
bool outOfRange(int nodeid, int lowerB)  // Check if the nodeid is out of the range
{
//    if(nodeid < lowerB)
//    {
//        cout << "Error: node " << nodeid << " is out of permitted range " << endl;
//        return false;
//    }
    return true;    
}

// Resistior name is "all" error is no longer required to be check, therefore this function always returns true
bool keyword(string name)  // Check if the name is the keyword "all"
{
//    if(name == "all")
//    {
//        cout << "Error: resistor name cannot be the keyword \"all\"" << endl;
//        return false;
//    }
    return true;
}

bool nodeValue(int nodeid1, int nodeid2)  // Check if the node ids are the same
{
    if(nodeid1 == nodeid2)
    {
        cout << "Error: both terminals of resistor connect to node" << nodeid1 << endl;
        return false;
    }
    return true;
}

void tooManyArgs()  // Prints out the error message for too many arguments
{
    cout << "Error: too many arguments" << endl;
}

void tooFewArgs()  // Prints out the error message for too mew arguments
{
    cout << "Error: too few arguments" << endl;
}

bool checkChange(NodeList &nodeL)  // Check if the change of node voltage exceeds the minimum iteration change
{
    Node* p = nodeL.getHead();
    
    while((p != NULL) && ((p->getChange() < MIN_ITERATION_CHANGE) || (p->getSet())))  // If p have not yet reached the end of the list, 
                                                                                      // and the change of voltage is smaller than the limit
        p = p->getNext();  // Proceed to the next node
    
    if(p == NULL)  // If p is at the end of the list
        return true;  
    return false;  
}

bool minimumKnowns(NodeList &nodeL)  // Check if this circuit is solvable or not, by checking the number of node with a set voltage
{
    Node* p = nodeL.getHead();
    
    while(p != NULL)  // If p have not yet reached the end of the list, 
    {
        if((p->getNumRes() != 0) && (p->getSet() == true))  // If a node has a set voltage, and this node is connected to the network
            return true;
        if((p->getNumRes() == 0) && (p->getSet() == true))  // If a node has a set voltage, but this node is not connected to the network
            return true;
        p = p->getNext();  // Proceed to the next node
    }
    return false;  // None of the node has a set voltage, this circuit is unsolvable        
}

void solveNode(NodeList &nodeL, Node* p)  // Solve the voltage at a single node
{
    double operand1 = 0, operand2 = 0, voltage, change;
    Resistor* q = p->getResList()->getHead();
    if(q == NULL)  // This node is not connected to the network, no calculation required
        return;
    while(q != NULL)  //  Calculate the two operands in the equation
    {
        int nodeid;
        if(q->getNodeID1() == p->getNodeID())  // If nodeid1 is the current node's id
            nodeid = q->getNodeID2();  // This resistor is connected from this this node to node with nodeid2
        else  // If nodeid2 is the current node's id
            nodeid = q->getNodeID1();  // This resistor is connected from this this node to node with nodeid1
        operand1 = operand1 + 1/(q->getResistance());
        operand2 = operand2 + (nodeL.findNode(nodeid)->getVoltage() / q->getResistance());
        q = q->getNext();
    }
    operand1 = 1 / operand1;
    voltage = operand1 * operand2;  // Calculate voltage
    change = voltage - p->getVoltage();  // Calculate change of voltage in this iteration
    if(change < 0)  //  Calculate the absolute value of change, to make it easier to compare
        change = -1 * change;
    p->setVoltage(voltage);
    p->setChange(change);
}


bool solve(NodeList &nodeL)
{
    Node* p = nodeL.getHead();
    while(p != NULL)  // while p has not yet reached the end of the list
    {
        if(!p->getSet())  // If this node does not have a set voltage      
            solveNode(nodeL, p);  // Solve for the voltage
        p = p->getNext();  // Move to the next node        
    }
    if(checkChange(nodeL))  // Check the change of voltage, if smaller than the maximum change
        return true;  // Success!
    solve(nodeL);  // If the change exceeds the limit, re-calculate
}

void drawNode()
{
    int numNode = nodeL.countNode();
    int numRes = nodeL.countRes();

    Node* p = nodeL.getHead();

    while((numNode != 0) && (p->getNumRes() == 0))
        p = p->getNext();
    for(int i = 0; i < numNode; i++)
    {
        string nodeid = static_cast<ostringstream*>( &(ostringstream() << p->getNodeID()) )->str();
        string voltage = static_cast<ostringstream*>( &(ostringstream() << p->getVoltage()) )->str();
        window.gl_setcolor (BLACK);
        window.gl_drawtext (170 + 250 * i, 75, "Node ",150.);
        window.gl_drawtext (190 + 250 * i, 75, nodeid, 150.);
        if(p->getSet())
            window.gl_setcolor (RED);
        window.gl_drawtext (170 + 250 * i, 50, voltage, 150.);
        window.gl_drawtext (190 + 250 * i, 50, " V", 150.);
        window.gl_setcolor (LIGHTGREY);
        window.gl_fillrect (150 + 250 * i , 100, 200 + 250 * i, 150 + 50 * numRes);
        p->setX(175 + 250 * i);
        p = p->getNext();
        while((i != (numNode - 1)) && (p->getNumRes() == 0))
                p = p->getNext();
    }
}

void drawResistor(Resistor* r, int x, int y)
{
    string resistance = static_cast<ostringstream*>( &(ostringstream() << r->getResistance()) )->str();
    window.gl_setcolor (BLACK);
    window.gl_drawtext (x + 10, y - 20, r->getName(),150.);
    window.gl_drawtext (x + 30, y - 20, " (", 150.);
    window.gl_drawtext (x + 40, y - 20, resistance, 150.);
    window.gl_drawtext (x + 60, y - 20, " Ohm)", 150.);
    window.gl_setcolor (RED);
    window.gl_drawline(x, y, x + 10, y);
    window.gl_drawline(x + 10, y, x + 15, y - 7.5);
    window.gl_drawline(x + 15, y - 7.5, x + 25, y + 7.5);
    window.gl_drawline(x + 25, y + 7.5, x + 35, y - 7.5);
    window.gl_drawline(x + 35, y - 7.5, x + 45, y + 7.5);
    window.gl_drawline(x + 45, y + 7.5, x + 55, y - 7.5);
    window.gl_drawline(x + 55, y - 7.5, x + 65, y + 7.5);
    window.gl_drawline(x + 65, y + 7.5, x + 70, y);
    window.gl_drawline(x + 70, y, x + 80, y);
    
}

void drawResistors()
{
    int numRes = nodeL.countRes();
    int numNode = nodeL.countNode();
    int y = 175;
    Node* p = nodeL.getHead();
    
    while((numNode != 0) && (p->getNumRes() == 0))
        p = p->getNext();
    for(int i = 0; i < numNode; i++)
    {
        Resistor* q = p->getResList()->getHead();
        for(int j = 0; j < numRes; j++)
        {
            if(q->getNodeIDL() == p->getNodeID())
            {                
                window.gl_setcolor (BLACK);
                window.gl_fillarc (p->getX(), y, 10, 0, 360);
                window.gl_fillarc (nodeL.findNode(q->getNodeIDH())->getX(), y, 10, 0, 360);
                window.gl_drawline(p->getX(), y, p->getX() + ((nodeL.findNode(q->getNodeIDH())->getX() - p->getX() - 80) / 2), y);
                drawResistor(q, p->getX() + ((nodeL.findNode(q->getNodeIDH())->getX() - p->getX() - 80) / 2), y);
                window.gl_setcolor (BLACK);
                window.gl_drawline(p->getX() + ((nodeL.findNode(q->getNodeIDH())->getX() - p->getX() - 80) / 2) + 80, y, nodeL.findNode(q->getNodeIDH())->getX(), y);
                y = y + 50;
            }
            if(q->getNext() != NULL)
                q = q->getNext();
            else
                j = numRes;
        }
        p = p->getNext();
        while((i != (numNode - 1)) && (p->getNumRes() == 0))
                p = p->getNext();
    }
}

void easygl::drawscreen()
{
   window.gl_clearscreen();  /* Should precede drawing for all drawscreens */
   drawNode();
   drawResistors();
}


int parser()
{
    string line, cmd;
    
    
    
    cout << "> ";
    cout.flush();
    
    getline(cin, line);  // Get a line from standard input
    if(cin.fail())
    {
        if(cin.eof())  // If cin fails due to eof
        {
            cout << "Error: EOF" << endl;
            return 0;
        }
        else  // If cin fails due to some unknown errors
        {
            cout << "Error: Other" << endl;
            return 0;
        }
    }
            
    
    while(!cin.eof())
    {
        stringstream ls(line);  // Pass the string line to the string stream buffer
        string name, extra;
        double resistance, voltage;
        int nodeid1, nodeid2;
        
        ls >> cmd;
        
        if(cmd == "insertR")  // InsertR command
        {
            ls >> name;
            if(ls.fail())  // Name is not obtained from the buffer
            {
                tooFewArgs();
                ls.ignore (1000, '\n');
            }
            else  // Name has been read from the buffer
            {
                ls >> resistance;
                if(ls.fail())  // Resistance is not obtained, handles errors
                {
                    
                    if(ls.eof())  
                        tooFewArgs();
                    else
                        invalidArg();
                    ls.ignore (1000, '\n');
                }
                else if(negativeR(resistance))  // If resistance is positive
                {
                    ls >> nodeid1;  // Get nodeid1
                    if(ls.fail())  // Nodeid1 not obtained, handles errors
                    {
                        if(ls.eof())
                            tooFewArgs();
                         else
                            invalidArg();
                         ls.ignore (1000, '\n');
                    }
                    else if(outOfRange(nodeid1, 0))  // If nodeid1 is within the permitted range
                    {
                        ls >> nodeid2;  // Get nodeid2
                        if(ls.fail())   // Nodeid2 not obtained, handles errors
                        {
                            if(ls.eof())  // Nodeid2 not obtained, handles errors
                                tooFewArgs();
                             else
                                invalidArg();
                            ls.ignore (1000, '\n');
                        }
                        else if(outOfRange(nodeid2, 0))  // If nodeid2 is within the permitted range
                        {
                            if(keyword(name))  // And name is not"all"
                            {
                                if(nodeValue(nodeid1, nodeid2))  // And nodeid1 is not equal to nodeid2
                                {
                                    ls >> extra;
                                    if(ls.fail())  // And there's nothing left in the buffer
                                    {
                                        ls.clear();
                                        ls.ignore(1000, '\n');
                                        
                                        
                                        Node* a = nodeL.findOrInsertNode(nodeid1);
                                        Node* b = nodeL.findOrInsertNode(nodeid2);
                                        
                                        a->addResistor(name, resistance, nodeid1, nodeid2);
                                        b->addResistor(name, resistance, nodeid1, nodeid2);
                                        
                                            cout << "Inserted: resistor " << name << " "
                                            << std::fixed << std::setprecision(2) 
                                            << resistance << " Ohms " << nodeid1 
                                            << " -> " << nodeid2 << endl;  // Prints out this message
                                    }
                                    else  // There is still something in the buffer
                                        tooManyArgs();
                                }
                            }
                        }
                    }                
                }
            }
        }
        
        else if(cmd == "modifyR")  // ModifyR command
        {
            ls >> name;  // Read the variable name from the buffer
            if(ls.fail())  // Failed, handles errors
            {
                tooFewArgs();
                ls.ignore (1000, '\n');
            }
            else  // Name is obtained form the buffer
            {
                ls >> resistance;  // Read resistance
                if(ls.fail())  // Resistance is not obtained, handles errors
                {
                    if(ls.eof())
                        tooFewArgs();
                    else
                        invalidArg();
                    ls.ignore (1000, '\n');
                }
                else if(negativeR(resistance))  // If resistance is positive
                {
                    if(keyword(name))  // And name is not "all"
                    {
                        ls >> extra;
                        if(ls.fail())  // And there's nothing else in the buffer
                        {
                            ls.clear();
                            ls.ignore(1000, '\n');
                            
                            Resistor* p = nodeL.findResistor(name);
                            if(p == NULL)
                                cout << "Error: resistor " << name << " not found" << endl;
                            else
                            {
                                cout << "Modified: resistor " << name << " from " << std::fixed <<
                                        std::setprecision(2) << p->getResistance() <<
                                        " Ohms to " << std::fixed << std::setprecision(2)  << 
                                        resistance << "Ohms" << endl;
                                nodeL.modifyR(name, resistance);
                            }                                                             
                        }
                        else  // Something is still in the buffer, too many arguments 
                            tooManyArgs();
                    }
                }
            }            
        }
        
        else if(cmd == "printR")  // printR command
        {
            ls >> name;  // Read the name from the buffer
            if(ls.fail())  // Name is not obtained, handles error
            {
                tooFewArgs();
                ls.ignore (1000, '\n');
            }
            else  // If name is some other strings
            {
                ls >> extra;
                if(ls.fail())  // Nothing left in the buffer
                {
                    ls.clear();
                    ls.ignore(1000, '\n');
                    
                    Resistor* p = nodeL.findResistor(name);
                    if(p == NULL)
                        cout << "Error: resistor " << name << " not found" << endl;
                    else
                    {
                        cout << "Print:" << endl;
                        p->print();
                    } 
                }
                else  // Too many arguments
                    tooManyArgs();
            }
        }
        
        else if(cmd == "printNode")  // printNode command
        {
            while(isspace(ls.peek()))  // Ignore all the white space first
                ls.ignore(1, '\n');
            if(isdigit(ls.peek()))  // Check to see if the next digit is a number, if yes, proceed
            {
                ls >> nodeid1;  // Get the number from the buffer
                if(outOfRange(nodeid1, 0))  // Proceed if the nodeid is within the permitted range
                {
                    ls >> extra;
                    if(ls.fail())  // If there's nothing left in the buffer
                    {
                        ls.clear();
                        ls.ignore(1000, '\n');
                        cout << "Print:" << endl;
                        nodeL.printNode(nodeid1);
                    }
                    else  // Something is still in the buffer, error!
                        tooManyArgs();
                }
            }
            else  // The next digit is not a number
            {                   
                ls >> name;  // Reads it as a string
                if(name == "all")  // If it's the keyword "all"
                {
                    if(ls.eof())  // And there's nothing left
                    {
                        cout << "Print:" << endl;
                        nodeL.printNode();                           
                    }                        
                    else  // Otherwise prints too many arguments
                        tooManyArgs();
                }
                else  // If it's not all, it's an invalid argument
                {
                    invalidArg();
                }
            }
        }

        else if(cmd == "deleteR")  // deleteR command
        {
            ls >> name;
            if(name == "all")  // If name is the keyword "all"
            {
                if(ls.eof())  // And there's nothing in the buffer
                {
                    //nodeL.~NodeList();
                    nodeL.deleteAll();
                    cout << "Deleted: all resistors " << endl;  // Prints this out
                }                    
                else  // Otherwise too many arguments
                    tooManyArgs();
            }
            else  // If name is a sting that is not "all"
            {
                if(ls.eof())  // Prints out the following msg if there's nothing left in the buffer
                {
                    if(nodeL.deleteR(name))
                        cout << "Deleted: resistor " << name << endl;
                    else
                        cout << "Error: resistor " << name << " not found" << endl;
                }
                else  // Or there're too many arguments
                    tooManyArgs();
            }
        }
        
        else if(cmd == "setV")
        {
            ls >> nodeid1;  // Read the variable name from the buffer
            if(ls.fail())  // Failed, handles errors
            {
                tooFewArgs();
                ls.ignore (1000, '\n');
            }
            else  // Name is obtained form the buffer
            {
                ls >> voltage;  // Read resistance
                if(ls.fail())  // Resistance is not obtained, handles errors
                {
                    if(ls.eof())
                        tooFewArgs();
                    else
                        invalidArg();
                    ls.ignore (1000, '\n');
                }
                else if(outOfRange(nodeid1, 0))  // If resistance is positive
                {
                    ls >> extra;
                    if(ls.fail())  // And there's nothing else in the buffer
                    {
                        ls.clear();
                        ls.ignore(1000, '\n');
                        
                        Node* p = nodeL.findOrInsertNode(nodeid1);
                        p->setVoltage(voltage);
                        p->setSet(true);
                        
                        cout << "Set: node " << nodeid1 << " to  " << std::fixed << 
                                std::setprecision(2) << p->getVoltage() << " Volts " << endl;                                                      
                    }
                    else  // Something is still in the buffer, too many arguments 
                        tooManyArgs();                    
                }
            }            
        }
        
        else if(cmd == "unsetV")
        {
            ls >> nodeid1;  // Read the variable name from the buffer
            if(ls.fail())  // Failed, handles errors
            {
                tooFewArgs();
                ls.ignore (1000, '\n');
            }
            if(outOfRange(nodeid1, 0))  // If resistance is positive
            {
                ls >> extra;
                if(ls.fail())  // And there's nothing else in the buffer
                {
                    ls.clear();
                    ls.ignore(1000, '\n');
                    
                    Node* p = nodeL.findOrInsertNode(nodeid1);
                    p->setVoltage(0);
                    p->setSet(false);
                      
                    cout << "Unset: the solver will determine the voltage of node " << nodeid1 << endl;            
                }
                else  // Something is still in the buffer, too many arguments 
                    tooManyArgs();            
            }            
        }
        
        else if(cmd == "solve")
        {
            if(!minimumKnowns(nodeL))
                cout << "Error: no nodes have their voltage set" << endl;
            else if(solve(nodeL))
            {
                cout << "Solve:" << endl;
                Node* p = nodeL.getHead();
                while(p != NULL) // If p have not yet reached the end of the list, 
                {
                    if(p->getNumRes() != 0)
                        cout << "  Node " << p->getNodeID() << ": " << p->getVoltage() << " V" << endl;
                    p = p->getNext();  // Proceed to the next node
                }
            }                    
        }
        
        else if(cmd == "draw")
        {
            Node* p = nodeL.getHead();
            float xleft, ybottom, xright, ytop;
            xleft = 0., ybottom = 0.;

            int numNode = nodeL.countNode();
            int numRes = nodeL.countRes();

            xright = xleft + 150 + 250 * numNode + 150;

            ytop = ybottom + 50 * numRes + 150;

            window.set_world_coordinates (xleft, ybottom, xright, ytop);
            cout << "Draw: control passed to graphics window" << endl;
            window.gl_event_loop ();

            
            cout << "Draw: complete; responding to commands again" << endl;
        }
                
        else  // If cmd does not match any of the above, it's an invalid command
            invalidCmd();
        
   
        cout << "> ";
        getline(cin, line);  // Get a new line from cin
    }
    return 0;
}




