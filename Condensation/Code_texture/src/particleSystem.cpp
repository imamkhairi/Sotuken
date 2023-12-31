#include <particleSystem.h>

particleSystem::particleSystem(int particleAmmount, int maxParticle, int mapHeight, int mapWidth) 
{
	srand(time(0));
    this->particleAmmount = particleAmmount;
    this->maxParticle = maxParticle;
    this->mapHeigth = mapHeight;
    this->mapWidth = mapWidth;
    this->initiateParticleSystem(); 
    this->initiateUpdatedParticles();
};

void particleSystem::initiateParticleSystem() 
{
    for (int i = 0; i < this->particleAmmount; i++) {
        Droplet a;
        this->initiateDroplet(&a);
        this->Particles.push_back(a);
    }
}

void particleSystem::initiateDroplet(Droplet *a) 
{    
    // a->mass = 20.0f;
    float m = (float) ((rand() % 32) + 12);
    a->mass = m;
    calcRadius(a);
    a->position =  glm::vec2((rand() % (int)(this->mapWidth - 2*(a->radius + 2))) + (int)(a->radius + 2), 
        (rand() % (int)(this->mapHeigth - 2*(a->radius + 2))) + (int)(a->radius + 2));
}

void particleSystem::initiateUpdatedParticles() 
{
    for (int i = 0; i < this->particleAmmount; i++) 
    {
        this->updatedParticles.push_back(i);
    }
}

void particleSystem::calcRadius(Droplet *a) 
{
    a->radius = std::cbrt((double)(3*a->mass)/(double)(2*a->density*M_1_PI));
}


void particleSystem::addParticle(int ammount)
{   
    for (int i = 0; i < ammount; i++) {
        // std::cout << this->particleAmmount << std::endl;
        if(this->particleAmmount < this->maxParticle) {
            this->particleAmmount += 1;
            this->updatedParticles.push_back(this->particleAmmount-1);
            Droplet a;
            this->initiateDroplet(&a);
            this->Particles.push_back(a);
        }
    }
}

void particleSystem::updateParticleSystem() 
{
    //// gerakan cuma untuk yg lewat threshold
    this->addParticle(5);
    // for (int i = 0; i < this->particleAmmount; i++) {
    //     if (this->Particles[i].position.y < this->mapHeigth/2) {
    //         this->Particles[i].position.y += 1;
    //         this->updatedParticles.push_back(i);
    //     }
    // }
}

std::vector <Droplet> &particleSystem::getParticleSystem() 
{
    return this->Particles;
}

int particleSystem::getParticleAmmount()
{
    return this->particleAmmount;
}

std::vector <int> &particleSystem::getUpdatedParticles()
{
    return this->updatedParticles;
}

