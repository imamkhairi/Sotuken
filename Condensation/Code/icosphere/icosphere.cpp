#include <icosphere.h>

Icosphere::Icosphere(float x0, float y0, float z0, float radius) {
    this->x0 = x0;
    this->y0 = y0;
    this->z0 = z0;
    this->radius = radius;
    this->subdivision = 1;

    buildVertices();
}

std::vector<float> Icosphere::computeIcosahedronVertices() {
    const float PI = acos(-1);
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    std::vector<float> vertices(12 * 3);    // 12 vertices
    int i1, i2;                             // indices
    float z, xy;                            // coords
    float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 2nd row
    float hAngle2 = -PI / 2;                // start from -90 deg at 3rd row

    // the first top vertex (0, 0, r)
    vertices[0] = 0;
    vertices[1] = 0;
    vertices[2] = radius;

    // 10 vertices at 2nd and 3rd rows
    for(int i = 1; i <= 5; ++i)
    {
        i1 = i * 3;         // for 2nd row
        i2 = (i + 5) * 3;   // for 3rd row

        z = radius * sinf(V_ANGLE);             // elevaton
        xy = radius * cosf(V_ANGLE);

        vertices[i1] = xy * cosf(hAngle1);      // x
        vertices[i2] = xy * cosf(hAngle2);
        vertices[i1 + 1] = xy * sinf(hAngle1);  // x
        vertices[i2 + 1] = xy * sinf(hAngle2);
        vertices[i1 + 2] = z;                   // z
        vertices[i2 + 2] = -z;

        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;
    }

    // the last bottom vertex (0, 0, -r)
    i1 = 11 * 3;
    vertices[i1] = 0;
    vertices[i1 + 1] = 0;
    vertices[i1 + 2] = -radius;

    // for (int i = 0; i < 36; i) {
    //     std::cout << vertices[i++] << " ";
    //     std::cout << vertices[i++] << " ";
    //     std::cout << vertices[i++] << std::endl;
    // }

    return vertices;
}

void Icosphere::subdivideVerticesFlat() {
    std::vector<float> tmpVertices;
    std::vector<unsigned int> tmpIndices;
    const float *v1, *v2, *v3;          // ptr to original vertices of a triangle
    float newV1[3], newV2[3], newV3[3]; // new vertex positions
    unsigned int index;

    // iterate all subdivision levels
    for(int i = 1; i <= subdivision; ++i)
    {
        // copy prev vertex/index arrays and clear
        tmpVertices = vertices;  // copy
        tmpIndices = indices;    // copy
        vertices.clear(); // clear
        indices.clear();  // clear
        index = 0;

        // perform subdivision for each triangle
        for(int j = 0; j < tmpIndices.size(); j += 3)
        {
            // get 3 vertices of a triangle
            v1 = &tmpVertices[tmpIndices[j] * 3];
            v2 = &tmpVertices[tmpIndices[j + 1] * 3];
            v3 = &tmpVertices[tmpIndices[j + 2] * 3];

            // compute 3 new vertices by spliting half on each edge
            //         v1       
            //        / \       
            // newV1 *---* newV3
            //      / \ / \     
            //    v2---*---v3   
            //       newV2      
            computeHalfVertex(v1, v2, newV1);
            computeHalfVertex(v2, v3, newV2);
            computeHalfVertex(v1, v3, newV3);

            // add 4 new triangles to vertex array
            addVertices(v1,    newV1, newV3);
            addVertices(newV1, v2,    newV2);
            addVertices(newV1, newV2, newV3);
            addVertices(newV3, newV2, v3);

            // add indices of 4 new triangles
            addIndices(index,   index+1, index+2);
            addIndices(index+3, index+4, index+5);
            addIndices(index+6, index+7, index+8);
            addIndices(index+9, index+10,index+11);
            index += 12;    // next index
        }
    }

}

void Icosphere::computeHalfVertex(const float v1[3], const float v2[3], float newV[3])
{
    newV[0] = v1[0] + v2[0];    // x
    newV[1] = v1[1] + v2[1];    // y
    newV[2] = v1[2] + v2[2];    // z
    float scale = radius / sqrtf(newV[0]*newV[0] + newV[1]*newV[1] + newV[2]*newV[2]);
    newV[0] *= scale;
    newV[1] *= scale;
    newV[2] *= scale;
}

void Icosphere::addVertices(const float v1[3], const float v2[3], const float v3[3])
{
    this->vertices.push_back(v1[0]);  // x
    this->vertices.push_back(v1[1]);  // y
    this->vertices.push_back(v1[2]);  // z

    this->vertices.push_back(v2[0]);
    this->vertices.push_back(v2[1]);
    this->vertices.push_back(v2[2]);
    
    this->vertices.push_back(v3[0]);
    this->vertices.push_back(v3[1]);
    this->vertices.push_back(v3[2]);
}


void Icosphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    this->indices.push_back(i1);
    this->indices.push_back(i2);
    this->indices.push_back(i3);
}

void Icosphere::buildVertices() {
    std::vector <float> tmpVertices = computeIcosahedronVertices();

    // clear memory
    std::vector <float>().swap(vertices);
    std::vector <float>().swap(normals);
    std::vector <unsigned int>().swap(indices);

    const float *v0, *v1, *v2, *v3, *v4, *v11;          // vertex positions
    float n[3];                                         // face normal
    unsigned int index = 0;

    v0 = &tmpVertices[0];       // 1st vertex
    v11 = &tmpVertices[11 * 3]; // 12th vertex
    for (int i = 1; i <= 5; i++) {

    }
}

void Icosphere::computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3])
{
    const float EPSILON = 0.000001f;

    // default return value (0, 0, 0)
    n[0] = n[1] = n[2] = 0;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = v2[0] - v1[0];
    float ey1 = v2[1] - v1[1];
    float ez1 = v2[2] - v1[2];
    float ex2 = v3[0] - v1[0];
    float ey2 = v3[1] - v1[1];
    float ez2 = v3[2] - v1[2];

    // cross product: e1 x e2
    float nx, ny, nz;
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if(length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        n[0] = nx * lengthInv;
        n[1] = ny * lengthInv;
        n[2] = nz * lengthInv;
    }
}