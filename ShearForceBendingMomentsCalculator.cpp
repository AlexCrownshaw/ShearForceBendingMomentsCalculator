#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

using namespace std;

//Function Prototypes
double inputBeamLength();
vector <vector<double>> inputPointForces(vector <double>, double);
vector <vector<double>> inputUDL(vector <double>, double);
vector <double> inputSupports(double);
vector <double> UDL2Point(vector <vector<double>>, vector <double>, int);
double computeSum(vector <vector<double>>);
double computeR_b(vector <vector<double>>, vector <double>);
double computeR_a(double, double);
void outputReactions(double, double, double);
vector <vector<double>> createForceVector(vector <vector<double>>, vector <vector<double>>, vector <double>, vector <double>, double, double);
bool SortDistance(const vector <double>&, const vector <double>&);
void outputForceVector(vector <vector<double>>);
double computeStepSize(double);
double computeVx(vector <vector<double>>, double, int);
double computeMx(vector <vector<double>>, double, int);
double computeVx_max(double, double);
double computeMx_max(double, double);
void outputSFBM(vector <vector<double>>);
void outputMax(double, double);
void programExit();

int main() {
    double beamLength{ inputBeamLength() };
    vector <double> vec{ 0, 0, 0 };
    vector <vector<double>> pointForceVector_2d{ inputPointForces(vec, beamLength) };
    vector <vector<double>> UDLVector_2d{ inputUDL(vec, beamLength) };
    vector <double> supportDistances{ inputSupports(beamLength) };

    for (int i{}; i < UDLVector_2d.size(); i++) {
        pointForceVector_2d.push_back(UDL2Point(UDLVector_2d, vec, i));
    }
    double sumPointForces{ computeSum(pointForceVector_2d) };
    double R_b{ computeR_b(pointForceVector_2d, supportDistances) };
    double R_a{ computeR_a(R_b, sumPointForces) };
    outputReactions(sumPointForces, R_a, R_b);

    vector <vector<double>> forceVector{ createForceVector(pointForceVector_2d, UDLVector_2d, supportDistances, vec, R_a, R_b) };
    sort(forceVector.begin(), forceVector.end(), SortDistance);
    outputForceVector(forceVector);

    double stepSize{ computeStepSize(beamLength) };
    double Vx{};
    double Mx{};
    double Vx_max{};
    double Mx_max{};
    vector <vector<double>> SFBM_2D{};
    for (double x{}; x <= beamLength; x += stepSize) {
        for (int i{}; i < forceVector.size(); i++) {
            if (x >= forceVector.at(i).at(1)) {
                Vx += computeVx(forceVector, x, i);
                Mx += computeMx(forceVector, x, i);
            }
            else {
                break;
            }
        }
        vec.at(0) = x;
        vec.at(1) = Vx;
        vec.at(2) = Mx;
        SFBM_2D.push_back(vec);
        Vx_max = computeVx_max(Vx, Vx_max);
        Mx_max = computeMx_max(Mx, Mx_max);
        Vx = 0;
        Mx = 0;
    }
    outputSFBM(SFBM_2D);
    outputMax(Vx_max, Mx_max);
    programExit();
    return 0;
}

double inputBeamLength() {
    cout << "Enter the length of the beam in metres: ";
    double beamLength{};
    cin >> beamLength;
    return beamLength;
}

vector <vector<double>> inputPointForces(vector <double> vec, double beamLength) {
    cout << "Enter the number of point forces: ";
    int numberPointForces{};
    cin >> numberPointForces;
    vector <vector<double>> pointForceVector_2d{};
    for (size_t i{}; i < numberPointForces; i++) {
        cout << "Enter the magnitude of force followed by the distance in metres from the left end of the beam: ";
        cin >> vec.at(0) >> vec.at(1);
        vec.at(2) = 0;
        if (vec.at(1) <= beamLength) {
            pointForceVector_2d.push_back(vec);
            cout << pointForceVector_2d.at(i).at(0) << " Newtons " << pointForceVector_2d.at(i).at(1) << " metres from the left" << endl;
        }
        else {
            cout << "The distance of a point forces must be less than the length of the beam:" << endl;
            i--;
        }
    }
    cout << "Number of point forces = " << pointForceVector_2d.size() << endl;
    cout << "================================================" << endl;
    return pointForceVector_2d;
}

