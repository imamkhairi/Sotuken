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
    float m = (float) ((rand() % 45) + 15);
    a->mass = m;
    calcRadius(a);
    a->position =  glm::vec2((rand() % (int)(this->mapWidth - 2*(a->radius + 3))) + (int)(a->radius + 3), 
        (rand() % (int)(this->mapHeigth - 2*(a->radius + 3))) + (int)(a->radius + 3));
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
    this->addParticle(5);
    //// gerakan cuma untuk yg lewat threshold
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

std::vector<int> &particleSystem::getMergingIndex()
{
    return this->mergingIndex;
}
std::vector<int> &particleSystem::getPatchingIndex()
{
    return this->patchingIndex;
}

int particleSystem::getBelow(int i, int j) 
{   
    return this->Particles[i].position.y >= this->Particles[j].position.y ? i : j; 
}

int particleSystem::getMergingCoordinate(int (particleSystem::*getValue)(int), bool flag)
{
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

int particleSystem::checkIndex(int valueToFind, std::vector<int> indexVector)
{
    if (std::find(indexVector.begin(), indexVector.end(), valueToFind) != indexVector.end()) 
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

void particleSystem::updateMergingMass()
{
    int bottomIndex = this->getMergingBottomIndex();
    float difMass = 0;
    for (int i : this->mergingIndex) 
    {
        if (i == bottomIndex) 
            continue;
        else 
        {
            if (this->Particles[i].mass > 5) {
                difMass += this->Particles[i].mass * 0.6;
                this->Particles[i].mass -= this->Particles[i].mass * 0.6;
                // std::cout << "after = " << this->Particles[i].mass << std::endl;
                this->calcRadius(&this->Particles[i]);
            }
        }
    }
    
    this->Particles[bottomIndex].mass += difMass * 0.8;
    this->calcRadius(&this->Particles[bottomIndex]);
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