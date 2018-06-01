#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
protected:
    void build() override;
    std::string getFileName() override;
};

#endif