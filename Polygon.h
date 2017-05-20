#include <stdio.h>
#include <stdlib.h>
#include <PointList.h>
#include <MVPTransform.h>
/*To Be Implemented*/
// draw Polygon. Takes points oclips them and draws.
// clip Polygon. Get's rid of the unnecessary parts.

class Polygon{
	
public:
		Polygon(int sz,float* a);
		Polygon(const PointList a);
		~Polygon();
		void printPoints();
		void draw(MVPTransform mtr);
		void clip();
		void transform(MVPTransform mtr);
		void PerspectiveDivide();
		bool isInside(glm::vec4 vector, int plane);
		int getPolygonSize(){return p.getLength();}; // returns amount of points.
		glm::vec4 Intersection(glm::vec4 v1,glm::vec4 v2, int plane);
		PointList getPolygonList(){return p;};	// returns pointer to the array of the points.
		int* ToViewport( int xOrigin,int yOrigin,int width, int height);

private:
	PointList p;

};