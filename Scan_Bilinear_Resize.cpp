#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <math.h>
#include <time.h>
//#include <Triangle.h>
#include <Sphere.h>
// Edge Data Structure
typedef struct node{
		int minx,miny,maxx,maxy,dx,dy;
		int sminx,sminy,smaxx,smaxy; 
		int inc, c;
		//float m; float b; float c; // line representation as by=mx+c
		struct node* next;
}edge;


void insertEdge(int x,int y, int x2,int y2);
void Scan();
void deleteEdgeTable();
void deleteActiveTable();
void printList();
void printListA();
void deleteFirstEdge();
void CalculateX();
void storeEdge(edge* e);
void deleteEdgeTableR();
void sleep(int n){ for(int i =0;i<n;i++){i=i;} }
void CreateTriangle();
bool isNUll(edge* head){return head==NULL;}
//some variables for usage later in functions

#define MIN_WIDTH  640
#define MIN_HEIGHT 480
#define MAX_WIDTH  (MIN_WIDTH*2)
#define MAX_HEIGHT (MIN_HEIGHT*2)
#define ASPECT_X   4
#define ASPECT_Y   3
clock_t start;
double cpu_time_used;
edge *headR     =NULL; // for redraw
edge *headB     =NULL; // for sorting list
edge *headA     =NULL; // second container
edge *head       =NULL;
edge *current    =NULL;
edge *previous   =NULL;
char WINDOW_NAME[] = "Graphics Window";
char ICON_NAME[]   = "Icon";
Display *display;
int screen;
Window main_window;
Colormap cmap;
GC gc;
Visual *vi;
//XSetWindowAttributes swa;
XGCValues gcValues;
XSizeHints size_hints;
    int          x_return;
    int          y_return;
    unsigned int width_return = 25;
    unsigned int height_return = 25;
    unsigned int border_width_return;
    unsigned int depth_return;
    unsigned int width_new;
    unsigned int height_new;
    Window root_return;
int r,g,b, screne;				
XColor colour;			
bool trueColor = true;	
int bBipolar = 1;
// Vars for Bilinear Interpolation
float areaT = 1.0; // Triangles area
int topY,topX,leftX,leftY,rightX,rightY,scaleLeft,scaleRight;
bool bVerticesSet = true;
unsigned long foreground, background;
// initialized to -1 so we recognize if it was already initialized and initialize the last_x/yetG
	int first_x=-1,first_y=-1,last_x,last_y,start_x,start_y;
	unsigned int oldwidth=640,oldheight=480;
//float Shoelace( int aX,int aY, int bX,int bY, int cX, int cY) __attribute__((always inline));
//	inline float Shoelace( int aX,int aY, int bX,int bY, int cX, int cY);
// Establish the connection;
void connectX()
{
   display = XOpenDisplay(NULL);
   if (display == NULL) {fprintf(stderr, "Cannot connect to X\n");
                         exit(1);}
   screen = DefaultScreen(display);
   foreground = BlackPixel(display, screen);
   background = WhitePixel(display, screen);
}

GC getGC()
{
    GC gc;
    XGCValues gcValues;
    gc = XCreateGC(display, main_window, (unsigned long)0, &gcValues);
    XSetBackground(display, gc, background);
   XSetForeground(display, gc, foreground);
    return (gc);
}

// End the connection after we are done
void disconnectX()
{
   XCloseDisplay(display);
   exit(0);
}

//Setting the appropriate color 
void setColor(int red, int green, int blue){
   if (trueColor){
   			int a = 0x000000;
         	a = ((blue & 0xff) | ((green & 0xff) << 8) | ((red & 0xff) << 16));
         	XSetForeground(display, gc, a);
    }
   else 
  /*For true color do n o t do XallocColor or such */ 
   XSetForeground(display, gc,colour.pixel);
}

	// Draw function. We will not use it, but youcan draw arc , polygon or anything inside
 void draw()
{
   XFlush(display);
}

