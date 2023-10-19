#ifndef particleSystem_CLASS_H
#define particleSystem_CLASS_H

#include <vector>
#include <math.h>
#include <iostream>
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

        std::vector <Droplet> getParticleSystem();

    private:
        int particleAmmount;
        int mapHeigth;
        int mapWidth;
        std::vector <Droplet> Particles;

        void initiateParticleSystem(std::vector <Droplet> *Particles);
        void initiateDroplet(Droplet *a);
        void calcRadius(Droplet *a);
};

#endif