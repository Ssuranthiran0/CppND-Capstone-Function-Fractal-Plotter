#include "network.h" // Header
#include "node.h" // Node, StartNode, OutputNode
#include <memory> // smart ptr
#include <iostream> // std::cout
#include <random> // random generation
#include <vector> // std::vector 



void Network::Initialize(){
    for(int i=0; i<_inputs.size(); i++){
        _startNodes.emplace_back(std::move(_inputs[i]), i); // emplace back calls the constructor of vector's type
    }
    for(int i=0; i<_defaultHiddenLayers; i++){
        std::vector<Node> layer;
        for(int j=0; j<_defaultNodesPerLayer; j++){
            layer.emplace_back(_defaultWeight, _defaultBias, i, j);
        }
        _nodes.emplace_back(layer);
    }
    for(int i=0; i<_outputs; i++){
        _outputNodes.emplace_back(i);
    }
}

Network::Network(){} // Blank


//// Ro5
Network::~Network(){}/* Smart Ptr So No Deallocation Needed */

Network::Network(const Network &source, bool mutate) : 
            _nodes(source._nodes), 
            _startNodes(source._startNodes), 
            _outputNodes(source._outputNodes)
{
    // must make deep copy because it is ptr (shared but still)
    _inputs.clear(); // MUST create new inputs MANUALLY. 
}

Network& Network::operator=(const Network &source){ // copy assignment op
    if(this == &source){
        return *this; // dont copy from this into this
    }
    // rest (besides return) is same as copy ctor
    _inputs.clear(); // MUST create new inputs MANUALLY. 
    _nodes = source._nodes;
    _startNodes = source._startNodes;
    _outputNodes = source._outputNodes;
    // return ref to this
    return *this;
} 

Network::Network(Network &&source) : 
            _nodes(std::move(source._nodes)),
            _startNodes(std::move(source._startNodes)),
            _inputs(std::move(source._inputs)),
            _outputNodes(std::move(source._outputNodes))
{

    // clear all source vectors || no need to invalidate all input ptrs since it is shared ptr
    source._inputs.clear();
    source._nodes.clear();
    source._startNodes.clear();
    source._outputNodes.clear();
}

Network& Network::operator=(Network &&source){ // move assignment op
    if(this == &source){
        return *this; // dont move from self into self
    }
    // move ctor
    _inputs = std::move(source._inputs); // actually im treating _inputs as a unique ptr vector so i could change it, but then i have to change it everywhere so idk
    _nodes = std::move(source._nodes);
    _startNodes = std::move(source._startNodes);
    _outputNodes = std::move(source._outputNodes);

    // clear all source vectors || no need to invalidate all input ptrs since it is shared ptr
    source._inputs.clear();
    source._nodes.clear();
    source._startNodes.clear();
    source._outputNodes.clear();

    return *this;
}
//// EOF Ro5


void Network::addHiddenNode(int layerIndex){
    std::vector<Node> &currentLayer = _nodes[layerIndex];
    currentLayer.emplace_back();
}

template <typename T, typename P> 
void connect(std::vector<T> &first, std::vector<P> &second){
    
}
template <typename T, typename P> 
void connect(T &first, P &second){
    first.connect(std::make_shared<P>(second));
}

void Network::addInput(std::shared_ptr<float> input){
    _inputs.emplace_back(std::move(input));
}
void Network::addOutput(int num){
    _outputs += num; // init after obviously (not here, but where this is called)
}
void Network::printStructure(){
    int nodeCount{0}; // for some reason it auto initializes to like 20k. really odd.
    for (auto layer : _nodes){
        nodeCount += layer.size();
   }
   std::cout << "Start: " << _startNodes.size() << " | Hidden Nodes: " << nodeCount << " | Outputs: " << _outputNodes.size() << std::endl;
}
void Network::addConnections(std::vector<StartNode> &inputs, std::vector<std::vector<Node>> &nodes, std::vector<OutputNode> &outputs){}
void Network::calculateNetwork(){

    // Clear any previous Values
    for (std::vector<Node>& layer : _nodes) {
        for (Node& node : layer) {
            node.clearValues();
        }
    }

    for (OutputNode &node : _outputNodes) {
        node.clearValues();
    } 

    for (StartNode& i : _startNodes) { // loop thru all start nodes
        i.giveValues(); // make current start node give value to all connected
    }
    // Calculate values for hidden layers
    for (std::vector<Node>& layer : _nodes){
        for (Node& node : layer){ // for each node in the layer
            node.giveValues(); // make it give values to all connections
        }
    }
}
std::vector<float> Network::getOutputs(){
    calculateNetwork();
    std::vector<float> outputs;
    for (OutputNode &oNode : _outputNodes) {
        outputs.push_back(oNode.getVal());
    }
    return outputs;
}   