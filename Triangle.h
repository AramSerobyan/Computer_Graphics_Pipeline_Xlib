#include <Polygon.h>

class Triangle: public Polygon{
// To be fixed later
public:
	 	Triangle(float* arr) : Polygon(9, arr){};
	 	Triangle(PointList list) : Polygon(9, list.toArray()){};
	 	~Triangle();
	 	//void draw();
private:
		PointList p;


};