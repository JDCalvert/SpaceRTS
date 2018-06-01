#include <vector>

#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Triangle.h"

#include "CapitalShip.h"

int main()
{
    std::vector<Shape*> shapes;
    shapes.push_back(new Cube());
    shapes.push_back(new Cylinder(30));
    shapes.push_back(new Sphere(30));
    shapes.push_back(new Triangle());

    shapes.push_back(new CapitalShip());

    for (unsigned int i=0; i<shapes.size(); i++)
    {
        Shape* shape = shapes[i];
        shape->buildAndWrite();
    }

    return 0;
}