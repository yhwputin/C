//
//  TestMC.cpp
//  MonteCarlo
//
//  C++ code to price an option, essential algorithms.
//
//  We take CEV model with a choice of the elaticity parameter
//  and teh Euler method. We give option price and number of times
//  S hits the origin.
//
//  Modified by Changheng Chen on 2/27/17.
//  Copyright © 2017 Changheng Chen. All rights reserved.
//

#include "OptionData.hpp"
#include "SDandSE.hpp"
#include "UtilitiesDJD/RNG/NormalGenerator.cpp"
#include "UtilitiesDJD/Geometry/Range.cpp"
#include "UtilitiesDJD/VectorsAndMatrices/ArrayMechanisms.cpp"
#include <vector>
#include <iostream>
using namespace std;


template <class T> void print (const vector<T>& l)
{ // A generic print function for vectors
    
    cout << endl << "Size of vector is " << l.size() << "\n[";
    
    // We must use a const iterator here, otherwise we get a compiler error.
    typename vector<T>::const_iterator i;
    for (i = l.begin(); i != l.end(); ++i)
    {
        cout << *i << ",";
    }
    
    cout << "]\n";
}

namespace SDEDefinition
{ // Defines drift + diffusion + data
    
    OptionData* data;        // The data for the option MC
    
    double drift(double t, double X)
    { // Drift term
        
        return (data->r)*X; // r - D
        // return (date->r-D)*X;
    }
    
    double diffusion(double t, double X)
    { // Diffusion term
        
        double betaCEV = 1.0;
        return data->sig * pow(X, betaCEV);
    }
    
    double diffusionDerivative(double t, double X)
    { // Diffusion term, needed for the Milstein method
        
        double betaCEV = 1.0;
        return 0.5 * (data->sig) * (betaCEV) * pow(X, 2.0 * betaCEV - 1.0);
    }
} // End of namespace



int main()
{
    cout << "1 factor Monte Carlo with explicit Euler\n";
    
    OptionData myOption;
    myOption.K = 100.0;
    myOption.T = 1.0;
    myOption.r = 0.0;
    myOption.sig = 0.2;
    myOption.type = 1; // Put -1, call +1
    double S_0 = 100;
    
    long N = 100;
    cout << "Number of subintervals in time: ";
    cin >> N;
    
    // Create the basic SDE (Context class)
    Range<double> range (0.0, myOption.T);
    double VOld = S_0;
    double VNew = VOld; // Initialized it
    
    Vector<double, long> x = range.mesh(N);
    
    // V2 mediator stuff
    long NSim = 50000;
    cout << "Number of simulations: ";
    cin >> NSim;
    
    double k = myOption.T / double(N);
    double sqrk = sqrt(k);
    
    // Normal random number
    double dW;
    double price = 0.0;  // Option price
    vector<double> CTjs; // Vector to store prices
       
    // NormalGenerator is a base class
    NormalGenerator* myNormal = new BoostNormal();
    
    using namespace SDEDefinition;
    data = &myOption;
    
    vector<double> res;
    int coun = 0; // Number of times S hits origin
    
    // A.
    for (long i = 1; i <= NSim; ++i)
    { // Calculate a path at each iteration
        
        if ((i/10000) * 10000 == i)
        { // Give status after each 1000th iteration
            
            cout << i << endl;
        }
        
        VOld = S_0;
        for (long index = x.MinIndex()+1; index <= x.MaxIndex(); ++index)
        {
            // Create a random number
            dW = myNormal->getNormal();
            
            // The FDM (in this case explicit Euler)
            VNew = VOld + (k * drift(x[index-1], VOld))
            + (sqrk * diffusion(x[index-1], VOld) * dW);
            
            VOld = VNew;
            
            // Spurious values
            if (VNew <= 0) coun++;
        }
        
        double tmp = myOption.myPayOffFunction(VNew); // Calculate call price at t=T
        price += (tmp)/double(NSim);
        
        CTjs.push_back(tmp);
    }
    
    // D. Finally, discouting the average price
    price *= exp(-myOption.r * myOption.T);
    
    // Cleanup; V2 use scoped pointer
    delete myNormal;
    
    cout << "Price, after discounting: " << price << ", " << endl;
    cout << "Number of times origin is hit: " << coun << endl;
    cout << "SD: " << optionSD(CTjs,myOption.r,myOption.T)
       << ", SE: " << optionSE(CTjs,myOption.r,myOption.T) << endl;
    
    return 0;
}
