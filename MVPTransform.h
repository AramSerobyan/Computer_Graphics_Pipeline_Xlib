#include <glm/glm.hpp>

class MVPTransform{
	
	public:
		MVPTransform();
		void Translate(float x, float y, float z);
		void RotateX(float Deg);
		void RotateY(float Deg);
		void RotateZ(float Deg);
		void Rotate(float Deg, glm::vec3 rotationAxis);
		void Scale(float x, float y, float z);
		void LookAt(float xEye,float yEye,float zEye,  float xAt, float yAt, float zAt, float xUp, float yUp, float zUp);
		void createFrustum(float left, float right, float bottom, float top, float near, float far);
		void createFrustum(float fovy, float aspect, float near, float far);
		void createOrthoFrustum(float left, float right, float bottom, float top, float near, float far);
		void LoadIdentity();
		void GenerateMVP();
		void print();
		glm::vec4 VectorTransform( glm::vec4 VecToTrans);
		glm::vec4 VectorToWorld( glm::vec4 VecToTrans);
		glm::vec4 VectorToEye( glm::vec4 VecToTrans);
		glm::vec4 VectorToClip(glm::vec4 VecToTrans);

	private:
		void print(glm::mat4 matrixToPrint);
		glm::mat4 ModelViewMatrix;	
		glm::mat4 ProjectionMatrix;
		glm::mat4 ModelMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 MVP;
};