#include <glm/glm.hpp>
#ifndef POINTLIST_H
#define POINTLIST_H

typedef struct nodeP{

	glm::vec4 v;
	struct nodeP* next;

}point3D;

#endif

class PointList{

public:
	PointList();
	~PointList();
	void printList();
	void AddPoint(point3D* pt);
	void AddPoint(glm::vec4 v);
	void AddPoint(float x, float y);
	void AddPoint(float x, float y, float z);
	void AddPoint(float x, float y, float z, float w);
	void AddPoint(float x, float y, float z, float w, int n);
	void RemovePoint(int n);
	void RemovePoint(point3D* previous);
	void deleteList();
	void duplicateList(PointList* newList);
	float* toArray();
	point3D* getHead() const{return head;};
	point3D* getTail() const{return tail;};
	int getLength() const{return length;};
	point3D* getPoint(int n);
	
private:
	point3D* head;
	point3D* tail;
	int length;
};


