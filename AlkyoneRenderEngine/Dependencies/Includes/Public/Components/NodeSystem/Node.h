#pragma once

#include <ParabolaMath.h>

#include <Utilities/Containers/Array.h>



class Node
{
public:

	Node();
	Node(const char* Name, bool Active)
		: ObjectName(Name), Active(Active) {}
	~Node();

	//virtual void RemoveAllChildren();
public:
	const char* ObjectName;

	//Node * Parent;

	////Quaternion Orientation;
	//
	//// Stores the position/translation of the node 
	//Vector3f Position;

	//// Stores the rotation of the node .
	//Vector3f Rotation;

	//// Stores the uniform scaling factor applied to this node
	//Vector3f Scale; 

	//
	//

protected:
	bool Active;

};
