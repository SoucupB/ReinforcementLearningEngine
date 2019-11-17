#include "NeuralNetwork.h"
double NeuralNetwork::sigmoidFunc(double x)
{
    double e = 2.7182818284590;
    return 1.0 / (1.0 + pow(e, -x));
}
double NeuralNetwork::dsigmoidFunc(double x)
{
    return x * (1.0 - x);
}
double NeuralNetwork::reluFunc(double x)
{
    return (x >= 0 ? x : 0.0001);
}
double NeuralNetwork::dreluFunc(double x)
{
    return (x >= 0 ? 1 : 0.0001);
}
double NeuralNetwork::relu(double x)
{
    if(x <= 0)
        return 0.0001;
    return x;
}
double NeuralNetwork::fabs(double a)
{
    return a < 0 ? -a : a;
}
void NeuralNetwork::assign_params(std::vector<int> conf, const char *activationFunction, double deviation, const char *biasSwitch, const char *lossFunc, double lr)
{
    this->weights.clear();
    this->biases.clear();
    for(int i = 0; i < conf.size() - 1; i++)
    {
        const char *random = "random";
        this->weights.push_back(Matrix(conf[i + 1], conf[i], random, deviation));
        this->biases.push_back(Matrix(conf[i + 1], 1, random, deviation));
    }
    if(!strcmp(activationFunction, "sigmoid"))
    {
        this->activSwitch = true;
    }
    if(!strcmp(activationFunction, "relu"))
    {
        this->reluSwitch = true;
    }
    if(!strcmp(biasSwitch, "usebias"))
    {
        this->biasSwitch = true;
    }
    if(!strcmp(lossFunc, "meanSquaredError"))
    {
        this->meanSq = 1;
    }
    this->lr = lr;
}
void NeuralNetwork::showWeights()
{
    for(int i = 0; i < weights.size(); i++)
    {
        weights[i].show();
        printf("\n");
    }
}

std::vector<double> NeuralNetwork::feed(std::vector<double> input)
{
    Matrix inputTensor = Matrix(input.size(), 1, "zero", 0);
    std::vector<Matrix> hidden;
    inputTensor.atribMatrix(input);
    hidden.push_back(inputTensor);
    for(int i = 1; i <= weights.size(); i++)
    {
            hidden.push_back(Matrix::matMul(this->weights[i - 1], hidden[i - 1]));
            if(biasSwitch)
                hidden[i] = hidden[i] + this->biases[i - 1];
            if(activSwitch)
                hidden[i].func(sigmoidFunc);
            if(reluSwitch)
                hidden[i].func(reluFunc);
    }
    return hidden[hidden.size() - 1].matr;
}

void NeuralNetwork::saveModel(const char *filename)
{
    std::ofstream fout;
    fout.open(filename, std::ios::out | std::ios::binary);
    for(int i = 0; i < this->weights.size(); i++)
    {
        int height = this->weights[i].height, width = this->weights[i].width;
        fout.write ((char*)&height, sizeof (height));
        fout.write ((char*)&width, sizeof (width));
        for(int j = 0; j < this->weights[i].matr.size(); j++)
        {
            double value = this->weights[i].matr[j];
            fout.write ((char*)&value, sizeof (value));
        }
    }
    for(int i = 0; i < this->biases.size(); i++)
    {
        int height = this->biases[i].height, width = this->biases[i].width;
        fout.write ((char*)&height, sizeof (height));
        fout.write ((char*)&width, sizeof (width));
        for(int j = 0; j < this->biases[i].matr.size(); j++)
        {
            double value = this->biases[i].matr[j];
            fout.write ((char*)&value, sizeof (value));
        }
    }
    fout.close();
}

void NeuralNetwork::assign_lr(double lr) {
    this->lr = lr;
}

double NeuralNetwork::get_lr() {
    return this->lr;
}

void NeuralNetwork::loadModel(const char *filename)
{
    std::ifstream fin;
    fin.open(filename, std::ios::in | std::ios::binary);
    if(!fin.is_open()) {
        printf("File with name %s could not be opened, maybe check the spelling or the path", filename);
        return ;
    }
    for(int i = 0; i < this->weights.size(); i++)
    {
        int height, width;
        fin.read ((char*)&height, sizeof (height));
        fin.read ((char*)&width, sizeof (width));
        this->weights[i].height = height;
        this->weights[i].width = width;
        for(int j = 0; j < this->weights[i].matr.size(); j++)
        {
            double value;
            fin.read ((char*)&value, sizeof (value));
            this->weights[i].matr[j] = value;
        }
    }
    for(int i = 0; i < this->biases.size(); i++)
    {
        int height, width;
        fin.read ((char*)&height, sizeof (height));
        fin.read ((char*)&width, sizeof (width));
        this->biases[i].height = height;
        this->biases[i].width = width;
        for(int j = 0; j < this->biases[i].matr.size(); j++)
        {
            double value;
            fin.read ((char*)&value, sizeof (value));
            this->biases[i].matr[j] = value;
        }
    }
    fin.close();
}

void NeuralNetwork::decrease_lr(int percent) {
    if(this->lr >= 0.1)
        this->lr = this->lr - (this->lr * ((double)percent / 100.0));
}

double NeuralNetwork::sgd(std::vector<double> input, std::vector<double> output)
{
    const char *zeros = "zero";
    Matrix inputTensor = Matrix(input.size(), 1, zeros, 0);
    std::vector<Matrix> hidden;
    inputTensor.atribMatrix(input);
    hidden.push_back(inputTensor);
    for(int i = 1; i <= weights.size(); i++)
    {
        hidden.push_back(Matrix::matMul(this->weights[i - 1], hidden[i - 1]));
        if(biasSwitch)
            hidden[i] = hidden[i] + this->biases[i - 1];
        if(activSwitch)
            hidden[i].func(sigmoidFunc);
        if(reluSwitch)
            hidden[i].func(reluFunc);
    }
    std::vector<Matrix> errorHidden;
    Matrix convOut(output.size(), 1, "zero", 0), outputError(output.size(), 1, "zero", 0);
    convOut.atribMatrix(output);
    if(meanSq)
    {
        outputError = Matrix::sgnhdProduct(convOut - hidden[hidden.size() - 1], convOut - hidden[hidden.size() - 1]);
    }
    else
    {
        outputError = convOut - hidden[hidden.size() - 1];
    }
    double cost = 0.0;
    for(int i = 0; i < outputError.matr.size(); i++)
        cost += fabs(outputError.matr[i]);
    errorHidden.push_back(outputError);
    int index = 0;
    for(int i = hidden.size() - 2; i >= 0; i--)
    {
        Matrix wTrans = this->weights[i];
        wTrans.transpose();
        errorHidden.push_back(Matrix::matMul(wTrans, errorHidden[index]));
        ++index;
    }
    index = 0;
    for(int i = hidden.size() - 1; i >= 1; i--)
    {
        Matrix newHid = hidden[i];
        if(activSwitch)
            newHid.func(dsigmoidFunc);
        if(reluSwitch)
            newHid.func(dreluFunc);
        Matrix gradients = Matrix::scalarProd(Matrix::hdProduct(errorHidden[index], newHid), this->lr);
        ++index;
        Matrix currentLayer = hidden[i - 1];
        currentLayer.transpose();
        Matrix deltas = Matrix::matMul(gradients, currentLayer);
        this->weights[i - 1] = this->weights[i - 1] + deltas;
        if(biasSwitch)
        {
            this->biases[i - 1] = this->biases[i - 1] + gradients;
        }
    }
    return cost;
}