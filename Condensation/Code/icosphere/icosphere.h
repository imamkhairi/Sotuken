#ifndef GEOMETRY_ICOSPHERE_H
#define GEOMETRY_ICOSPHERE_H

#include <vector>
#include <math.h>
#include <iostream>

class Icosphere {
    public:
        // Constructor
        Icosphere(float x0 = 0.0f, float y0 = 0.0f, float z0 = 0.0f, float radius = 1.0f);
        
        float x0;
        float y0;
        float z0;
        float radius;
        
        std::vector <float> vertices;

        std::vector<float> computeIcosahedronVertices();

};

#endif