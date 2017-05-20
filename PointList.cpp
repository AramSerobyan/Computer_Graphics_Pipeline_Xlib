#include <PointList.h>
#include <stdio.h>
// Constructor. Initializes head and length
PointList::PointList()
{
	head = NULL;
	tail = NULL;
	length = 0;
}
// prints list in the form [(x,y,z)...]
void PointList::printList() {
   point3D *ptr = head;
   printf("\n[ ");
   //start from the beginning
   while(ptr != NULL) {
      printf("(%f,%f,%f,%f)",ptr->v[0],ptr->v[1],ptr->v[2],ptr->v[3]);
      ptr = ptr->next;
   }
   printf(" ]\n");
}
// return pointers to the nth item. counting from 1.
point3D* PointList::getPoint(int n)
{
	if(n <= 1)
		return head;
	else if(n == length)
		return tail;
	else
	{
		point3D* current = head;
		for(int i = 0;i<n-1;i++)
			current = current->next;

		return current;
	}
}

// Removes point in a specified positon. N = 1 will remove the head. O(n).
void PointList::RemovePoint(int n)
{
	if(n>=length) return;

	if(head !=NULL)
	{
   		point3D* current = head;
   		if(n==1)
   		{
   			head = head->next;
			free(current);	
			return;
   		}
   		else
   		{
   			point3D* previous;
   			for(int i=0; i<n-1; i++)	
   			{	
   				previous = current;
   				current = current->next;
   			}

   			if(current == tail) tail = previous;
    		previous->next = current->next;
			free(current);	
		}
		length--;
    return;
	}
}

//removes the next point. Takes a pointer to the previous location. O(1).
void PointList::RemovePoint(point3D* previous)
{
	if(previous->next == NULL) return;

	point3D* tmp = previous->next;
	if(tmp == tail) tail = previous;
	previous->next = tmp->next;
	free(tmp); 
	length--;
	return;
}

//inserts a point in the end. Also initializes head and tail if its the first point.
void PointList::AddPoint(float x, float y, float z, float w)
{
	point3D* tmp  = ( point3D *) malloc(sizeof(point3D));
		tmp->v[0] = x; tmp->v[1] =y; tmp->v[2] = z;tmp->v[3] = w;
	if(head ==NULL)
	{
		
		head = tmp;
		tail = head;
		head->next =NULL;
	}
	else
	{

		tail->next = tmp;
		tail= tail->next;
		//if(tail==head) tail = head->next;
		tail->next = NULL;
	}	
	length++;
	return;
}
void PointList::AddPoint(float x, float y, float z)
{
	AddPoint(x,y,z,1.0f);
}

void PointList::AddPoint(point3D* pt)
{
	AddPoint(pt->v[0],pt->v[1],pt->v[2],pt->v[3]);
}

void PointList::AddPoint(glm::vec4 v)
{
	AddPoint(v[0],v[1],v[2],v[3]);
}

void PointList::AddPoint(float x, float y)
{
	AddPoint(x,y,0.0f,1.0f);
	return;
}
// adds point in an arbitrary position. If position is negative it adds in the beginnin. If its larger than length it adds in the end.
void PointList::AddPoint(float x, float y, float z, float w, int n)
{
	if(head==NULL) 
	{
		 AddPoint(x,y,z); 
		 return;
	}
		
		point3D* tmp  = (point3D *) malloc(sizeof(point3D));
				tmp->v[0] = x; tmp->v[1] =y; tmp->v[2] = z;tmp->v[3] = w;
	if(n >= length+1)
	{
		tmp->next =NULL;
		tail->next = tmp;
		tail = tmp;
	} 
	else if(n<=1)
	{
		tmp->next= head;
		head= tmp;
	}
	else
	{
		point3D* current=head;
		for(int i =0;i<n-2;i++)
			current = current->next;

		tmp->next = current->next;
		current->next = tmp;
	}
	length++;
	return;
}
// Destroys the list by freeing occupied memory. 


float* PointList::toArray()
{
	float* arr;
	arr = new float[length*3];
	point3D* tmp=head;
	for(int i = 0;i<length*3;i+=3)
		{
			arr[i] = tmp->v[0]; arr[i+1] = tmp->v[1]; arr[i+2] = tmp->v[2];
			tmp = tmp->next;
		}

		
		return arr;

}


void PointList::duplicateList(PointList* newList)
{
	if(this->head==NULL) return;
	newList->deleteList();
	point3D* tmp = head; 

	while(tmp != NULL)
		{
			newList->AddPoint(tmp->v);
			tmp = tmp->next;
		}
	return;
}

PointList::~PointList()
{
	point3D* current;
	tail == NULL;
	while(head!=NULL)
	{
		current=head;
		head=head->next;
		free(current);    
	}
	
	printf(" List destructed \n");
}

// In case we will need to delete list and insert points a new without destroying the object itself. Does same as destructor.
void PointList::deleteList()
{
	if(head==NULL) return;

	point3D* current;
	while(head!=NULL)
	{
		current=head;
		head=head->next;
		free(current);    
	}
	tail == NULL;
	head == NULL;
	length =0;
	printf(" List deleted \n");
	return;
}