// the actual redraw;
void Redraw()
{
	edge* link = head; // saving link to head
	head = NULL;
	while(headR!=NULL) //filling head with storage;
	{
		insertEdge(headR->minx,headR->miny,headR->maxx,headR->maxy);
		headR= headR->next;
	}
	Scan();				// running scan convert;
	if(head!= NULL) {printf("trouble, Scan went wrong? \n"); deleteEdgeTable();}
	head = link; 		// pointing head to place where he was pointing before function;
	return;
}

// redraws when configure notify or expose event is called. It handles resizing of the window as well.
void redraw(){
		// We get the geometry of the window
	    XGetGeometry(display, main_window, &root_return, &x_return,&y_return,
                    &width_return, &height_return, &border_width_return, &depth_return);

          width_new =width_return;
          height_new=height_return;
          // We make sure ratio is kept
          if(width_new*ASPECT_Y < height_new*ASPECT_X)
          {
            height_new=(width_new*ASPECT_Y)/ASPECT_X;
          }
          else
          if(width_new*ASPECT_Y > height_new*ASPECT_X)
          {
            width_new=(height_new*ASPECT_X)/ASPECT_Y;
          }

          if((width_new <MIN_WIDTH ) ||
             (height_new<MIN_HEIGHT)
            )
          {
            width_new =MIN_WIDTH;
            height_new=MIN_HEIGHT;
          }

          if((width_new >MAX_WIDTH ) ||
             (height_new>MAX_HEIGHT)
            )
          {
            width_new =MAX_WIDTH;
            height_new=MAX_HEIGHT;
          }

           if(headR!=NULL && headR->next != NULL)
          {
           float difwidth = (float)width_new/(float)oldwidth;
          float difheight = (float)height_new/(float)oldheight;
          		printf("%f %f \n",difwidth,difheight);
          		edge* current = headR;
          		while(current!=NULL)
          		{
          			current->minx = (int)((float)(current->minx)*difwidth);
          			current->maxx = (int)((float)(current->maxx)*difwidth);
          			current->miny = (int)((float)(current->miny)*difheight);
          			current->maxy = (int)((float)(current->maxy)*difheight);
          			current = current->next;
          		}
          /*	headR->minx = (int)((float)(headR->minx)*difwidth);
          	headR->maxx = (int)((float)(headR->maxx)*difwidth);
          	headR->next->minx = (int)((float)(headR->next->minx)*difwidth);
          	headR->next->maxx = (int)((float)(headR->next->maxx)*difwidth);
          	headR->miny = (int)((float)(headR->miny)*difheight);
          	headR->maxy = (int)((float)(headR->maxy)*difheight);
          	headR->next->maxy = (int)((float)(headR->next->maxy)*difheight);*/
          
      		}//*/

       oldwidth= width_new; oldheight = height_new;
		if( width_return != width_new && height_return !=height_new)
		{
       printf("resizing from %4d,%4d to %4d,%4d\n",
                   width_return,
                   height_return,
                   width_new,
                   height_new
                  );
            XResizeWindow(display,main_window,width_new,height_new);
        }
          Redraw();
}

// Draws lines, uses Xlibs function
void drawLines()
{
	edge *ptr = head;
  	while(ptr != NULL)
  	 {
   	  XDrawLine(display,main_window,gc,ptr->minx,ptr->miny,ptr->maxx,ptr->maxy);
     ptr = ptr->next;
  	}							
	XFlush(display);
	return;
}
	// Take coordinates of presset mouFse.
void doButtonPressEvent(XButtonEvent *pEvent)
{	//if the polygon is just created
	if(first_x==-1)
	{
 	 first_x = pEvent->x;
  	 first_y = pEvent->y;
	 start_x = first_x;
	 start_y = first_y;
	 last_x  = first_x;
	 last_y  = first_y;
	}	
	else
	{
	 last_x = pEvent->x;
  	 last_y = pEvent->y;
	 insertEdge(first_x,first_y,last_x,last_y);
	 first_x = last_x;
  	 first_y = last_y;
	}
}

