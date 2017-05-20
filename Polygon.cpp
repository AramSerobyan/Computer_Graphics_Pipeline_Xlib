// #include <stdio.h>
// #include <stdlib.h>
#include <Polygon.h>

// Constructs Pointlist from Array. Sz %3 = 0 MUST.
Polygon::Polygon(int sz, float*a)
{
	for(int i= 0; i<sz;i+=3)
		p.AddPoint(a[i],a[i+1],a[i+2]);
}

Polygon::Polygon(const PointList list)
{
	point3D* a;
	a = list.getHead();
			
	while(a!=NULL)
		{
			p.AddPoint(a->v);
			a = a->next;
		}

}

Polygon::~Polygon()
{
	p.deleteList();
}

void Polygon::printPoints()
{	
	p.printList();
}

void Polygon::draw(MVPTransform mtr)
{
	transform(mtr); // multiplies all vectors with MVP
	clip();			// clips points
	PerspectiveDivide();
	printPoints();
	//scanConvert(p); // draws points

}


//Uses a clipping algorithm to save only points inside the frustrum
void Polygon::transform(MVPTransform mtr)
{
	point3D* tmp = p.getHead();
	for(int i =0; i<p.getLength();i++)
	{
		glm::vec4 vc = mtr.VectorTransform(tmp->v);
		tmp->v[0] = vc[0];tmp->v[1] = vc[1];tmp->v[2] = vc[2];tmp->v[3] = vc[3];
		tmp =tmp->next;
	}
	printPoints();
}
void Polygon::clip()
{
	PointList newList;
	for(int plane = 0; plane<6; plane++)
	{
		newList.deleteList();
		point3D* current = p.getHead();
		point3D* next = current->next;

		for(int i =0; i<p.getLength();i++)
		{
			if(i==p.getLength()-1) next = p.getHead();

			if(isInside(current->v,plane) && isInside(next->v,plane))
			{
				newList.AddPoint(next->v);
			}
			else if(isInside(current->v,plane) && !isInside(next->v,plane))
			{
				newList.AddPoint(Intersection(current->v,next->v,plane));
			}
			else if(!isInside(current->v,plane) && isInside(next->v,plane))
			{
				newList.AddPoint(Intersection(current->v,next->v,plane));
				newList.AddPoint(next->v);
			}

			current = next;
			next = next->next;
		}

		newList.duplicateList(&p);
	 	p.printList();
}

	//p.printList();
}
bool Polygon::isInside(glm::vec4 vector, int plane)
{
	if(plane<3) return vector[plane]<=vector[3]; 
	else		return vector[plane%3]>=-vector[3];
	//return vector[plane%3]*sign >vector[3]*sign;
}

glm::vec4 Polygon::Intersection(glm::vec4 v1,glm::vec4 v2,int plane)
{
	int sign;
	if(plane<3) sign= 1;
	else		sign=-1;

	float t = (v2[plane%3] + (-sign)*v2[3]) / (v2[plane%3] - v1[plane%3] + (-sign)*v2[3] + sign*v2[3]); 

	printf("%f \n", t);
	//return glm::vec4(v1[1],v1[2],v1[0],t);
	 //   return (v2*(1-t)+v1*t);
	      return (v1*(1-t)+v2*t);
}
 
void Polygon::PerspectiveDivide()
{
	point3D* tmp = p.getHead(); 
	PointList newList;

		for(int i =0; i<p.getLength();i++)
		{
			if(tmp!=NULL) // just in case;
				newList.AddPoint(tmp->v[0]/tmp->v[3],tmp->v[1]/tmp->v[3],tmp->v[2]/tmp->v[3]);

			tmp = tmp->next;
		}
		newList.duplicateList(&p);
}


int* Polygon::ToViewport(int xOrigin,int yOrigin,int width, int height)
{
	int* arr;
	arr = new int[p.getLength()*2+1];
	int i = 0;
	point3D* tmp= p.getHead();
		while(tmp != NULL)
		{
		//	printf("still ok %d\n",i);
			arr[i]  = (tmp->v[0]+1)*(width/2) +xOrigin;
			arr[i+1]= (tmp->v[1]+1)*(height/2)+yOrigin;
			i+=2;
			//printf("still ok %d\n",i);
			tmp = tmp->next;
		}
		arr[i] =10000;
		i=0;
		while(arr[i]!=10000)
		{
			printf("%d  ",arr[i]);
			i++;
		}

		return arr;
}
