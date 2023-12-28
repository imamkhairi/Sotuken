#ifndef particleSystem_CLASS_H
#define particleSystem_CLASS_H

#include <vector>
#include <math.h>
#include <iostream>
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

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
        static void print();

        void addParticle(int ammount);
        void updateParticleSystem();

        std::vector <Droplet> &getParticleSystem();
        std::vector <int> &getUpdatedParticles();
        int getParticleAmmount();
    


    private:
        int particleAmmount;
        int maxParticle;
        int mapHeigth;
        int mapWidth;
        std::vector <Droplet> Particles;
        std::vector <int> updatedParticles;

        void initiateParticleSystem();
        void initiateDroplet(Droplet *a);
        void initiateUpdatedParticles();
        void calcRadius(Droplet *a);

};

#endif