void doMotionNotifyEvent(XMotionEvent *pEvent)
{
  //DO nothing. If wee need motion we can implement
}
// Activates when keyboard is pressed
void doKeyPressEvent(XKeyEvent *pEvent)
{
    int key_buffer_size = 10;
    char key_buffer[9];
    XComposeStatus compose_status;
    KeySym key_sym;
    XLookupString(pEvent, key_buffer, key_buffer_size, &key_sym, &compose_status);
   if 	  (key_buffer[0] == 'q') { deleteEdgeTable();deleteActiveTable(); disconnectX();} //close program
   else if(key_buffer[0] == 'p') { printList();printListA();} 
   else if(key_buffer[0] == 'c'){ insertEdge(start_x,start_y,last_x,last_y); first_x = -1;}
   else if(key_buffer[0] == 'x') { deleteFirstEdge();}	
   else if(key_buffer[0] == 'd') { drawLines();}
   else if(key_buffer[0] == 's') {  start = clock();
   									Scan();
   									cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    								printf("time used in Scan %f \n",cpu_time_used);
    								XFlush(display);
    								return;	}
   else if(key_buffer[0] == 't') {  insertEdge(200,200,200,400); 
   									insertEdge(200,200,450,450);
   									insertEdge(200,400,450,450);
   									//drawLines();
   									Scan();
   									sleep(1);
   									insertEdge(200,200,450,450); 
   									insertEdge(200,200,380,320);
   									insertEdge(380,320,450,450);
   									Scan();
   									//drawLines();
   								}
   else if(key_buffer[0] == 'b') { bBipolar = 1 - bBipolar;} //switch bBiliniear state. 
   else if(key_buffer[0] == 'i') { for(int i= headA->miny;i<headA->maxy;i= i+1) {CalculateX(); printf("%d \n",i);}//testing CalculateX
     	 							headA=NULL;}
}
// Linked List functions. 
//display the list
void printList() {
   edge *ptr = head;
   printf("\n[ ");
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d,%d)",ptr->minx,ptr->miny);
      ptr = ptr->next;
   }
   printf(" ]");
}

void printListA(){
	edge *ptr = headA;
  	 printf("\n[ ");
   	//start from the beginning
 	  while(ptr != NULL) {
 	  	printf("(%d,%d)",ptr->minx,ptr->miny);
  	    //printf("minx %d ",ptr->minx);
   	    ptr = ptr->next;
  	 }
  		 printf(" ]");
  		 return;
} 
int findMaxX() //find maximum X in the scanLine
{
	edge *ptr = headA;
	if(ptr==NULL) {return 1; printf("time to cry");}
	int max = ptr->minx+1;
   	//start from the beginning
 	 	while(ptr!=NULL)
		{
			if(ptr->minx> max )
			{ max=ptr->minx; }
			ptr=ptr->next;
  	 	}
  	return max;
}
int findMinX() //find minimum X in the scanLine
{
	edge *ptr = headA;
	if(ptr==NULL) {return 1; printf("time to cry");}
	int min = ptr->minx;
   	//start from the beginning
 	 	while(ptr!=NULL)
		{
			if(ptr->minx< min )
				{ min=ptr->minx; }
				ptr=ptr->next;	
  	 	}
  	return min;
} 
int findMax()
{
	edge *ptr = head;
	if(ptr==NULL) { return 1; }
	int max= ptr->miny;
   	//start from the beginning
 	 	while(ptr!=NULL)
		{
			if(ptr->maxy> max) { max=ptr->maxy ; }
			ptr=ptr->next;
  	 	}

  	return max;
} 

