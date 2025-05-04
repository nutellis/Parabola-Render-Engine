#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H



#include <Core/RenderCore.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Components/Lights.h>
#include <Utilities/Containers/Array.h>

// #include <ObjectInitializer.h>

class PRenderActor;
class PStaticMesh;
class Shader;

//Component that has Transformation info but cannot be rendered.
class PSceneComponent {

public:
	PRenderActor* Parent;

	PSceneComponent();

	PSceneComponent(PRenderActor* InParent) : Parent(InParent) {}

	bool bAbsoluteLocation;
	bool bAbsoluteRotation;
	bool bAbsoluteScale;

	void SetAbsolute(bool bNewAbsoluteLocation, bool bNewAbsoluteRotation, bool bNewAbsoluteScale);

	Vector3f GetPosition() const;
	Vector3f GetRotation() const;
	Vector3f GetScale() const;
};

//Components that have geometry and can be renderd.
class PPrimitiveComponent : public PSceneComponent {

	// Draw function here ? !

};

//Basic shape components best used for collision detection.
class PShapeComponent : public PPrimitiveComponent {

	// Add Static Mesh member and later create Box, cylinder etc.

};


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum ProjectionType {
	ORTHOGRAPHIC, PERSPECTIVE
};

struct Frustum
{
	Vector4f TopPlane;
	Vector4f BottomPlane;

	Vector4f RightPlane;
	Vector4f LeftPlane;

	Vector4f FarPlane;
	Vector4f NearPlane;
};


// Default camera values			//Load them in some way ?? -> .ini file probably
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.2f;
const float ZOOM = 60.0f;

class PCameraComponent : public PSceneComponent
{
	// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
public:

	Matrix4f View;
	Matrix4f Projection;
	Frustum CameraFrustrum;


	// Camera Attributes
	Vector3f Front;
	Vector3f Up;
	Vector3f Right;
	Vector3f WorldUp;

	Vector3f CameraDirection;
	
	// Euler Angles
	float Yaw;
	float Pitch;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;

	//degrees
	float Zoom;

	float FieldOfView = 60.0f;
	float ZNear = 0.5f;
	float ZFar = 250.0f;
	float AspectRatio = 16.0f/9.0f;

	//set true for active camera
	bool IsActiveCamera;

	// Constructor with vectors
	//PCameraComponent(Vector3f up = Vector3f(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	PCameraComponent(PRenderActor* Parent, Vector3f up = Vector3f(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// Constructor with scalar values
	PCameraComponent(float upX, float upY, float upZ, float yaw, float pitch);

	void SetProjection(ProjectionType Type);

	Matrix4f GetViewMatrix() const;

	Matrix4f GetProjectionMatrix() const;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void UpdateCameraVectors();

	void SetDefaults();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, float deltaTime);

	void RotateCamera(float xoffset, float yoffset);


	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	void SetupShaderCamera(Shader* ActiveShader);

};
#endif // !_COMPONENTS_H
