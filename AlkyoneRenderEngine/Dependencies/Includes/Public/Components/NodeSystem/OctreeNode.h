//#pragma once
//
//#include<Components/NodeSystem/SceneNode.h>
//
//class Octree;
//
//
//class OctreeNode : public SceneNode
//{
//public:
//	OctreeNode();
//	~OctreeNode();
//
//	/** Sets the Octree in which this OctreeNode resides*/
//	void SetOctant(Octree *inOctant);
//
//	AABB & GetLocalAABB();
//
//
//protected:
//	// Local bounding box
//	AABB LocalBoundingBox;
//
//	//Octree this node is attached to.
//	Octree *Octant;
//
//
//	// Inherited via SceneNode
//	virtual void Init() override;
//
//	virtual void Reset() override;
//
//};