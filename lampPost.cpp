#include "lampPost.h"

LampPost::LampPost()
{
    location = {0, 15, 0};
    poleRadius = 4;
    poleHeight = 30;
    lightRadius = 5;
    lightHeight = 8;
    baseHeight = poleHeight/6;
    poleColor = {0, 0.5, 0.1, 1.0};
    lightColor = {1.0, 1.0, 0.5, 1.0};
    lightIntensity = 8;
    initializeSolids();
}
LampPost::LampPost(Point inputLocation, double inputPoleRadius, double inputPoleHeight, double inputLightRadius,
double inputLightHeight, RGBAcolor inputPoleColor, RGBAcolor inputLightColor, double inputLightIntensity)
{
    location = inputLocation;
    poleRadius = inputPoleRadius;
    poleHeight = inputPoleHeight;
    lightRadius = inputLightRadius;
    lightHeight = inputLightHeight;
    baseHeight = poleHeight/6;
    poleColor = inputPoleColor;
    lightColor = inputLightColor;
    lightIntensity = inputLightIntensity;
    initializeSolids();
}

void LampPost::initializeSolids()
{
    // Make the base (cone-shaped)
    Point baseCenter = {location.x, baseHeight/2, location.z};
    double baseRadius = 3.5*poleRadius;
    double baseTopRadius = 2.5*poleRadius;
    std::shared_ptr<EllipticCyl> base = std::make_shared<EllipticCyl>(EllipticCyl(baseCenter, poleColor,
    2*baseRadius, baseHeight, 2*baseRadius, {1,1,1,1},
            2*baseTopRadius, 2*baseTopRadius,
            NoLines));
    solids.push_back(base);

    // Pole
    Point poleCenter = {location.x, baseCenter.y + baseHeight/2 + poleHeight/2, location.z};
    solids.push_back(std::make_shared<EllipticCyl>(EllipticCyl(poleCenter, poleColor, 2*poleRadius, poleHeight, 2*poleRadius, {1,1,1,1}, NoLines)));

    // Light bulb
    Point lightCenter = getLightLocation();
    solids.push_back(std::make_shared<EllipticCyl>(EllipticCyl(lightCenter, lightColor, 2*lightRadius, lightHeight, 2*lightRadius, {1,1,1,1}, NoLines)));

    // Light covering
    RGBAcolor glass = {1.0, 1.0, 1.0, 0.5};
    solids.push_back(std::make_shared<Frustum>(Frustum(lightCenter, glass, 2*baseTopRadius, lightHeight, 2*baseTopRadius, poleColor, 2*baseRadius, 2*baseRadius)));

    // Roof
    double roofHeight = lightHeight/3;
    Point roofCenter = {location.x, lightCenter.y + lightHeight/2 + roofHeight/2, location.z};
    solids.push_back(std::make_shared<Frustum>(Frustum(roofCenter, poleColor, 2*baseRadius+1, roofHeight, 2*baseRadius+1, {1,1,1,1}, poleRadius, poleRadius, NoLines)));
}

// Getters
Point LampPost::getLocation() const
{
    return location;
};
double LampPost::getLightIntensity() const
{
    return lightIntensity;
};
Point LampPost::getLightLocation() const
{
    return {location.x, baseHeight + poleHeight + lightHeight/2, location.z};
};

void LampPost::draw(double lightLevel) const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw(lightLevel);
    }
};