vector <vector<double>> inputUDL(vector <double> vec, double beamLength) {
    cout << "Enter the number of UDL's: ";
    int numberUDL{};
    cin >> numberUDL;
    vector <vector<double>> UDLVector_2d{};
    for (int i{ 0 }; i < numberUDL; i++) {
        cout << "Enter the magnitude of the UDL follwed by the starting distance and the end distance: ";
        cin >> vec.at(0) >> vec.at(1) >> vec.at(2);
        if ((vec.at(1) < beamLength) && (vec.at(2) <= beamLength)) {
            if (vec.at(1) < vec.at(2)) {
                UDLVector_2d.push_back(vec);
                cout << UDLVector_2d.at(i).at(0) << " Newton Metres " << UDLVector_2d.at(i).at(1) << " Metres to " << UDLVector_2d.at(i).at(2) << " Metres" << endl;
            }
            else {
                cout << "The start distance must be less than the end distance." << endl;
                i--;
            }
        }
        else {
            cout << "The UDL must be within the beam length." << endl;
            i--;
        }
    }
    cout << "Number of UDL's = " << UDLVector_2d.size() << endl;
    cout << "================================================" << endl;
    return UDLVector_2d;
}

vector <double> inputSupports(double beamLength) {
    cout << "Enter the distance of both supports from the left side: ";
    vector <double> supportDistances{};
    double supportUserInput{};
    for (size_t i{}; i < 2; i++) {
        cin >> supportUserInput;
        if (supportUserInput <= beamLength) {
            supportDistances.push_back(supportUserInput);
            cout << "support " << supportDistances.at(i) << " metres from the left" << endl;
        }
        else {
            cout << "The distance of a support must be less than the length of the beam:" << endl;
            i--;
        }
    }
    cout << "Number of supports = " << supportDistances.size() << endl;
    cout << "================================================" << endl;
    return supportDistances;
}

vector <double> UDL2Point(vector <vector<double>> UDLVector_2d, vector <double> vec, int i) {
    vec.at(0) = UDLVector_2d.at(i).at(0) * (UDLVector_2d.at(i).at(2) - UDLVector_2d.at(i).at(1));
    vec.at(1) = UDLVector_2d.at(i).at(1) + ((UDLVector_2d.at(i).at(2) - UDLVector_2d.at(i).at(1)) / 2);
    vec.at(2) = 1;
    return vec;
}

double computeSum(vector <vector<double>> pointForceVector_2d) {
    double pointForceSum{};
    for (size_t i{}; i < pointForceVector_2d.size(); i++) {
        pointForceSum += pointForceVector_2d.at(i).at(0);
    }
    return pointForceSum;
}

double computeR_b(vector <vector<double>> pointForceVector_2d, vector <double> supportDistances) {
    double R_b_ccw{};
    double R_b_cw{};
    for (size_t i{}; i < pointForceVector_2d.size(); i++) {
        if (pointForceVector_2d.at(i).at(1) < supportDistances.at(0)) {
            R_b_ccw += (supportDistances.at(0) - pointForceVector_2d.at(i).at(1)) * pointForceVector_2d.at(i).at(0);
        }
        else {
            R_b_cw += (pointForceVector_2d.at(i).at(1) - supportDistances.at(0)) * pointForceVector_2d.at(i).at(0);
        }
    }
    return ((R_b_ccw - R_b_cw) / -(supportDistances.at(1) - supportDistances.at(0)));
}

double computeR_a(double R_b, double sumPointForces) {
    return (sumPointForces - R_b);
}

void outputReactions(double sumPointForces, double R_a, double R_b) {
    cout << "Sum of Forces = " << sumPointForces << endl;
    cout << "Ra = " << R_a << endl;
    cout << "Rb = " << R_b << endl;
    cout << "================================================" << endl;
}

