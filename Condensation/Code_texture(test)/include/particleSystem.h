#ifndef particleSystem_CLASS_H
#define particleSystem_CLASS_H

#define MINVALUE 0
#define MAXVALUE 1

#include <algorithm>
#include <vector>
#include <math.h>
#include <iostream>
#include <time.h>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// Particle
struct Droplet {
	glm::vec3 position;
	float mass; 
	float radius;
	const float density = 1;
};

class particleSystem {
    public:
        particleSystem(int particleAmmount, int mapHeight, int mapWidth);
        static void print();

        std::vector<int> mergingIndex;
        std::vector<int> patchingIndex;
        std::vector <Droplet> getParticleSystem();

        void addParticle(int ammount);
        void setDrewAmmount(int value);
        int getParticleAmmount();
        int getDrewAmmount();
        int getMergingBottomIndex();
        int checkIndex(int valueToFind, std::vector<int> vectorIndex);

        int getParticleTop(int index);
        int getParticleRight(int index);
        int getParticleBottom(int index);
        int getParticleLeft(int index);

        // int getParicleY(int index);

        int getMergingCooridnate(int (particleSystem::*getValue)(int), bool flag);

        void printMergingIndex();
        void updateMergingMass();

    private:
        int particleAmmount;
        int mapHeigth;
        int mapWidth;
        int drewAmmount;
        std::vector <Droplet> Particles;

        void initiateParticleSystem(std::vector <Droplet> *Particles);
        // void initiateDroplet(Droplet *a);
        void initiateDroplet(Droplet *a, int i);
        void calcRadius(Droplet *a);
};

#endif