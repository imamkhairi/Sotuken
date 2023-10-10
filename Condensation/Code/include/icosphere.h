#ifndef GEOMETRY_ICOSPHERE_H
#define GEOMETRY_ICOSPHERE_H

#include <vector>
#include <math.h>
#include <iostream>

class Icosphere {
    public:
        // Constructor
        Icosphere(float x0 = 0.0f, float y0 = 0.0f, float z0 = 0.0f, float radius = 1.0f, int subdivision = 3);

        // std::vector <float> getVertices() { return vertices;}; -> doesnot work
        // unsigned int getVertexCount() const { return (unsigned int)vertices.size() * sizeof(float);}

        std::vector <float> vertices;
        std::vector <float> normals;
        std::vector <unsigned int> indices;
        
        std::vector <float> interleavedVertices;
        int interleavedStride;
        
    
    private:
        float x0;
        float y0;
        float z0;
        float radius;
        int subdivision;
        
        

        static void computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3]);
        static void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3]);
        static float computeScaleForLength(const float v[3], float length);

        void subdivideVerticesFlat();
        void buildVertices();
        void buildInterleavedVertices();
        void zThreshold(float z[3]);

        std::vector<float> computeIcosahedronVertices();

        void addVertices(const float v1[3], const float v2[3], const float v3[3]);
        void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        void addNormals(const float n1[3], const float n2[3], const float n3[3]);
        
};

#endif