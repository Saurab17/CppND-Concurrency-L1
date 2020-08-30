#include <iostream>
#include <thread>
#include <vector>

#include "Vehicle.h"
#include "Street.h"
#include "Intersection.h"
#include "Graphics.h"


#define INTERSECT1_X    (size_t)(1430/SCALE_DOWN_FACTOR)   
#define INTERSECT1_Y    (size_t)(625/SCALE_DOWN_FACTOR)
#define INTERSECT2_X    (size_t)(2575/SCALE_DOWN_FACTOR)   
#define INTERSECT2_Y    (size_t)(1260/SCALE_DOWN_FACTOR)
#define INTERSECT3_X    (size_t)(2200/SCALE_DOWN_FACTOR)
#define INTERSECT3_Y    (size_t)(1950/SCALE_DOWN_FACTOR)
#define INTERSECT4_X    (size_t)(1000/SCALE_DOWN_FACTOR) 
#define INTERSECT4_Y    (size_t)(1350/SCALE_DOWN_FACTOR)
#define INTERSECT5_X    (size_t)(400/SCALE_DOWN_FACTOR)
#define INTERSECT5_Y    (size_t)(1000/SCALE_DOWN_FACTOR)
#define INTERSECT6_X    (size_t)(750/SCALE_DOWN_FACTOR)
#define INTERSECT6_Y    (size_t)(250/SCALE_DOWN_FACTOR)

// NYC
void createTrafficObjects(std::vector<std::shared_ptr<Street>> &streets, std::vector<std::shared_ptr<Intersection>> &intersections, std::vector<std::shared_ptr<Vehicle>> &vehicles, std::string &filename, int nVehicles)
{
    // assign filename of corresponding city map
    filename = "../data/nyc.jpg";

    // init traffic objects
    int nIntersections = 6;
    for (size_t ni = 0; ni < nIntersections; ni++)
    {
        intersections.push_back(std::make_shared<Intersection>());
    }

    // position intersections in pixel coordinates
    intersections.at(0)->setPosition(INTERSECT1_X, INTERSECT1_Y);
    intersections.at(1)->setPosition(INTERSECT2_X, INTERSECT2_Y);
    intersections.at(2)->setPosition(INTERSECT3_X, INTERSECT3_Y);
    intersections.at(3)->setPosition(INTERSECT4_X, INTERSECT4_Y);
    intersections.at(4)->setPosition(INTERSECT5_X, INTERSECT5_Y);
    intersections.at(5)->setPosition(INTERSECT6_X, INTERSECT6_Y);

    // create streets and connect traffic objects
    int nStreets = 7;
    for (size_t ns = 0; ns < nStreets; ns++)
    {
        streets.push_back(std::make_shared<Street>());
    }

    streets.at(0)->setInIntersection(intersections.at(0));
    streets.at(0)->setOutIntersection(intersections.at(1));

    streets.at(1)->setInIntersection(intersections.at(1));
    streets.at(1)->setOutIntersection(intersections.at(2));

    streets.at(2)->setInIntersection(intersections.at(2));
    streets.at(2)->setOutIntersection(intersections.at(3));

    streets.at(3)->setInIntersection(intersections.at(3));
    streets.at(3)->setOutIntersection(intersections.at(4));

    streets.at(4)->setInIntersection(intersections.at(4));
    streets.at(4)->setOutIntersection(intersections.at(5));

    streets.at(5)->setInIntersection(intersections.at(5));
    streets.at(5)->setOutIntersection(intersections.at(0));

    streets.at(6)->setInIntersection(intersections.at(0));
    streets.at(6)->setOutIntersection(intersections.at(3));

    // add vehicles to streets
    for (size_t nv = 0; nv < nVehicles; nv++)
    {
        vehicles.push_back(std::make_shared<Vehicle>());
        vehicles.at(nv)->setCurrentStreet(streets.at(nv%nIntersections));
        vehicles.at(nv)->setCurrentDestination(intersections.at(nv%nIntersections));
    }
}

/* Main function */
int main()
{
    /* PART 1 : Set up traffic objects */

    // create and connect intersections and streets
    std::vector<std::shared_ptr<Street>> streets;
    std::vector<std::shared_ptr<Intersection>> intersections;
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::string backgroundImg;
  
    // Task L1.3 : Vary the number of simulated vehicles and use the top function on the terminal or 
    // the task manager of your system to observe the number of threads used by the simulation.   
    int nVehicles = 7;
    createTrafficObjects(streets, intersections, vehicles, backgroundImg, nVehicles);

    /* PART 2 : simulate traffic objects */

    // simulate vehicles
    std::for_each(vehicles.begin(), vehicles.end(), [](std::shared_ptr<Vehicle> &v) {
        v->simulate();
    });

    /* PART 3 : Launch visualization */
    
    // add all objects into common vector
    std::vector<std::shared_ptr<TrafficObject>> trafficObjects;
    std::for_each(intersections.begin(), intersections.end(), [&trafficObjects](std::shared_ptr<Intersection> &intersection) {
        std::shared_ptr<TrafficObject> trafficObject = std::dynamic_pointer_cast<TrafficObject>(intersection);
        trafficObjects.push_back(trafficObject);
    });

    std::for_each(vehicles.begin(), vehicles.end(), [&trafficObjects](std::shared_ptr<Vehicle> &vehicles) {
        std::shared_ptr<TrafficObject> trafficObject = std::dynamic_pointer_cast<TrafficObject>(vehicles);
        trafficObjects.push_back(trafficObject);
    });

    // draw all objects in vector
    Graphics *graphics = new Graphics();
    graphics->setBgFilename(backgroundImg);
    graphics->setTrafficObjects(trafficObjects);
    graphics->simulate();
}