# ShearForceBendingMomentsCalculator
Calculates Shear Forces and bending moments along a simply supported two-dimensional beam of any length.
The beam is assumed to be in equilibrium. 
Forces acting on the beam can include any number of single point forces and uniformily distributed loads. 
The number of supports is currently limited to two. 
The calculator divides the beam into 100 points and calculates the shear force and bending moment at each point. 

# Context
This is my first project in my learning of C++ and aslo my first repository in my learning of git/github. 
The project aims to automate concepts learnt from my statics module of my first year Aerospace Engineering degree.
Any feedback (C++ or git related) is very welcome.

# User Inputs
The user will be prompted to enter each component in the following order:
- Length of beam - Can be entered as an integer or a float in metres.
- Number of Point Forces
- Point forces - Each point force should be entered one at a time. Two values are required for each. The first is the Force
                 in Newtons and the second is the distance of the force from the left-hand side of the beam in metres.
                 Each value should be separated with a space.
- Number of UDL's
- UDL's - Each UDL should be entered one at a time. Three values are required for each. The first is the distributed force
          entered in Newtons/Metre. The second is the starting distance of the UDL from the left-hand side of the beam in 
          metres. The third is the end distance of the UDL from the left-hand side of the beam in metres. The Third value must
          be larger than the second. Each value should be speperated by a space.
- Distance of Supports - Each distance should be entered one at a time in metres from the left-hand side of the beam.

# Example
This example shows how the shear forces and bending moments can be solved for a simply supported beam with two point loads and two
 uniformily distributed loads. (NOTE: The UDL's are inputted in Newtons, NOT Kilo Newtons).

![Beam Word art](https://user-images.githubusercontent.com/69728629/93761009-f5ac4d00-fc04-11ea-8ec9-070e19e3e62f.PNG)

This example is inputted as follows:

![Input example](https://user-images.githubusercontent.com/69728629/94932275-c3ca9e80-04c0-11eb-8900-92d275d1d51a.PNG)

The Reaction forces and force summary outputs look like;

![Example Reactions and forces](https://user-images.githubusercontent.com/69728629/94932387-e8267b00-04c0-11eb-9261-ea3dbe265c85.PNG)

The programme with calculate the shear forces and bending moments at 1000 points on the beam. While it is too long to include the entire calculation, here is
a short snippet:

![Example Calculations](https://user-images.githubusercontent.com/69728629/94932593-320f6100-04c1-11eb-82eb-fe4fecfcce15.PNG)

The Maximum shear force and bending moment acting on the beam is outputted as:

![Example Max](https://user-images.githubusercontent.com/69728629/94932630-3fc4e680-04c1-11eb-9e5f-7579f77f9b90.PNG)

# Planned features
- Allow the user to input any number of supports.
- Produce SF and BM graphs for easy visualisation.
- Add Triangular distributed load functionality 