//Initialize when head==NULL
void insertFirstEdge(int x,int y, int x2, int y2) {
  
  		//edge *e;
		edge *e = (struct node *)malloc(sizeof(edge));
		// find out the minimal point
		if(y<y2)
			{
				e->minx= x;  e->miny=y;
				e->maxx= x2; e->maxy=y2;
			}
		else 
			{
				e->minx= x2; e->miny=y2;
				e->maxx= x;  e->maxy=y;
			}
				e->sminx= e->minx; e->sminy=e->miny;
				e->smaxx= e->maxx; e->smaxy=e->maxy;

				if(e->maxx>e->minx){e->dx = e->maxx-e->minx;}
				else{   e->dx = - e->maxx+ e->minx; }
				if(e->maxy>e->miny){e->dy = e->maxy-e->miny;}//just in case
				else {e->dy = -e->maxy+e->miny;}
				e->inc= e->dx;
				e->c = 0;
				head = e; e->next = NULL;
 			
}
void insertEdge(int x,int y, int x2,int y2)
{	
	if(y==y2){ return; }
	else if(head == NULL){insertFirstEdge(x,y,x2,y2); return;}
	else
	{	
		//edge *e;
		edge *e = (struct node *) malloc(sizeof(edge));
		// find out the minimal point
		if(y<y2)
			{
				e->minx= x; e->miny=y;
				e->maxx= x2; e->maxy=y2;
			}
		else 
			{
				e->minx= x2; e->miny=y2;
				e->maxx= x; e->maxy=y;
			}
				//saving initial coordinates
				e->sminx= e->minx; e->sminy=e->miny;
				e->smaxx= e->maxx; e->smaxy=e->maxy;
				// Find differences and initalize the slope
				if(e->maxx>e->minx){e->dx = e->maxx-e->minx;}
				else{   e->dx = - e->maxx+ e->minx; }
				if(e->maxy>e->miny){e->dy = e->maxy-e->miny;}//just in case
				else {e->dy = -e->maxy+e->miny;}
				e->inc= e->dx;
				e->c = 0;

		if(head->miny > e->miny ||
		 (head->miny==e->miny && head->minx>e->minx))
		{
				e->next = head;
				head = e;
		}
		else	
		{
		 bool iter =true;
		 current  =head->next;
		 previous =head;
		 while(iter)
		 {
		  if(current==NULL)
			{

			 previous->next= e;e->next=NULL;
			 iter =false;
			}
		  else if(current->miny>e->miny || 
			(current->miny==e->miny && current->minx>e->minx))
			{
			 e-> next = current;
			 previous->next = e;
			 iter =false;
			}
		  else if(current->next==NULL)
			{
			 current->next = e;e->next = NULL;
			 iter =false;
			}
		 else  
			{
			 previous = current;
			 current  = current->next;
			}
		 }	
		}
	}
	return;
}
//delete head
void deleteEdgeTable()
{ 
	edge* current;
	while(head!=NULL)
	{
			current=head;
			head=head->next;
			free(current);    
	}
	deleteEdgeTableR();
	return;
}

void deleteEdgeTableR()
{
	while(headR!=NULL)
	{
			current=headR;
			headR=headR->next;
			free(current);
	}

	return;		
}

void deleteActiveTable(){
	edge* currentA;
	while(headA!=NULL)
	{
		currentA=headA;
		headA=headA->next;
		free(currentA);
		    
	}
	return;		
}

void deleteFirstEdge() {
  
	if(head !=NULL){
   current = head;	
   //Next becomes head
   head = head->next;
	free(current);	
	current = NULL;
   
			}
   return ;
}
// Looking for a line with the maximal y coinciding with our scan_line. If we find it then we remove its edge since we already scanned it
void checkY(int current_line)
{
	//printf("%d ",current_line);
	if(headA == NULL) return;
	edge* previousA = headA; edge* currentA = headA->next;
	while(headA->maxy==current_line) 
	{
		headA=currentA;
		storeEdge(previousA); 
		if(headA == NULL) return;
		previousA=headA; currentA = previousA->next;

	}
	while(currentA!= NULL)
	{
		if(currentA->maxy==current_line)
		{
			previousA->next = currentA->next;
			storeEdge(currentA); 
			currentA = previousA->next;
		}
		else
		{
			previousA = currentA; currentA = currentA->next;
		}

	}
	return;
}
// For each scan line we recalculate the X's understanding how scan affected them.
void CalculateX()
{
	edge *ptr = headA;
	while(ptr != NULL) 
 	{	// We use the slope for determining how it moves.
 		while(ptr->c>= ptr->dy && ptr->dy != 0)
 		{
 			 if(ptr->maxx>ptr->minx)
 			{
 				ptr->minx = ptr->minx+1; ptr->c = ptr->c - ptr->dy;
 			}
 			else if(ptr->maxx<ptr->minx)
 			{	
 				ptr->minx = ptr->minx-1; ptr->c = ptr->c - ptr->dy;
 			}	
 		}
 		ptr->c =ptr->c + ptr->inc;
 		ptr = ptr->next;
 	}
 	return;
}

