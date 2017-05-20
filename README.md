# Computer_Graphics_Pipeline_Xlib
Implementation of the standard computer Graphics pipeline which is done behind OpenGl. Xlib and C++ were used for that purpose

* ScanConversion
* Barycentric interpolation 
* A basic pipeline was implemented.

- ScanConersion

A basic ScanConversion is implemented. It is responsible for storing the points in the list and upon the necessary command fill the objects bounded with dotes. 

- Barycentric Interpolation

Barycentric interpolation is used for distributing three colors over the triangle. That is if we set the colors in the tips of the triangle to R,G,B. Then the color of the inside of our triangle will consist of the portion of that colors. The distribution of each color depends from the distance between every point and the tips.

- Implemented Pipeline.

The basic OpenGl pipeline is implemented using c++ and Xlib. also Glm was used for matrix operations.
Here is the list of implemented features

* ModelView Transformation
* Projective Transformation
* Clipping with respect of specified Frustum
* Perspective division
* Viewport Mapping

- additional comments

Simple Polygon and Triangle classes exist. They can create a polygon or a triangle from the given points and later perform all the transformation up to the viewport if necessary.
MakeFile exists for creating an executable file in Linux. Also the library directories should be changed to the corresponding directories of Xlib and glm in the computer.


