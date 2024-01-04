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
    a->mass = 30.0f;
    calcRadius(a);
    std::vector <glm::vec2> position;
    position.push_back(glm::vec2(15, 15));
    position.push_back(glm::vec2(23, 15));
    position.push_back(glm::vec2(19, 20));
    position.push_back(glm::vec2(13, 20));
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


int particleSystem::checkMergingIndex(int valueToFind)
{
    if (std::find(this->mergingIndex.begin(), this->mergingIndex.end(), valueToFind) != this->mergingIndex.end()) 
        return 0;
    else 
        return 1;
}

// not used (debug only)
void particleSystem::printMergingIndex() 
{   
    if (this->mergingIndex.size() > 0) 
    {
        std::cout << "merging: ";
        for (int index : this->mergingIndex) {
            std::cout << index << ", ";
        }
        std::cout << std::endl;
        std::cout << "below = " << this->getMergingBottomIndex() << std::endl;
        // std::cout << "min = " << this->getMergingCooridnate(&particleSystem::getParicleY, MINVALUE) << std::endl;    
        // std::cout << "max = " << this->getMergingCooridnate(&particleSystem::getParicleY, MAXVALUE) << std::endl;    
    }
}

int particleSystem::getMergingBottomIndex() 
{
    int maxValue = this->Particles[this->mergingIndex[0]].position.y;
    int index = this->mergingIndex[0];
    for (int i = 1; i < this->mergingIndex.size(); i++) 
    {
        if (this->Particles[this->mergingIndex[i]].position.y > maxValue) 
        {
            maxValue = this->Particles[this->mergingIndex[i]].position.y;
            index = this->mergingIndex[i];
        }
    }
    return index;
}

void particleSystem::updateMergingMass()
{
    int lowIndex = this->getMergingBottomIndex();
    float difMass = 0;
    for (int i : this->mergingIndex) 
    {
        if (i == lowIndex) 
            continue;
        else 
        {
            if (this->Particles[i].mass > 10) {
                difMass += this->Particles[i].mass * 0.6;
                this->Particles[i].mass -= this->Particles[i].mass * 0.6;
                // std::cout << "after = " << this->Particles[i].mass << std::endl;
                this->calcRadius(&this->Particles[i]);
            }
        }
    }
    
    this->Particles[lowIndex].mass += difMass;
    this->calcRadius(&this->Particles[lowIndex]);
}

int particleSystem::getParticleTop(int index)
{
    return (int)this->Particles[index].position.y - (int)this->Particles[index].radius;
}

int particleSystem::getParticleBottom(int index)
{
    return (int)this->Particles[index].position.y + (int)this->Particles[index].radius;
}

int particleSystem::getParticleRight(int index)
{
    return (int)this->Particles[index].position.x + (int)this->Particles[index].radius;
}

int particleSystem::getParticleLeft(int index)
{
    return (int)this->Particles[index].position.x - (int)this->Particles[index].radius;
}

int particleSystem::getParicleY(int index) 
{
    return (int)this->Particles[index].position.y;
}

int particleSystem::getMergingCooridnate(int (particleSystem::*getValue)(int), bool flag)
{
    int value = (this->*getValue)(this->mergingIndex[0]);
    for (int i = 1; i < this->mergingIndex.size(); i++) 
    {   
        if (flag == MAXVALUE) 
        {
            // std::cout << "MAX value " << std::endl;
            if ((this->*getValue)(i) > value) 
            {
                value = (this->*getValue)(i);
            }
        }
        else 
        {
            // std::cout << "MIN value " << std::endl;
            if ((this->*getValue)(i) < value) 
            {
                // std::cout << "sini" << std::endl;
                value = (this->*getValue)(i);
            }

        }
    }
    return value;
}