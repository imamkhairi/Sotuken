#include <particleSystem.h>

particleSystem::particleSystem(int particleAmmount, int mapHeight, int mapWidth) {
	srand(time(0));
    
    this->particleAmmount = particleAmmount;
    this->mapHeigth = mapHeight;
    this->mapWidth = mapWidth;
    this->initiateParticleSystem(&this->Particles); 
    this->drewAmmount = 0;
};

void particleSystem::initiateParticleSystem(std::vector <Droplet> *Particles) {
    for (int i = 0; i < this->particleAmmount; i++) {
        Droplet a;
        this->initiateDroplet(&a, i);
        Particles->push_back(a);
    }
}

void particleSystem::initiateDroplet(Droplet *a, int i) {    
    a->mass = 100.0f;
    calcRadius(a);
    std::vector <glm::vec2> position;
    position.push_back(glm::vec2(10, 10));
    position.push_back(glm::vec2(20, 10));
    position.push_back(glm::vec2(15, 15));
    // a->position =  glm::vec3((rand() % (int)(this->mapWidth - 2*(a->radius + 2))) + (int)(a->radius + 2), 
    //     (rand() % (int)(this->mapHeigth - 2*(a->radius + 2))) + (int)(a->radius + 2), 
    //     10);
    a->position = glm::vec3(position[i].x, position[i].y, 10);
}

void particleSystem::calcRadius(Droplet *a) {
    a->radius = std::cbrt((double)(3*a->mass)/(double)(2*a->density*M_1_PI));
}


void particleSystem::addParticle(int ammount)
{
    this->particleAmmount += ammount;
    for (int i = 0; i < ammount; i++) {
        Droplet a;
        this->initiateDroplet(&a, i);
        this->Particles.push_back(a);
    }
}

void particleSystem::setDrewAmmount(int value)
{
    this->drewAmmount = value;
}

std::vector <Droplet> particleSystem::getParticleSystem() {
    return this->Particles;
}

int particleSystem::getParticleAmmount()
{
    return this->particleAmmount;
}

int particleSystem::getDrewAmmount() 
{
    return this->drewAmmount;
}