// sorts by mooving
void moveSort()
{
	if(headA==NULL) {return;}
	edge* candidate = headA;
	headA = headA->next;
	if(headB==NULL)
		{
		 headB = candidate; headB->next =NULL;
		}
	else
	{
		edge* currentB;
		edge* previousB;
		
		if(headB->minx>candidate->minx||
			 (headB->minx== candidate->minx && headB->maxx>candidate->maxx))
		{
			candidate->next = headB;
			headB = candidate;
		}
		else	
		{
		 bool iter =true;
		 currentB  =headB->next;
		 previousB =headB;
		 while(iter)
		 {
			if(currentB==NULL)
			{
			 previousB->next= candidate;candidate->next=NULL;
			 iter =false;
			}
			else if(currentB->minx>candidate->minx ||
			 (currentB->minx== candidate->minx && currentB->maxx>candidate->maxx))
			{
			 candidate-> next = currentB;
			 previousB->next = candidate;
			 iter =false;
			}
			else
		 	{
			 previousB = currentB;
			 currentB  = currentB->next;
			}
		 }	
		}
	}
	return;
}
void move()
{
	if(head==NULL) {return;}

	edge* candidate = head;
	head = head->next;

	if(headA==NULL)
	{
		headA = candidate; headA->next =NULL;
	}
	else
	{
		edge* currentA;
		edge* previousA;
		
		if(headA->minx>candidate->minx||
			 (headA->minx== candidate->minx && headA->maxx>candidate->maxx))
		{
			candidate->next = headA;
			headA = candidate;
		}
		else	
		{
		 bool iter =true;
		 currentA  =headA->next;
		 previousA =headA;
		 while(iter)
		 {
			if(currentA==NULL)
			{
			 previousA->next= candidate;candidate->next=NULL;
			 iter =false;
			}
			else if(currentA->minx>candidate->minx ||
			 (currentA->minx== candidate->minx && currentA->maxx>candidate->maxx))
			{
			 candidate-> next = currentA;
			 previousA->next = candidate;
			 iter =false;
			}
			else
		 	{
			 previousA = currentA;
			 currentA  = currentA->next;
			}
		 }	
		}

	}
	return;
}
void storeEdge(edge* e)
{	// since we were changing minx we had to recover them
	e->minx= e->sminx; e->miny=e->sminy;
	e->maxx= e->smaxx; e->maxy=e->smaxy;
	// We save the pointers to all deleted edges. When expose or canfigure not is called we fill back the headA and scan it.
	if(headR==NULL)
	{		
		headR = e; headR->next =NULL;
	}
	else
	{
		e->next=headR; headR=e;
	}
	return;
}

// Returns size of list in case we will need it.
int length()
   {
   int length = 0;
   edge *iter;	
   for(iter = head; iter != NULL; iter = iter->next)
    {
      length = length+1;
    }	
   return length;
}

