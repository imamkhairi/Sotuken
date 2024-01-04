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
        // std::cout << i << " :" << a.position.x << ", " << a.position.y << std::endl;
        Particles->push_back(a);
    }
}

void particleSystem::initiateDroplet(Droplet *a, int i) {    
    a->mass = 30.0f;
    calcRadius(a);
    std::vector <glm::vec2> position;
    
    //// for patching problem
    // position.push_back(glm::vec2(23, 9));
    // position.push_back(glm::vec2(31, 7));
    // position.push_back(glm::vec2(9, 25));
    // position.push_back(glm::vec2(21, 10));
    // a->position = glm::vec3(position[i].x, position[i].y, 10);

    //// smoothing problem
    // position.push_back(glm::vec2(19, 22));
    // position.push_back(glm::vec2(12, 14));
    // position.push_back(glm::vec2(20, 10));
    // position.push_back(glm::vec2(31, 24));
    // a->position = glm::vec3(position[i].x, position[i].y, 10);

    //// bleeding problem
    position.push_back(glm::vec2(9, 24));
    position.push_back(glm::vec2(12, 22));
    position.push_back(glm::vec2(23, 18));
    position.push_back(glm::vec2(9, 30));
    a->position = glm::vec3(position[i].x, position[i].y, 10);

    // a->position =  glm::vec3((rand() % (int)(this->mapWidth - 2*(a->radius + 2))) + (int)(a->radius + 2), 
    //     (rand() % (int)(this->mapHeigth - 2*(a->radius + 2))) + (int)(a->radius + 2), 
    //     10);
    
    // std::cout << a->position.x << ", " << a->position.y << std::endl;
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
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

// not used (debug only)
void particleSystem::printMergingIndex() 
{   
    std::cout << "merging: ";
    for (int &index: this->mergingIndex) 
    {
        std::cout << index << ", ";
    }
    std::cout << std::endl;
    // std::cout << "below = " << this->getMergingBottomIndex() << std::endl;

    // std::cout << "DEBUGGING X0 : " << std::endl;
    // for (int index : this->mergingIndex) {
    //     std::cout << index << ": " << this->getParticleLeft(index) - 1 << std::endl;
    // }

    // std::cout << "DEBUGGING Y0 : " << std::endl;
    // for (int index : this->mergingIndex) {
    //     std::cout << index << ": " << this->getParticleTop(index) - 1 << std::endl;
    // }
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

// int particleSystem::getParicleY(int index) 
// {
//     return (int)this->Particles[index].position.y;
// }

int particleSystem::getMergingCooridnate(int (particleSystem::*getValue)(int), bool flag)
{
    // this->printMergingIndex();

    int value = (this->*getValue)(this->mergingIndex[0]);
    for (int i = 1; i < this->mergingIndex.size(); i++) 
    {   
        if (flag == MAXVALUE) 
        {
            if ((this->*getValue)(this->mergingIndex[i]) > value) 
            {
                value = (this->*getValue)(this->mergingIndex[i]);
            }
        }
        else 
        {
            if ((this->*getValue)(this->mergingIndex[i]) < value) 
            {
                value = (this->*getValue)(this->mergingIndex[i]);
            }

        }
    }
    return value;
}