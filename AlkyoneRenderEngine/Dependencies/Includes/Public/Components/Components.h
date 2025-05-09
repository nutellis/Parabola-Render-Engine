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


#endif // !_COMPONENTS_H
