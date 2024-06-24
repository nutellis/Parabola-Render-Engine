#include <Components/Camera.h>



#include <Core/RenderCore.h>


Camera::Camera()
{
	Position = Vector3f(0.0f, 0.0f, 5.0f);
	Up = Vector3f(0.0f, 1.0f, 0.0f);
	Front = Vector3f(0.0f, 0.0f, -1.0f);

	Yaw = 90.0f;
	Pitch = 0.f;

	MouseSensitivity = 0.2f;

	Zoom = 60.0f;

	UpdateCameraVectors();

	ProcessMouseMovement(0,0);
	
}

Camera::~Camera()
{}

void Camera::LookAt(const Vector4f & Eye, const Vector4f & At,const  Vector4f & Up)
{
	Vector4f F = Normalize(At - Eye);
	Vector4f S = Normalize(F.Cross(Up));
	Vector4f U = S.Cross(F);

	Vector4f T = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4f Result = Matrix4f(S, U, -F, T);

	Matrix4f Translation;
	Translation = Matrix4f::IDENTITY;

	Translation = Translate(-Eye, Matrix4f::IDENTITY);

	//Translation[0][3] = - Eye.X;
	//Translation[1][3] = - Eye.Y;
	//Translation[2][3] = - Eye.Z;

	Result = Result * Translation;

	View =  Result;

}

//void Camera::LookAt(const Vector4f & Eye, const Vector4f & Center, const Vector4f & Up)
//{
//	Vector4f F = Normalize(Center - Eye);
//	Vector4f S = Normalize(F.Cross(Up));
//	Vector4f U = S.Cross(F);
//
//	Vector4f T = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
//
//	Matrix4f Result = Matrix4f(S, U, -F, T);
//
//	//Result = Translate(Vector3f(-Eye), Result);
//
//	View = Result;
//}

void Camera::SetProjection(Projection_Type Type)
{
	if (Type == ORTHO_GRAPHIC)
	{
		//Projection = Ortho();
	}
	else if (Type == PERS_PECTIVE)
	{
		//Projection == Perspective();
	}
}

//Depth buffer precision is affected by the values specified for zNear and zFar.
//The greater the ratio of zFar to zNear is, the less effective the depth buffer will be at distinguishing between surfaces that are near each other.
//zNear must never be set to 0
void Camera::Perspective(
	const float & FieldOfView, const float & AspectRatio,
	const float & ZNear, const float & ZFar)
{
	Matrix4f Result = Matrix4f::ZERO;
	if (ZNear <= 0) {

	}
	else {

		float F = SMath::Cotan(DegreesToRadians(FieldOfView)*0.5f);

		Result[0][0] = F / AspectRatio;
		Result[1][1] = F;
		Result[2][2] = (ZFar + ZNear) / (ZNear - ZFar);
		Result[2][3] = -1;
		Result[3][2] = (2 * ZFar * ZNear) / (ZNear - ZFar);
		
	}
	Projection =  Result;
}

void Camera::Ortho(
	const float & Left, const float &  Right, 
	const float &  Top, const float &  Bottom,
	const float & ZNear, const float & ZFar
	)
{
	Matrix4f Result = Matrix4f::IDENTITY;
	
	Result[0][0] = 2.0f / (Right - Left);
	Result[1][1] = 2.0f / (Top - Bottom );
	Result[2][2] = -2.0f / (ZFar - ZNear);
	
	Result[3][0] = -(Right + Left) / (Right - Left);
	Result[3][1] = -(Top + Bottom) / (Top - Bottom );
	Result[3][2] = -(ZFar + ZNear) / (ZFar - ZNear);
	
	Projection = Result;
}

Matrix4f Camera::GetViewMatrix()
{
	//LookAt(Vector4f(Position), Vector4f(Position + Front), Vector4f(Up));
	return View;
}

void Camera::UpdateCameraVectors()
{
	// update Front, Right and Up Vectors using the updated Euler angles
	Front.X = SMath::Cos(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));
	Front.Y = SMath::Sin(DegreesToRadians(Pitch));
	Front.Z = SMath::Sin(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));
	Front = Normalize(Front);
	Right = Normalize((Front.Cross(Up)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = Normalize((Right.Cross(Front)));
}

void Camera::SetDefaults()
{
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVTY = 0.1f;
	const float FOV = 60.0f;
}

/*
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	Vector3f front;
	front.X = SMath::Cos(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));
	front.Y = SMath::Sin(DegreesToRadians(Pitch));
	front.Z = SMath::Sin(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));
	Front = Normalize(front);
	// Also re-calculate the Right and Up vector
	Right = Normalize((Front.Cross(WorldUp)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = Normalize((Right.Cross(Front)));
}
*/

void Camera::ProcessKeyboard(Camera_Movement Direction, float DeltaTime)
{
	float Velocity = DeltaTime;// MovementSpeed;
	if (Direction == FOR_WARD)
	{
		Position = Position + (Front * Velocity);
		//Position.Print();
	}
	
	if (Direction == BACK_WARD)
		Position = Position - (Front * Velocity);
	if (Direction == LE_FT) 
	{
		Position = Position - (Right * Velocity);
		//Position.Print();
	}
		
	if (Direction == RI_GHT)
	{
		Position = Position + (Right * Velocity);
		//Position.Print();
	}
		
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, uint8 constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;
	float camX;// = SMath::Sin(xoffset);

	float radius = 5;
	camX = SMath::Cos(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));
	
	float camZ;

	camZ = SMath::Sin(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));

		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
		Position = Vector3f(camX, SMath::Sin(DegreesToRadians(Pitch)), camZ) * radius;
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}