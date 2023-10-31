#include <heightMap.h>
#include <particleSystem.h>
#include <time.h>

const unsigned int texWidth = 512;
const unsigned int texHeight = 512;

int main() {
    clock_t tStart = clock();
    particleSystem ParticleSystem(1000, texHeight, texWidth);
    printf("Particle System: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));

    tStart = clock();
	heightMap HeightMap(ParticleSystem.getParticleSystem(), texHeight, texWidth);
    printf("Height Map: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));
}