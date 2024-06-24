#pragma once

#include <ParabolaMath.h>



//A viewpoint from which the scene will be rendered. 
//Cameras maintain their own aspect ratios, field of view, and frustum,
//and project co-ordinates into a space measured from -1 to 1 in x and y, and 0 to 1 in z.
//At render time, the camera will be rendering to a ''Viewport'' 
//which will translate these parametric co-ordinates into real screen co-ordinates. 
//Obviously it is advisable that the viewport has the same aspect ratio
//as the camera to avoid distortion 

enum Projection_Type
{
	ORTHO_GRAPHIC, PERS_PECTIVE
};

enum Camera_Movement {
	FOR_WARD,
	BACK_WARD,
	LE_FT,
	RI_GHT
};


class Camera
{
public:

	Matrix4f View;
	Matrix4f Projection;

	Camera();
	~Camera();

	//Matrix4f LookAt(const Vector4f & Eye, const Vector4f & At, const Vector4f & Up = Vector4f(0.0f, 1.0f, 0.0f));
	void LookAt(const Vector4f & Eye, const Vector4f & At,const Vector4f & Up = Vector4f(0.0f, 1.0f, 0.0f)); 
	
	void SetProjection(Projection_Type Type);
	
	/*Matrix4f Perspective(const float & FieldOfView, const float & AspectRatio,
		const float & ZNear = 0.1f, const float & ZFar = 100.0f);*/
	
	void Perspective(const float & FieldOfView, const float & AspectRatio,
		const float & ZNear = 0.1f, const float & ZFar = 100.0f);
	
	//Matrix4f Ortho(
	//	const float & Left, const float &  Right,
	//	const float &  Top, const float &  Bottom,
	//	const float & ZNear = 0.1f, const float & ZFar = 100.0f
	//);

	void Ortho(
		const float & Left, const float &  Right,
		const float &  Top, const float &  Bottom,
		const float & ZNear = 0.1f, const float & ZFar = 100.0f
	);

	Matrix4f GetViewMatrix();

	void UpdateCameraVectors();

	void SetDefaults();

public:
	// Camera Attributes
	Vector3f Position;
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


	void ProcessKeyboard(Camera_Movement Direction, float DeltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, uint8 constrainPitch = true);

	void ProcessMouseScroll(float yoffset);
private:
	
	void updateCameraVectors();

	

};



	


//void perspective(float FieldView, float ProjectionWindow, float ZNear, float ZFar);
/*
The perspective projection tranformation will require us to supply 4 parameters:

	The aspect ratio - the ratio between the width and the height of the rectangular area which will be the target of projection.
	The vertical field of view: the vertical angle of the camera through which we are looking at the world.
	The location of the near Z plane. This allows us to clip objects that are too close to the camera.
	The location of the far Z plane. This allows us to clip objects that are too distant from the camera.

*/


//SceneManager * 	getSceneManager(void) const
//Returns a pointer to the SceneManager this camera is rendering through.

//virtual bool 	getUseRenderingDistance(void) const
//Get whether this camera should use the 'rendering distance' on objects to exclude distant objects from the final image.

