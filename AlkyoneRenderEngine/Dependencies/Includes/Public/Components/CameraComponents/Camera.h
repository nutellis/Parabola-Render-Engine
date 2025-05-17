#pragma once

#include <ParabolaMath.h>
#include <Components/Components.h>
#include <Components/RenderActor.h>


class PCameraComponent;

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


struct PFrustrum {
public:
	TArray<Vector3f> Corners;
	Vector3f Center;
	Vector3f Min;
	Vector3f Max;
	Vector3f Extents;
	float DiagonalLength;
	float NearPlane;
	float FarPlane;
	float Ratio;
	float FieldOfView;

	PFrustrum(); 
	~PFrustrum();
	void CalculateFrustrumCorners(PCameraComponent* Camera);

	void SetupDebugFrustrumEdges(Vector3f LightPosition);

	//void SetupDebugFrustrumEdges();

	void SetupDebugFrustrumPlanes();

	void RenderDebugFrustrum(int RenderOption, Vector4f Colour, Matrix4f Projection, Matrix4f View, Vector3f LightPosition);

	//void RenderDebugFrustrum(int RenderOption, Vector4f Colour, Matrix4f Projection, Matrix4f View);

private:
	PStaticMeshComponent * DebugFrustrumMesh;
};

// Default camera values			//Load them in some way ?? -> .ini file probably
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVTY = 0.2f;
const float ZOOM = 60.0f;

class PCameraComponent : public PSceneComponent
{
public:

	Matrix4f View;
	Matrix4f Projection;
	PFrustrum Frustrum;


	// Camera Attributes
	Vector3f Front;
	Vector3f Up;
	Vector3f Right;
	Vector3f WorldUp;


	// Euler Angles
	float Yaw;
	float Pitch;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;

	//degrees
	float Zoom;

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

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	void SetupShaderCamera(Shader* ActiveShader);

};



class PCameraActor : public PRenderActor
{
public:
	PCameraComponent* Camera;

public:
	PCameraActor();
	PCameraActor(std::string NodeName);

	void ControlCamera(uint32 Width, uint32 Height);

	PCameraActor(PRenderActor* Parent);
	~PCameraActor();
	//void SetRotation(Vector3f InRotation);

	void SetupDebugFrustrum();
	void RenderDebugFrustrum(Matrix4f Projection, Matrix4f View);
};