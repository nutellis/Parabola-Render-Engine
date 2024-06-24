#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H



#include <Core.h>
#include <RenderEngine/StaticMesh.h>
#include <RenderEngine/Material.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <RenderEngine/Lights.h>

#include <ObjectInitializer.h>
#include <PComponent.generated.h>

class PComponent : public PObject
{
public:
	GENERATED_BODY()

	PComponent();

	UArchive *ComponentArchive;

};

//Component that has Transformation info but cannot be rendered.
class PSceneComponent : public PComponent {

public:
	

	PSceneComponent();

	uint8 bAbsoluteLocation : 1;
	uint8 bAbsoluteRotation : 1;
	uint8 bAbsoluteScale : 1;
	//private:
	glm::vec3 RelativeLocation;
	glm::vec3 RelativeRotation;
	glm::vec3 RelativeScale;

	void SetAbsolute(bool bNewAbsoluteLocation, bool bNewAbsoluteRotation, bool bNewAbsoluteScale);

};

//Components that have geometry and can be renderd.
class PPrimitiveComponent : public PSceneComponent {



};

//Basic shape components best used for collision detection.
class PShapeComponent : public PPrimitiveComponent {


};

//Component that has premade geometry
class PStaticMeshComponent : public PPrimitiveComponent {

public:
	float angle;
	PStaticMeshComponent();
	//	PStaticMeshComponent(const char* path,PSceneComponent *Default,bool isAbsolute);
	inline StaticMesh * GetMesh() const { return Mesh; }
	inline Material * GetMaterial() const { return cMaterial; }

	void SetShader(Shader shader);
	void Deserialize(UArchive &Ar);
	void BindTextures(Shader shader);


private:
	StaticMesh *Mesh;
	Material *cMaterial;
};

class PPointLightComponent : public PSceneComponent
{
public:
	PPointLightComponent();

	PPointLightComponent(PSceneComponent *Default);

	~PPointLightComponent();

	PointLightAttributes *Attributes;																			//EEEEEEEEEEEEEE OPOINTLIGHTATTRIBUTES
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values			//Load them in some way ??
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

class PCameraComponent : public PSceneComponent
{
	// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
public:

	// Camera Attributes
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	PCameraComponent(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// Constructor with scalar values
	PCameraComponent(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	void SetShader(Shader shader);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();


	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);


	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
};
#endif // !_COMPONENTS_H
