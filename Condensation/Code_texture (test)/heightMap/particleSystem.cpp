#include <particleSystem.h>

particleSystem::particleSystem(int particleAmmount, int mapHeight, int mapWidth) {
	srand(time(0));
    
    this->particleAmmount = particleAmmount;
    this->mapHeigth = mapHeight;
    this->mapWidth = mapWidth;
    this->initiateParticleSystem(&this->Particles);
};

void particleSystem::initiateParticleSystem(std::vector <Droplet> *Particles) {
    for (int i = 0; i < this->particleAmmount; i++) {
        Droplet a;
        this->initiateDroplet(&a);
        Particles->push_back(a);
    }
}

void particleSystem::initiateDroplet(Droplet *a) {    
    a->mass = 100.0f;
    calcRadius(a);
    a->position =  glm::vec3((rand() % (int)(this->mapWidth - 2*(a->radius + 2))) + (int)(a->radius + 2), 
        (rand() % (int)(this->mapHeigth - 2*(a->radius + 2))) + (int)(a->radius + 2), 
        10);
}

void particleSystem::calcRadius(Droplet *a) {
    a->radius = std::cbrt((double)(3*a->mass)/(double)(2*a->density*M_1_PI));
}

std::vector <Droplet> particleSystem::getParticleSystem() {
    return this->Particles;
}

void particleSystem::addParticle(int ammount)
{
    this->particleAmmount += ammount;
    for (int i = 0; i < ammount; i++) {
        Droplet a;
        this->initiateDroplet(&a);
        this->Particles.push_back(a);
    }
}

