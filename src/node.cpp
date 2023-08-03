#include "node.h" // Header
#include <cmath> // For activator functions
#include <iostream> // std::cout



// Node Class Implementation
Node::Node(float weight, float bias, int layer, int pos): 
            _weight(weight), 
            _bias(bias), 
            currentLayer(layer), 
            positionInLayer(pos)
{}

float Node::getVal(){
    calculateValues(); // calculate all values
    //std::cout << "Getting value: " << _value; printLocation(); // debugging
    return _value;
}

void Node::printLocation(){
    //std::cout << " | Layer: " << currentLayer << " Pos: " << positionInLayer << std::endl; // print location of this node in the model
}

void Node::clearValues(){
    //std::cout << "Clearing "; printLocation(); // debugging
    _values.clear(); // clear vector
}

void Node::addValue(float val) {
    //std::cout << "Adding value: " << val; printLocation(); //debugging
    _values.emplace_back(val); // add val to the vector
}

void Node::calculateValues() {
    _value = 0; // reset
    for(float i : _values){ // add all values to the value
        _value += i;
    }
    _value *= _weight; // multiply by the node weight
    _value += _bias; // add bias
    _value = activator(_value); // use the activator to add nonlinearity. this avoid the problem which occurs when linear equations add up to form another linear equation
    // if each linear equation ((input) * weight + bias) is made nonlinear, it can emulate any function, when added to enough other linear equations
    giveValues();
}

void Node::giveValues(){
    for(std::shared_ptr<Node> n : _connections){ // loop thru all
        n->addValue(getVal()); // add this node's value to the connected node
    }
}

std::vector<std::shared_ptr<Node>> Node::getConnections() {
    // Return the connections of the Node
    return _connections;
}

void Node::addConnection(std::shared_ptr<Node> node) {
    _connections.push_back(node); // add connection
}

float Node::activator(float val, int aType) { // activator function. should work with any of these. just choose a different value for aType when testing
    switch (aType){
        case 0:
            // Tanh
            val = std::tanh(val); // hyperbolic tan, otehr form is (e^x-e^-x)/(e^x+e^-x) -1<x<1
            break;
        case 1:
            // ReLU
            val = std::max<float> (0, val); // if x<0, x->0, otherwise leave at same
            break;
        case 2:
            // Leaky ReLU
            val = std::max<float> (val * 0.01, val); // like relu but x->0.01x if x<0
            break;
        default:
            // Sigmoid
            val = 1 / (1 + std::exp(-val));  // logistic function
            break;
    }
    return val;
}


// StartNode Class Implementation
void StartNode::setValue(float &val){
    value = std::make_shared<float>(val); // shallow copy
}

StartNode::StartNode(std::shared_ptr<float> val, int pos){
    value = std::make_shared<float>(*val); // deep copy
}

void StartNode::printLocation(){
    std::cout << " | INPUT NODE " << positionInLayer << std::endl; // show that its an inputnode (startnode)
}

float StartNode::getVal(){
    return *value; // override node getVal | get the value of the stored ptr
}


// OutputNode Class Implementation
OutputNode::OutputNode(int pos){
    positionInLayer = pos; // set
}

void OutputNode::printLocation(){
    std::cout << " | OUTPUT NODE " << positionInLayer << std::endl; // show that its an outputnode
}