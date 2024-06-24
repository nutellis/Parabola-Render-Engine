#pragma once

#include <Core/PODTypes.h>

#include <ParabolaMath.h>

#include <Utilities/Containers/Array.h>

#include <Components/NodeSystem/OctreeNode.h>

/*
This is a loose octree implementation, meaning that each
octant child of the octree actually overlaps it's siblings by a factor
of .5.  This guarantees that any thing that is half the size of the parent will
fit completely into a child, with no splitting necessary.
*/


enum OctreeIndex
{
	TopLeftFront,
	TopRightFront,
	BottomRightFront,
	BottomLeftFront,
	TopLeftBack,
	TopRightBack,
	BottomRightBack,
	BottomLeftBack
};


class Octree
{
public:

	Octree();
	Octree(Octree * inParent);

	~Octree();

	/*This is called by the OctreeSceneManager after
		it has determined the correct Octree to insert the node into.
	*/
	void AddNode(OctreeNode *NewNode);
	void RemoveNode(OctreeNode *);

	int32 NumberOfNodes();

	uint8 isLeafNode() const;

	uint8 isRootNode() const;


	uint8 isTwiceSize(const AABB & inBox) const;


	int32 GetChildrenIndex(const AABB &inBox);


public:
	AxisAlignedBoundingBox Box;
	// WireBoundingBox* mWireBoundingBox;

	   /** Vector containing the dimensions of this octree / 2
	*/
	Point  HalfSize;

	TArray<Octree *> Children; //TODO : maybe use vector.


	TArray<OctreeNode *> NodeList;


	Octree * Parent;

	uint32 VisibleObjects;

	int32 NodesCount;

private:
	int32 Depth; //Number of subdivisions of the cubes.


	inline void IncrementCount();

	inline void DecrementCount();

};