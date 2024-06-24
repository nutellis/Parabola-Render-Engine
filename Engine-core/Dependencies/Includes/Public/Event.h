#pragma once
#ifndef ACTOR_H
#define ACTOR_H

#include<Pawn.h>

class Actor : public Pawn 
{
public:
	Actor();

	std::vector<Actor *> Children;

	StaticMeshComponent *StaticMesh;
	PointLightComponent * Light;
	CameraComponent * Camera;

	void ObjectS();
	void AddCamera();
	void AddLight();
private:
	//std::vector<Component *> Components;
};


#endif // !_ACTOR_H