vector <vector<double>> createForceVector(vector <vector<double>> pointForceVector_2d, vector <vector<double>> UDLVector_2d,
    /*                                         */vector <double> supportDistances, vector <double> vec, double R_a, double R_b) {
    vector <vector<double>> forceVector{};
    for (int i{}; i < pointForceVector_2d.size(); i++) {
        if (pointForceVector_2d.at(i).at(2) == 0) {
            vec.at(0) = pointForceVector_2d.at(i).at(0);
            vec.at(1) = pointForceVector_2d.at(i).at(1);
            vec.at(2) = 0;
            forceVector.push_back(vec);
        }
    }
    for (int i{}; i < UDLVector_2d.size(); i++) {
        vec.at(0) = UDLVector_2d.at(i).at(0);
        vec.at(1) = UDLVector_2d.at(i).at(1);
        vec.at(2) = UDLVector_2d.at(i).at(2);
        forceVector.push_back(vec);
    }
    vec.at(0) = -R_a;
    vec.at(1) = supportDistances.at(0);
    vec.at(2) = 0;
    forceVector.push_back(vec);
    vec.at(0) = -R_b;
    vec.at(1) = supportDistances.at(1);
    vec.at(2) = 0;
    forceVector.push_back(vec);
    return forceVector;
}

bool SortDistance(const vector <double>& v1, const vector <double>& v2) {
    return v1.at(1) < v2.at(1);
}

void outputForceVector(vector <vector<double>> forceVector) {
    for (int i{}; i < forceVector.size(); i++) {
        if (forceVector.at(i).at(2) == 0) {
            cout << forceVector.at(i).at(0) << " Newtons " << forceVector.at(i).at(1) << " Metres" << endl;
        }
        else {
            cout << forceVector.at(i).at(0) << " Newtons " << forceVector.at(i).at(1) << " to " << forceVector.at(i).at(2) << " Metres" << endl;
        }
    }
    cout << "================================================" << endl;
}

double computeStepSize(double beamLength) {
    return (beamLength / 1000);
}

double computeVx(vector <vector<double>> forceVector, double x, int i) {
    if (forceVector.at(i).at(2) == 0) {
        return forceVector.at(i).at(0);
    }
    else if (forceVector.at(i).at(2) <= x) {
        return (forceVector.at(i).at(0) * (forceVector.at(i).at(2) - forceVector.at(i).at(1)));
    }
    else {
        return (forceVector.at(i).at(0) * (x - forceVector.at(i).at(1)));
    }
}

double computeMx(vector <vector<double>> forceVector, double x, int i) {
    if (forceVector.at(i).at(2) == 0) {
        return (forceVector.at(i).at(0) * (x - forceVector.at(i).at(1)));
    }
    else if (forceVector.at(i).at(2) <= x) {
        return ((forceVector.at(i).at(0) * (forceVector.at(i).at(2) - forceVector.at(i).at(1))) * (x - (forceVector.at(i).at(1) + ((forceVector.at(i).at(2) - forceVector.at(i).at(1)) / 2))));
    }
    else {
        return ((forceVector.at(i).at(0) * (x - forceVector.at(i).at(1))) * ((x - forceVector.at(i).at(1)) / 2));
    }
}

double computeVx_max(double Vx, double Vx_max) {
    if (abs(Vx) > abs(Vx_max)) {
        return Vx;
    }
    else {
        return Vx_max;
    }
}

double computeMx_max(double Mx, double Mx_max) {
    if (abs(Mx) > abs(Mx_max)) {
        return Mx;
    }
    else {
        return Mx_max;
    }
}

void outputSFBM(vector <vector<double>> SFBM_2D) {
    for (int i{}; i < SFBM_2D.size(); i++) {
        cout << "Distance (m): " << SFBM_2D.at(i).at(0) << " Shear Force (N): " << SFBM_2D.at(i).at(1) << " Bending Moment (Nm): " << SFBM_2D.at(i).at(2) << endl;
    }
    cout << "================================================" << endl;
}

void outputMax(double Vx_max, double Mx_max) {
    cout << "Vx max (N) = " << Vx_max << endl;
    cout << "Mx max (Nm) = " << Mx_max << endl;
    cout << "================================================" << endl;
}

void programExit() {
    cout << "Enter any character to exit: ";
    char i{};
    cin >> i;
}