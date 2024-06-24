#include <Components/NodeSystem/SceneNode.h>


SceneNode::SceneNode()
{
	Position = Vector3f::ZERO;
	Rotation = Vector3f::ZERO;
	Scale = Vector3f::ONE;
}

SceneNode::SceneNode(const char* NodeName) : Node(NodeName, true)
{
	Position = Vector3f::ZERO;
	Rotation = Vector3f::ZERO;
	Scale = Vector3f::ONE;

}

SceneNode::~SceneNode()
{
}

Vector3f SceneNode::GetPosition()
{
	return this->Position;
}

Vector3f SceneNode::GetRotation()
{
	return Rotation;
}

void SceneNode::SetRotation(Vector3f inRotation)
{
	//TODO: nope. Fix dis
	Rotation = inRotation;
}

void SceneNode::Rotate(Vector3f Rotation)
{
	//TODO: nope. Fix dis
	Rotation = Rotation;
}

void SceneNode::resetOrientation()
{
	//TODO: resetOrientation
}

Vector3f SceneNode::GetScale()
{
	//TODO: do something for uniform scaling? Maybe return a float?
	return Scale;

}

void SceneNode::SetScale(Vector3f inScale)
{
	Scale = inScale;
}

void SceneNode::Scaling(Vector3f Scaling)
{
	Scale = Scaling;
}
