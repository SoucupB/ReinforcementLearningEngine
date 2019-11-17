#pragma once
#include "MathMatrix.h"
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <random>
#include <cstring>
class NeuralNetwork{
private:
    double fabs(double a);
    bool activSwitch = 0, biasSwitch = 0, meanSq = 0, reluSwitch = 0;
    double lr = 0.001;
public:
    std::vector<Matrix> weights, biases;
    static double sigmoidFunc(double x);
    static double dsigmoidFunc(double x);
    static double reluFunc(double x);
    static double dreluFunc(double x);
    static double relu(double x);
    //NeuralNetwork(std::vector<int> conf, const char *activationFunction, double deviation, const char *biasSwitch, const char *lossFunc, double lr);
    void assign_params(std::vector<int> conf, const char *activationFunction, double deviation, const char *biasSwitch, const char *lossFunc, double lr);
    void showWeights();
    std::vector<double> feed(std::vector<double> input);
    void saveModel(const char *filename);
    void loadModel(const char *filename);
    double sgd(std::vector<double> input, std::vector<double> output);
    void decrease_lr(int percent);
    void assign_lr(double lr);
    double get_lr();
};