// Functions for Bilinear interpolation start
float Shoelace( int aX,int aY, int bX,int bY, int cX, int cY) // Triangle Area Calculation
{
	return fabsf( 0.5*((aX-cX)*(bY-aY)-(aX-bX)*(cY-aY)));
	//if(Area==0) { return 1;}
	//else if(Area<0) { return -1.0*Area;}
	//return fabsf(Area);
}
void ColorVertices()  // function for obtaining the crucial vertices of triangle
{	
	if(headA==NULL){ return; } if(headA->next==NULL) {return;}
	topY = headA->miny; topX = headA->minx;
	if(headA->maxx<headA->next->maxx)
	{
		leftY=headA->maxy; leftX=headA->maxx;
		rightY=headA->next->maxy; rightX=headA->next->maxx;
	}
	else
	{
		rightY=headA->maxy; rightX=headA->maxx;
		leftY=headA->next->maxy; leftX=headA->next->maxx;
	}
	areaT = Shoelace( topX, topY, leftX, leftY, rightX, rightY);
	return;
}
// Calculates percentages in the Area of a given color.
float colorPercentage(float Area, float Color)
{
	if(Area==0.0) { return 0.0;}
	return (Color)/(Area);
	//if(prc>1.0|| prc<0.0) { return 0.0;}
	//return prc;
}
// Bilinear interpolation using ratio of triangle areas as percentage of colors in the triangle
void BiInterpol(int current_y, int current_x)
	{	
		

		time_t start_t = clock();
		if(headA==NULL || headA->next==NULL) {return;} 
		if(bVerticesSet) // first time
		{
			ColorVertices();
			bVerticesSet = false;
		}		
//plot
			setColor(((colorPercentage(areaT,(Shoelace(topX,topY,current_x,current_y,rightX,rightY))))*256.0), //red
					((colorPercentage(areaT,(Shoelace(topX,topY,leftX,leftY,current_x,current_y))))*256.0), //green
					((colorPercentage(areaT,(Shoelace(current_x,current_y,leftX,leftY,rightX,rightY))))*256.0)); //blue */
		cpu_time_used = ((double) (clock() - start_t)) / CLOCKS_PER_SEC;
  		// printf("time used in Bilinear %f \n",cpu_time_used);
	XFlush(display);
	return;
	}

// Scanning each line from miny to maxy in our list. 
void scanLine(int begin_x,int end_x,int current_line)
{

	time_t start_t = clock();
	//if(begin_x<0) { printf("Cry out loud %d", begin_x);begin_x =0;}
	int count = 0; // not yet used
	edge* ptr = headA;
	if(ptr==NULL) return;
	//if(begin_x>end_x){ printf("trouble"); return; }
	for(int i = begin_x;i<end_x;i= i+1)
	{
		if(i==ptr->minx)
		{
			count = count + 1;
			ptr=ptr->next;
			if(ptr==NULL) return;// just in case
			if(ptr->minx ==i) { count = count +1; ptr =ptr->next; if(ptr==NULL) {return;} }
		}
		else
		{
			if(count%2 ==1) // assuming we will count the very first edges minx;
			{ 
				if(bBipolar)
				{ 
					BiInterpol(current_line,i); 
				}
					XDrawPoint(display,main_window,gc,i,current_line); 
			}
		}
	}
	cpu_time_used = ((double) (clock() - start_t)) / CLOCKS_PER_SEC;
  //  printf("time used in scan_line %f \n",cpu_time_used);
	XFlush(display);
	return;
}
// Scan convert Algorithm
void Scan()
{	
	//start= clock();
	if(head==NULL) return;
	int current_line = head->miny;
	int begin_x=0, end_x;
	int maxy = findMax();
	while(headA != NULL || head !=NULL)
	{ 
		// Populating ActiveEdgeTable //printf(" %d , %d \n", head->miny,scan_y );
		while(head!=NULL && head->miny == current_line)
		{
			move(); // moves head edge to another list of edges
		}
		 	// Done with table 	
		 	// Next is sorting by x, which our algorithm does when populating the list
		checkY(current_line);
		CalculateX(); //to be implemented(int)
		while(headA!=NULL)
		{ 	
			moveSort();

		}
			if(headA==NULL)
			{
				headA = headB;
				headB = NULL;
			}
		begin_x=findMinX();
		end_x=findMaxX(); // span of our line
		scanLine(begin_x,end_x,current_line); // draws line with checks
	 	current_line = current_line + 1;
	 	if(current_line>1000) headB =NULL;
	 //	printf("der frumem %d %d %d\n",isNUll(head),isNUll(headA),current_line);
	}
		 //cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    // printf("time used in Scan %f \n",cpu_time_used);
	XFlush(display);
	bVerticesSet = true;
	return;
}
int main (int argc, char** argv)
{
  XEvent event;
  connectX();
  int x= 0, y= 20,width = 640,height=480, border_width= 45;

  int black = BlackPixel(display, DefaultScreen(display));
  int white = WhitePixel(display, DefaultScreen(display));
 
  //swa.border_pixel = 0;
  //swa.event_mask = ExposureMask | ButtonPressMask | StructureNotifyMask;
  main_window=XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0,500,500,0, black,black);
  XSetWindowAttributes atts;
  atts.backing_store = Always;
  XChangeWindowAttributes(display, main_window, CWBackingStore, &atts);
   size_hints.flags = PPosition | PAspect | PMinSize | PMaxSize;
   size_hints.x = x;
   size_hints.y = y;
   size_hints.width = width;
   size_hints.height = height;
   size_hints.min_width=640;
   size_hints.min_height=480;
   size_hints.max_height=2*640;
   size_hints.max_width=2*480;
   size_hints.min_aspect.x=4;
   size_hints.max_aspect.x=size_hints.min_aspect.x;
   size_hints.min_aspect.y=3;
   size_hints.max_aspect.y=size_hints.min_aspect.y;
   XSetStandardProperties(display, main_window, WINDOW_NAME, ICON_NAME,
                          None, 0, 0, &size_hints);
   XSelectInput(display, main_window, (ButtonPressMask | KeyPressMask |
                                      ExposureMask |  StructureNotifyMask)); //PointerMotionMask removed we didn't need it
  /*For later*/ 
  XMapWindow(display, main_window);
  XSync(display, False);
  XFlush(display);
  gc = XCreateGC(display, main_window, (unsigned long)0, &gcValues);					/* check when creating the GC.  */
  if (gc < 0){
     fprintf(stderr, "No graphics context...\n");
     exit(0);
  }

   XSetForeground(display, gc, white); 
      cmap = DefaultColormap(display,0);
      
     setColor(100,0,0);

     CreateTriangle();

  while (1){
    XNextEvent(display, &event);
   switch(event.type){
    case ConfigureNotify:
	//printf(stderr, "Config Notify Called\n");
    case Expose:
     // printf("Expose event\n");
      if(XPending(display)<1){
      	redraw();
      }
     // doExposeEvent (&event);
      break;
    case ButtonPress:
      printf("Button Pressed\n");
      doButtonPressEvent((XButtonEvent *)&event);
      break;
    case KeyPress:
     printf("Key pressed\n");
     doKeyPressEvent((XKeyEvent *)&event);
     break;
			// MotionNotify event removed since not used;   
   }
  }   
}   

