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

void particleSystem::initiateDroplet(Droplet *a, int i, int lowIndex, int mass) 
{
    a->mass = mass;
    calcRadius(a);
    int midX = this->calcMidX(i, lowIndex);
    int midY = this->calcMidY(i, lowIndex);

    a->position = glm::vec3(midX, midY, 10);

}

void particleSystem::initiateDroplet(Droplet *a, int i) {    
    a->mass = 30.0f;
    calcRadius(a);
    std::vector <glm::vec2> position;
    
    //// for patching problem
    // position.push_back(glm::vec2(22, 25));
    // position.push_back(glm::vec2(18, 17));
    // position.push_back(glm::vec2(20, 21));
    // position.push_back(glm::vec2(30, 15));
    // a->position = glm::vec3(position[i].x, position[i].y, 10);

    //// smoothing problem
    position.push_back(glm::vec2(22, 18));
    position.push_back(glm::vec2(21, 22));
    position.push_back(glm::vec2(27, 24));
    position.push_back(glm::vec2(30, 26));
    a->position = glm::vec3(position[i].x, position[i].y, 10);

    //// bleeding problem
    // position.push_back(glm::vec2(16, 16));
    // position.push_back(glm::vec2(19, 12));
    // position.push_back(glm::vec2(10, 22));
    // position.push_back(glm::vec2(15, 17));
    // a->position = glm::vec3(position[i].x, position[i].y, 10);

    // a->position =  glm::vec3((rand() % (int)(this->mapWidth - 2*(a->radius + 3))) + (int)(a->radius + 3), 
    //     (rand() % (int)(this->mapHeigth - 2*(a->radius + 3))) + (int)(a->radius + 3), 
    //     10);
    
    std::cout << a->position.x << ", " << a->position.y << std::endl;
}

void particleSystem::calcRadius(Droplet *a) {
    a->radius = std::cbrt((double)(3*a->mass)/(double)(2*a->density*M_1_PI));
}


void particleSystem::addParticle(int i, int bottomIndex, float mass)
{
    this->particleAmmount++;
    

    Droplet a;
    this->initiateDroplet(&a, i, bottomIndex, mass);
    this->Particles.push_back(a);

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


int particleSystem::checkIndex(int valueToFind, std::vector<int> vectorIndex)
{
    if (std::find(vectorIndex.begin(), vectorIndex.end(), valueToFind) != vectorIndex.end()) 
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
    std::vector<int> midParticle;
    for (int i : this->mergingIndex) 
    {
        if (i == lowIndex) 
            continue;
        else 
        {
            if (this->Particles[i].mass > 5) {
                float currentMass = this->Particles[i].mass;
                this->Particles[i].mass -= currentMass * 0.8; 
                this->calcRadius(&this->Particles[i]);
                
                difMass += currentMass * 0.2;
                
                midParticle.push_back(this->Particles.size());
                this->addParticle(i, lowIndex, currentMass * 0.5);
            }
        }
    }
    
    // for (auto &i : midParticle)
    // {
    //     this->mergingIndex.push_back(i);
    // }

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

int particleSystem::calcMidX(int i, int bottomIndex) 
{
    int x0 = this->Particles[i].position.x;
    int x1 = this->Particles[bottomIndex].position.x;
    return (int)(x0+x1)/2;
}

int particleSystem::calcMidY(int i, int bottomIndex) 
{
    int y0 = this->Particles[i].position.y;
    int y1 = this->Particles[bottomIndex].position.y;
    return (int)(y0+y1)/2;
}