#include <heightMap.h>
#include <particleSystem.h>
#include <time.h>
#include <unistd.h>

const unsigned int texWidth = 1024;
const unsigned int texHeight = 1024;

int main() {
    clock_t tStart = clock();
    particleSystem ParticleSystem(3, texHeight, texWidth);
    // printf("Particle System: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));

	heightMap HeightMap(&ParticleSystem, texHeight, texWidth);
    // std::cout << ParticleSystem.getDrewAmmount() << std::endl;
    for (int i = 0; i < 1000; i++) {
        tStart = clock();
        ParticleSystem.addParticle(1);
        HeightMap.updateHeightMap(&ParticleSystem);
        printf("Height Map: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));
        usleep(50000); 
    }
}