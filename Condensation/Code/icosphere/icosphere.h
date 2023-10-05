#ifndef GEOMETRY_ICOSPHERE_H
#define GEOMETRY_ICOSPHERE_H

#include <vector>
#include <math.h>
#include <iostream>

class Icosphere {
    public:
        // Constructor
        Icosphere(float x0 = 0.0f, float y0 = 0.0f, float z0 = 0.0f, float radius = 1.0f);
        
    
    private:
        float x0;
        float y0;
        float z0;
        float radius;
        int subdivision;
        
        std::vector <float> vertices;
        std::vector <float> normals;
        std::vector <unsigned int> indices;

        std::vector<float> computeIcosahedronVertices();
        void subdivideVerticesFlat();
        void computeHalfVertex(const float v1[3], const float v2[3], float newV[3]);
        void buildVertices();
        void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3]);

        void addVertices(const float v1[3], const float v2[3], const float v3[3]);
        void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

};

#endif