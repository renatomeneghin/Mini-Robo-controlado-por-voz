#ifndef TINYML_H_INCLUDED
#define TINYML_H_INCLUDED

#define N_LAYERS 10

class TinyML {
private:
    float weigths[N_LAYERS];
    float bias[N_LAYERS];
public:
    TinyML(float, float);
    float interpret();
};

#endif