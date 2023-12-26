#include <heightMap.h>
#include <time.h>
#include <unistd.h>

const unsigned int texWidth  = 40;
const unsigned int texHeight = 40;

int main() {
    // clock_t tStart = clock();
    particleSystem ParticleSystem(3, texHeight, texWidth);
    // printf("Particle System: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));

    IDMap idMap(texWidth, texHeight);
	heightMap HeightMap(&ParticleSystem, texHeight, texWidth);

    idMap.print();

}