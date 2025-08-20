#pragma once

#include <Core/RenderCore.h>

#include <Utilities/Containers/Array.h>

#include <Components/NodeSystem/Node.h>


#include <Components/StaticMesh.h>


//#include <Components/BoundingBox.h>

//#include <map>

//class PObject;


enum TransformSpace
{
	/// Transform is relative to the local space
	TS_LOCAL,
	/// Transform is relative to the space of the parent node
	TS_PARENT,
	/// Transform is relative to world space
	TS_WORLD
};


class SceneNode : public Node
{
public:

	SceneNode();
	SceneNode(const char* NodeName);
	// SceneNode(SceneNode* Parent, const TArray<SceneNode*>& Children) : Node("test", true), Parent(Parent), Children(Children)  {}
	
	/*SceneNode(SceneNode* Parent, const Vector3f& Position, const Vector3f& Rotation, const Vector3f& Scale, const TArray<SceneNode*>& Children, RStaticMesh* Mesh)
		: Parent(Parent), Position(Position), Rotation(Rotation), Scale(Scale), Children(Children), Mesh(Mesh) {}
	*/
	~SceneNode();

	/// Only available internally - notification of parent.
	//virtual void SetParent(SceneNode* parent);

	//SceneNode * GetParent() const;



	//SceneNode * GetChild() const;


	//uint32 GetChildCount() const;


	//virtual void Update(uint8 UpdateChildren);

	//void Init() = 0;


	//virtual void Render() = 0;

	//void Reset() = 0;

	//probably (definitely) we will need an array of sorts to keep ONLY the children that needs updating
//ChildUpdateSet mChildrenToUpdate;

	//virtual const Sphere & GetBoundingSphere() const = 0;


	//get number of visible triangles
	//	virtual uint32 GetNumTriangles() const = 0;

	//AABB GetBoundingBox() const;

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
	void Rotate(Vector3f Rotation);


	//Reset all rotations done to the object.
	void resetOrientation();

	//SetOrientation
		
	//GetOrientation

	Vector3f GetScale();

	//TODO : always relative to the parent SceneNode
	void SetScale(Vector3f inScale);


	//You can move the object relative to its current position by using the translate method.
	void Scaling(Vector3f Scaling);



public:

	// Stores the position/translation of the node 
	Vector3f Position;

	// Stores the rotation of the node .
	Vector3f Rotation;
	//Quaternion Orientation;


	// Stores the uniform scaling factor applied to this node
	Vector3f Scale;
protected:
	//This nodes Bounding Box
	//AABB BoundingBox;
};

