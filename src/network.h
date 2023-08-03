#ifndef NETWORK_H // if not defined
#define NETWORK_H // define

#include <vector>
#include <memory>
#include "node.h"

// NOTE : Network class is meant to be used with creator. if u want to play around with this (when reviewing maybe) then just make a creator instance.

class Network{
    public:
        Network(); // regular constructor | create blank network

        //// Ro5
        ~Network(); // Destructor
        Network(const Network &source, bool mutate=false); // copy constructor | copy existing network + mutate
        Network& operator=(const Network &source); // copy assignment op
        Network(Network &&source); // move constructor
        Network& operator=(Network &&source); // move assignment op
    
        //// EOF Ro5

        void Initialize();
        void addInput(std::shared_ptr<float> input); // add an input variable
        void addOutput(int num=1); // add an output node


        //// TEMPLATE
        template <typename T, typename P> void connect(std::vector<T> &first, std::vector<P> &second); // connect all elements of first to second
        template <typename T, typename P> void connect(T &first, P &second); 
        ////


        void addConnections(std::vector<StartNode> &inputs, std::vector<std::vector<Node>> &nodes, std::vector<OutputNode> &outputs); // add connections
        void printStructure(); // print whole network structure
        std::vector<float> getOutputs(); // get the final outputs
    private:
        void addHiddenNode(int layer);


        void calculateNetwork(); // self explanatory
        const float _mutationChance = 0.1; // this value divided by score. if it is good, dont change it
        const float _defaultWeight = 1; // starting weight
        const float _defaultBias = 0; // starting bias
        const float _initConnectionChance = 0.3; // chance of creating connection at inititialization for each node
        const int _defaultHiddenLayers = 2; // number of hidden layers
        const int _defaultNodesPerLayer = 3; // number of nodes in each hidden layer
        int _outputs = 0; // output amount | 
        int id{0}; // id, just for debuggin
        std::vector<std::shared_ptr<float>> _inputs;
        std::vector<std::vector<Node>> _nodes;
        std::vector<StartNode> _startNodes;
        std::vector<OutputNode> _outputNodes;
};

#endif // end the if statement