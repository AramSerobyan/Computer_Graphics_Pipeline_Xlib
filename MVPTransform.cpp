#include <MVPTransform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include  <math.h>
#define PI 3.14159265

MVPTransform::MVPTransform()
{
	ModelMatrix = glm::mat4(1.0);
	ViewMatrix  = glm::mat4(1.0);
	ProjectionMatrix = glm::mat4(1.0);
	MVP  = glm::mat4(1.0);
}
void MVPTransform::Translate(float x, float y, float z)
{
		//	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x, y,z));
	ModelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(x, y,z))*ModelMatrix;

}
void MVPTransform::RotateX(float Deg)
{	
	Rotate(Deg, glm::vec3(1.0f,0.0f,0.0f));
}
void MVPTransform::RotateY(float Deg)
{	
	Rotate(Deg, glm::vec3(0.0f,1.0f,0.0f));
}
void MVPTransform::RotateZ(float Deg)
{	
	Rotate(Deg, glm::vec3(0.0f,0.0f,1.0f));
}
void MVPTransform::Rotate(float Deg, glm::vec3 rotationAxis)
{
	//ModelView = glm::rotate(ModelView, Deg, rotationAxis);
	ModelMatrix = glm::rotate(glm::mat4(1.0f), (float)(Deg*PI/180), rotationAxis)*ModelMatrix;
}
void MVPTransform::Scale(float x, float y, float z)
{
	ModelMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(x,y,z))*ModelMatrix;
}
void MVPTransform::LookAt(float xEye,float yEye,float zEye,  float xAt, float yAt, float zAt, float xUp, float yUp, float zUp)
{
	ViewMatrix = glm::lookAt(glm::vec3(xEye,yEye,zEye),glm::vec3(xAt,yAt,zAt),glm::vec3(xUp,yUp,zUp));
	ModelViewMatrix = ViewMatrix*ModelMatrix;
}
void MVPTransform::LoadIdentity()
{
	ModelMatrix = glm::mat4(1.0f);
}
void MVPTransform::GenerateMVP()
{
	//MVP = ProjectionMatrix*glm::transpose(ViewMatrix)*glm::transpose(ModelMatrix);
	//MVP = glm::transpose(ProjectionMatrix)*ViewMatrix*ModelMatrix;
	MVP = ProjectionMatrix*ViewMatrix*ModelMatrix;
}
void MVPTransform::createFrustum(float left, float right, float bottom, float top, float near, float far)
{
	// construct projection matrix
	float a = (right + left) / (right - left);
    float b = (top + bottom) / (top - bottom);
    float c = -(far + near) / ( far - near);
    float d = -(2.0f * far * near) / (far - near);
    float x = (2.0f * near) / (right - left);
    float y = (2.0f * near) / (top - bottom);

    ProjectionMatrix[0][0] = x;    ProjectionMatrix[0][1] = 0.0f; ProjectionMatrix[0][2] = a;     ProjectionMatrix[0][3] = 0.0f;
    ProjectionMatrix[1][0] = 0.0f; ProjectionMatrix[1][1] = y;    ProjectionMatrix[1][2] = b;     ProjectionMatrix[1][3] = 0.0f;
    ProjectionMatrix[2][0] = 0.0f; ProjectionMatrix[2][1] = 0.0f; ProjectionMatrix[2][2] = c;     ProjectionMatrix[2][3] = d;
    ProjectionMatrix[3][0] = 0.0f; ProjectionMatrix[3][1] = 0.0f; ProjectionMatrix[3][2] = -1.0f; ProjectionMatrix[3][3] = 0.0f;

    ProjectionMatrix  = glm::frustum(left,right,bottom,top,near,far);
    GenerateMVP();
}
void MVPTransform::createFrustum(float fovy, float aspect, float near, float far)
{
	float tangent = tanf(fovy/2 * PI/180);  // tangent/2 of fovY
    float height = near * tangent;          // height/2 of np
    float width =  far * aspect;            // width/2 of np
	createFrustum(-width, width, -height, height, near, far);

	GenerateMVP();
}
void MVPTransform::createOrthoFrustum(float left, float right, float bottom, float top, float near, float far)
{

	float x = (2.0f) / (right - left);
    float y = (2.0f) / (top - bottom);
    float z = (-2.0f)/ (far - near);
	float a = -(right + left) / (right - left);
    float b = -(top + bottom) / (top - bottom);
    float c = -(far + near) / ( far - near);

    ProjectionMatrix[0][0] = x;    ProjectionMatrix[0][1] = 0.0f; ProjectionMatrix[0][2] = 0.0f;     ProjectionMatrix[0][3] = a;
    ProjectionMatrix[1][0] = 0.0f; ProjectionMatrix[1][1] = y;    ProjectionMatrix[1][2] = 0.0f;     ProjectionMatrix[1][3] = b;
    ProjectionMatrix[2][0] = 0.0f; ProjectionMatrix[2][1] = 0.0f; ProjectionMatrix[2][2] = z;     ProjectionMatrix[2][3] = c;
    ProjectionMatrix[3][0] = 0.0f; ProjectionMatrix[3][1] = 0.0f; ProjectionMatrix[3][2] = 0.0f; ProjectionMatrix[3][3] = 0.0f;

    GenerateMVP();
}

glm::vec4 MVPTransform::VectorTransform( glm::vec4 VecToTrans)
{
	return MVP*VecToTrans;
}
glm::vec4 MVPTransform::VectorToWorld( glm::vec4 VecToTrans)
{
	return ModelMatrix*VecToTrans;
}
glm::vec4 MVPTransform::VectorToEye( glm::vec4 VecToTrans)
{
	return ViewMatrix*VecToTrans;
}
glm::vec4 MVPTransform::VectorToClip( glm::vec4 VecToTrans)
{
	return ProjectionMatrix*VecToTrans;
}


void MVPTransform::print()
{
	printf("Model Matrix: \n");
	print(ModelMatrix);
	printf("View Matrix: \n");
	print(ViewMatrix);
	printf("Projection Matrix: \n");
	print(ProjectionMatrix);
	printf("MVP: \n");
	print(MVP);
}


void MVPTransform::print( glm::mat4 matrixToPrint)
{
	for(int i = 0; i <4;i++)
	{
		for(int j =0; j < 4; j++)
			printf("%f ",matrixToPrint[i][j]);
		printf("\n");
	}
		printf("\n");
}