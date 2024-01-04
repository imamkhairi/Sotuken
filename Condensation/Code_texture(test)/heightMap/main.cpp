#include <heightMap.h>
#include <time.h>
#include <unistd.h>

const unsigned int texWidth  = 40;
const unsigned int texHeight = 40;

int main(int argc, char **argv) {
    particleSystem ParticleSystem(4, texHeight, texWidth);
    // clock_t tStart = clock();
    // printf("Particle System: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));

    // argv[1] berapa kali smoothing?
    // std::cout << std::atoi(argv[1]) << std::endl;

    IDMap idMap(texWidth, texHeight);
	heightMap HeightMap(&ParticleSystem, &idMap, texHeight, texWidth, std::atoi(argv[1]));

    // idMap.print();
}