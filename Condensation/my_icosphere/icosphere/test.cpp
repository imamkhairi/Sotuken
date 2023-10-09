#include <icosphere.h>

int main() {
    Icosphere sphere(0.0f, 0.0f, 0.0f, 10.0f);

    for (int i = 0; i < (int)sphere.vertices.size(); i) {
        std::cout << sphere.vertices[i++] << " " <<  sphere.vertices[i++] << " " << sphere.vertices[i++] << " " << std::endl;
        // std::cout << sphere.indices[i++] << " " <<  sphere.indices[i++] << " " << sphere.indices[i++] << " " << std::endl;
        if(i%9==0) std::cout << std::endl;
    }
}