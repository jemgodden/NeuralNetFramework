#pragma once


class Network {
    private:
        int _inputNodes;
        int _hiddenLayers;
        int _hiddenNodes;
        int _outputNodes;

    public:
        Network(int inputNodes, int hiddenLayers, int hiddenNodes, int outputNodes);
        
        ~Network();
        
        int inputNodes();
        
        int hiddenLayers();

        int hiddenNodes();

        int outputNodes();

        void info();

        void save();

        void load();

        void train();

        void predict();
};
