#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <Components/NodeSystem/SceneNode.h>

class PObject : public SceneNode {

public:
	PObject();
	PObject(const char* Name);
	
	//virtual void Serialize() {}	

	//virtual void Deserialize() {}

	inline const char* GetName() const { return ObjectName; }
	
	inline void SetName(const char * InObjectName) { ObjectName = InObjectName; }

	//inline uint32 GetHash() { return ObjectName->Hash; }

	void Draw();
};


#endif // !OBJECT_H

