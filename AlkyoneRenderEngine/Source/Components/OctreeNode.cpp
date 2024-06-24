#include <Components\NodeSystem\OctreeNode.h>

OctreeNode::OctreeNode() : SceneNode()
{


	//BoundingBox = AABB();
	//BoundingBox.SetExtents(Point(0.3f, 0.3f, 0.3f), Point(0.5f, 0.5f, 0.5f));
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::SetOctant(Octree * inOctant)
{
}

AABB & OctreeNode::GetLocalAABB()
{
	return LocalBoundingBox;
}

void OctreeNode::Init()
{
}

void OctreeNode::Reset()
{
}
