#pragma once
#ifndef RENDER_ACTOR_H
#define RENDER_ACTOR_H

#include <Core/RenderCore.h>
#include <Components/Components.h>

#include <Utilities/Containers/Array.h>
#include "VertexBuffers.h"


class PStaticMeshComponent;
class PDirectionalLightComponent;
class PCameraComponent;

enum EntityType {
	LIGHT,
	CAMERA,
	MODEL,
	SKYBOX
};

class RRenderActor {

public:
	RRenderActor();
	RRenderActor(std::string NodeName);

	// std::vector<RRenderActor*> Children; // do i need this? Find out how to access children

	//PSceneComponent* RootComponent; // not sure if this is needed

	void AddLight();

	void AddMesh(const char* path);

	void AddChild(RRenderActor* Child);

	void RemoveChild(RRenderActor* Child);

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
	void AddRotation(Vector3f inRotation);
	//You can move the object relative to its current position by using the translate method.

	//Reset all rotations done to the object.
	void resetOrientation();

	//SetOrientation

	//GetOrientation

	Vector3f GetScale();

	//TODO : always relative to the parent SceneNode
	void SetScale(Vector3f inScale);

	void SetScale(float inScale);

	void DrawMeshChildren(Shader* ActiveShader);

	void SetupModelMatrix();

	void UpdateWorldBoundingBox();

public:
	std::string ObjectName;

	EntityType ActorType;

	RRenderActor* Parent;
	//children vector
	TArray<RRenderActor*> Children;

	PStaticMeshComponent* StaticMesh;

	PDirectionalLightComponent* Light;

	// Stores the position/translation of the node 
	Vector3f ObjectPosition;

	// Stores the rotation of the node .
	Vector3f ObjectRotation;
	//Quaternion Orientation;

	// Stores the uniform scaling factor applied to this node
	Vector3f ObjectScale;

	bool IsMovable;
};

class RQuadRenderActor {

public:
	RQuadRenderActor();
	~RQuadRenderActor();
	VertexArray CreateFullScreenQuad();

	void DrawFullScreenQuad();

public:
	VertexArrayObject VAO;
	VertexBufferObject VBO;
	VertexArray Vertices;
};

#endif // !_ACTOR_H
