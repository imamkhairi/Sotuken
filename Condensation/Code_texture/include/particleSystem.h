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
	glm::vec3 position;
	float mass; 
	float radius;
	const float density = 1;
};

class particleSystem {
    public:
        particleSystem(int particleAmmount, int mapHeight, int mapWidth);
        static void print();

        void addParticle(int ammount);

        void setDrewAmmount(int value);

        std::vector <Droplet> getParticleSystem();
        int getParticleAmmount();
        int getDrewAmmount();

    private:
        int particleAmmount;
        int mapHeigth;
        int mapWidth;
        int drewAmmount;
        std::vector <Droplet> Particles;

        void initiateParticleSystem(std::vector <Droplet> *Particles);
        void initiateDroplet(Droplet *a);
        void calcRadius(Droplet *a);
        void updateParticleSystem();

};

#endif