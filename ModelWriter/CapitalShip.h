#ifndef CAPITAL_SHIP_H
#define CAPITAL_SHIP_H

#include "Shape.h"

class CapitalShip : public Shape
{
public:
    void build() override;
    std::string getFileName() override;
};

#endif
