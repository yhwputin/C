//
//  Point.cpp
//  L3S2.2E2
//
//  Created by Christine Chan on 11/12/19.
//  Copyright © 2019 Christine Chan. All rights reserved.
//
#include <sstream>
#include <cmath>
#include <math.h>
#include "Point.hpp"

Point::Point()
{// Default constructor
    px = py =0.0;
}

Point::Point(double x, double y)
{// Initialize using newx and newy
    px = x;
    py = y;
}

Point::~Point()
{// Des...
    cout << "delete point..\n";
}

// Point member function
double Point::GetX() const
{
    return px;
}

// Point member function
double Point::GetY() const
{
    return py;
}

void Point::SetX(double x)
{
    px = x;
}

void Point::SetY(double y)
{
    py = y;
}

void Point::ToString()
{
    // Create an output string stream
    std::ostringstream streamObj;
    streamObj << "Point(";
    // Add double to stream
    streamObj << px;
    streamObj << ", ";
    streamObj << py;
    streamObj << ")";
    
    std::string strObj = streamObj.str();
    
    cout << strObj << endl;
}

double Point::DistanceOrigin()
{
    double d;
    d = std::sqrt((pow(px,2)+pow (py, 2)));
    return d;
}
double Point::Distance(Point p)
{
    double d;
    d = std::sqrt((pow((px-p.px),2)+pow((py-p.py), 2)));
    return d;
}
