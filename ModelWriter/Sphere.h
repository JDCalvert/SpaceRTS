#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere(unsigned int detail);

    void build() override;
    std::string getFileName() override;

private:
    unsigned int detail;
};

#endif