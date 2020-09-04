#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool SortDistance (const vector <double>& v1, const vector <double>& v2)    {
    return v1.at(1) < v2.at(1);
}

int main()  {
    cout << "Enter the length of the beam in metres: ";
    double beamLength {};
    cin >> beamLength;

    cout << "Enter the number of point forces: ";
    int numberPointForces {};
    cin >> numberPointForces;
    
    cout << "Enter the magnitude of each force followed by the distance in metres from the left end of the beam: ";
    vector <vector<double>> pointForceVector_2d {};                 
    vector <double> pointForceVector {1, 0};                              /*Uses a 1D vector to intiliase a point force and a distance from the left hand side of the beam.*/
    int countPointForce {0};                                                      /*1D vector is pushed back into 2D vector which comprises of all the point forces and distances.*/                    
    while (countPointForce < numberPointForces) {                             /*Iterates until all point forces specified by the user are inputted.*/       
        cin >> pointForceVector.at(0) >> pointForceVector.at(1);
        if (pointForceVector.at(1) <= beamLength)   {                               /*If statement checks that forces are within the user specified beam length*/
            pointForceVector_2d.push_back(pointForceVector);
            cout << pointForceVector_2d.at(countPointForce).at(0) << " Newtons " << pointForceVector_2d.at(countPointForce).at(1) << " metres from the left" << endl;
            countPointForce++;  
        }
        else    {
            cout << "The distance of a point forces must be less than the length of the beam:" << endl;
        }
    }
    cout << "Number of point forces = " << (countPointForce)<< endl;

    cout << "Enter the distance of both supports from the left side: " ;
    vector <double> SupportDistances {};
    double supportUserInput {};                                     /*User inputs the position of two supports which are stored in another 1D Vector.*/
    int countSupport {0};                                           /*The use of a vector in this case allows for future versions to compute beams with more than 2 supports*/
    while (countSupport < 2)    {
        cin >> supportUserInput;
        if (supportUserInput <= beamLength) {
            SupportDistances.push_back(supportUserInput);
            cout << "support " << SupportDistances.at(countSupport) << " metres from the left" << endl;
            countSupport++; 
        }
        else    {
            cout << "The distance of a support must be less than the length of the beam:" << endl;
        }
    }
    cout << "Number of supports = " << countSupport << endl;

    double R_b_ccw {};
    double R_b_cw {};
    int R_bCount {};
    while (R_bCount < (countPointForce))    {                                                                                          /*Iterates through all forces in 2D vector*/
        if (pointForceVector_2d.at(R_bCount).at(1) < SupportDistances.at(0))    {                                                      /*and checks whether they induce a cw or ccw*/
            R_b_ccw += (SupportDistances.at(0) - pointForceVector_2d.at(R_bCount).at(1)) * pointForceVector_2d.at(R_bCount).at(0);     /*moment. Both are summed up seperately .*/
        }
        else {
            R_b_cw += (pointForceVector_2d.at(R_bCount).at(1) - SupportDistances.at(0)) * pointForceVector_2d.at(R_bCount).at(0);
        }
        R_bCount++;
    }
    double R_b {(R_b_ccw - R_b_cw) / -(SupportDistances.at(1) - SupportDistances.at(0))};         /*calculates the reaction force at the second support by summing up the cw and ccw moments*/
    double pointForceSum {};                                                                      /*and dividing by the distance between the 2 supports*/
    int sumCount {};
    while (sumCount < (countPointForce))    {                                                     /*Iterates through all forces in 2D vector and sums them up*/
        pointForceSum += pointForceVector_2d.at(sumCount).at(0);
        sumCount++;
    }
    cout << "Sum of point forces = " << pointForceSum << " Newtons" << endl;                
    double R_a {pointForceSum - R_b};                                                             /*Presumes beam is in equilibrium and uses sum of point forces and reaction forces */
    cout << "R_a at " << SupportDistances.at(0) << " metres = " << R_a << " Newtons" << endl;     /*to calculate the reaction at the first support.*/
    cout << "R_b at " << SupportDistances.at(1) << " metres = " << R_b << " Newtons" << endl;

    pointForceVector.at(0) = 0;
    pointForceVector.at(1) = 0;                                                    /*Adds the reaction forces to the 2D vector */
    pointForceVector_2d.push_back(pointForceVector);

    pointForceVector.at(0) = - R_a;
    pointForceVector.at(1) = SupportDistances.at(0);                                                    /*Adds the reaction forces to the 2D vector */
    pointForceVector_2d.push_back(pointForceVector);

    pointForceVector.at(0) = - R_b;
    pointForceVector.at(1) = SupportDistances.at(1);
    pointForceVector_2d.push_back(pointForceVector);

    pointForceVector.at(0) = 0;
    pointForceVector.at(1) = beamLength;                                                    /*Adds the reaction forces to the 2D vector */
    pointForceVector_2d.push_back(pointForceVector);

    sort(pointForceVector_2d.begin(), pointForceVector_2d.end(), SortDistance);

    for (int i {}; i < pointForceVector_2d.size(); i++)   {
        cout << pointForceVector_2d.at(i).at(0) << " Newtons " << pointForceVector_2d.at(i).at(1) << " Metres" << endl;
    }

    return 0;
}