void CreateTriangle(){

	MVPTransform Wrld;
 	//Wrld.Rotate(90,glm::vec3(0.0,1.0,0.0));
 	//Wrld.Rotate(90,glm::vec3(0.0,0.0,0.0));
 	Wrld.Translate(0.1f,-7.2f,0.5f);
 	Wrld.LookAt(0.0f,0.0f,-1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
 	Wrld.createFrustum(1.0,-1.0,1.0,-1.0,1.0,10.0);
 	Wrld.GenerateMVP();
	PointList list;
	list.AddPoint(6,10,10);
	//list.AddPoint(0.5f,1.0f,5.0f);
	list.AddPoint(1.0f,1.0f,5.0f);
	list.AddPoint(12,2,8);
	float* arr = list.toArray();
	Triangle tri(arr);
	tri.printPoints();
	tri.draw(Wrld);

		    XGetGeometry(display, main_window, &root_return, &x_return,&y_return,
                    &width_return, &height_return, &border_width_return, &depth_return);

		 printf("%d %d %d %d = \n", width_return, height_return, border_width_return, depth_return); 
	//int* arr2 = tri.ToViewport(x_return,y_return,width_return,height_return);
		    int* arr2 = tri.ToViewport(x_return,y_return,width_return,height_return);
			int i=0; 
		while(arr2[i+2]!=10000)
		{
			insertEdge(arr2[i],arr2[i+1],arr2[i+2],arr2[i+3]);
			i+=2;
		}
		    insertEdge(arr2[i],arr2[i+1],arr2[0],arr2[1]);

		    printList();
		    //bBipolar =false;
		  Scan();

	//printf("%d \n",arr2[0]);

	delete arr;
	delete arr2;
	return;
}


//StructureNotifyMask()