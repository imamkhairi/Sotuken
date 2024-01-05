#ifndef particleSystem_CLASS_H
#define particleSystem_CLASS_H

#include <vector>
#include <math.h>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#define MINVALUE 0
#define MAXVALUE 1

// Particle
struct Droplet {
	glm::vec2 position;
	float mass; 
	float radius;
	const float density = 1;
};

class particleSystem {
    public:
        particleSystem(int particleAmmount, int maxParticle, int mapHeight, int mapWidth);

        void addParticle(int ammount);
        void updateParticleSystem();

        std::vector<Droplet> &getParticleSystem();
        std::vector<int> &getUpdatedParticles();
        std::vector<int> &getMergingIndex();
        std::vector<int> &getPatchingIndex();

        int getParticleAmmount();
        int getBelow(int i, int j);

        int getMergingCoordinate(int (particleSystem::*getValue)(int), bool flag);
        int getParticleTop(int index);
        int getParticleRight(int index);
        int getParticleBottom(int index);
        int getParticleLeft(int index);
        void updateMergingMass();

        int checkIndex(int valueToFind, std::vector<int> indexVector);
    


    private:
        int particleAmmount;
        int maxParticle;
        int mapHeigth;
        int mapWidth;
        
        std::vector<Droplet> Particles;
        std::vector<int> updatedParticles;
        std::vector<int> mergingIndex;
        std::vector<int> patchingIndex;


        void initiateParticleSystem();
        void initiateDroplet(Droplet *a);
        void initiateUpdatedParticles();
        void calcRadius(Droplet *a);

        int getMergingBottomIndex();

       
        
};

#endif