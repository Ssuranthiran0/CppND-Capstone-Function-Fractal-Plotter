#ifndef NODE_H
#define NODE_H

#include <memory> // smart ptr
#include <vector> // std::vector

class Node {
    public:
        Node(float weight=1, float bias=0, int layer=0, int pos=0);
        void addValue(float val); // add a value to the "queue" of values to be calculated
        void giveValues(); // give values to all connections
        void clearValues(); // clear

        /// VIRTUAL
        virtual float getVal(); // calculate and return value
        ///

        std::vector<std::shared_ptr<Node>> getConnections(); // getter for _connections
        void addConnection(std::shared_ptr<Node> node); // connect to ptr of node
        std::vector<float> _values; // values to calculate
        int currentLayer; // testing layer
        int positionInLayer; // testing position
    protected:
        void calculateValues(); // calculate everything. getval uses this so this can be private/protected
        std::vector<std::shared_ptr<Node>> _connections; // shared_ptr to allow copying 

        /// VIRTUAL
        virtual void printLocation();
        ///

        // take linear value (after calculation from sum(inputs) * weight, and introduce some nonlinearity)
        float _weight{1.0}; // multiply input by weight
        float _value;
        float activator(float val, int aType=_defaultActivator); 
        float _bias; // plain addition
        static const int _defaultActivator{0}; // change this to 
};
class StartNode : public Node{ 
    public:
        StartNode(std::shared_ptr<float> val, int pos); // construct with shared ptr and position
        void printLocation() override; // override printing for debugging
        std::shared_ptr<float> value; // ptr of value
        void setValue(float &val); // set value
        float getVal() override; // override getVal
};
class OutputNode : public Node {
    public:
        void printLocation() override; // override debug print
        OutputNode(int pos=0); // constructor
};

#endif // NODE_H