#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape
{
public:
    unsigned int detail;

    Cylinder(unsigned int detail);

protected:
    void build() override;
    std::string getFileName() override;
};

#endif