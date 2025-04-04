#pragma once
#ifndef RENDER_ACTOR_H
#define RENDER_ACTOR_H

#include <Core/RenderCore.h>
#include <Components/RenderComponents/Components.h>

#include <Utilities/Containers/Array.h>


class PStaticMeshComponent;


enum EntityType {
	LIGHT,
	CAMERA,
	MODEL,
	SKYBOX
};

class PRenderActor {

public:
	PRenderActor();
	PRenderActor(const char* NodeName);

	// std::vector<PRenderActor*> Children; // do i need this? Find out how to access children

	//PSceneComponent* RootComponent; // not sure if this is needed
	
	void AddCamera();

	void AddLight();

	void AddMesh(const char* path);

	void AddChild(PRenderActor* Child);

	void RemoveChild(PRenderActor* Child);

	void RemoveAllChildren();


	// position, rotation, scale

	Vector3f GetPosition();

	//TODO : always relative to the parent SceneNode
	void SetPosition(Vector3f inPosition);


	//You can move the object relative to its current position by using the translate method.
	//void Translate(Vector3f Translation);

	Vector3f GetRotation();

	//TODO : always relative to the parent SceneNode
	//You can use the pitch, yaw, and roll functions to rotate objects.
	void SetRotation(Vector3f inRotation);
	//You can move the object relative to its current position by using the translate method.

	//Reset all rotations done to the object.
	void resetOrientation();

	//SetOrientation

	//GetOrientation

	Vector3f GetScale();

	//TODO : always relative to the parent SceneNode
	void SetScale(Vector3f inScale);

	void SetScale(float inScale);


	//You can move the object relative to its current position by using the translate method.
	void Scaling(Vector3f Scaling);

	void DrawMeshChildren(Shader* ActiveShader);

	void ControlCamera(uint32 Width, uint32 Height);


public:
	std::string ObjectName;

	EntityType ActorType;

	PRenderActor* Parent;
	//children vector
	TArray<PRenderActor*> Children;

	union {
		PStaticMeshComponent* StaticMesh;

		PPointLightComponent* Light;

		PCameraComponent* Camera;
	};

	// Stores the position/translation of the node 
	Vector3f ObjectPosition;

	// Stores the rotation of the node .
	Vector3f ObjectRotation;
	//Quaternion Orientation;

	// Stores the uniform scaling factor applied to this node
	Vector3f ObjectScale;
	
};


#endif // !_ACTOR_H
