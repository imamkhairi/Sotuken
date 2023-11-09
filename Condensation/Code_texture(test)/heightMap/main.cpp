#include <heightMap.h>
#include <particleSystem.h>
#include <time.h>
#include <unistd.h>

const unsigned int texWidth  = 50;
const unsigned int texHeight = 50;

int main() {
    clock_t tStart = clock();
    particleSystem ParticleSystem(2, texHeight, texWidth);
    // printf("Particle System: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));

	heightMap HeightMap(&ParticleSystem, texHeight, texWidth);
    // for (int i = 0; i < 250; i++) {
    //     tStart = clock();
    //     ParticleSystem.addParticle(1);
    //     HeightMap.updateHeightMap(&ParticleSystem);
    //     printf("Height Map: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));
    //     usleep(50000); 
    // }

    int *IDMap = (int*)malloc(texHeight * texWidth * sizeof(int));

    // tStart = clock();
    // printf("Smoothing: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));


    int *p = IDMap;
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            *p = 0;
            p++;
        }
    }

    HeightMap.smoothingHeightMap(IDMap, &ParticleSystem);

    p = IDMap;
    for (int y = 0; y < texHeight; y++) {
        // std::cout << y << " | ";
        for (int x = 0; x < texWidth; x++) {
            std::cout << *p << " ";
            p++;
        }
        std::cout << std::endl;
    }

    free(IDMap);

}