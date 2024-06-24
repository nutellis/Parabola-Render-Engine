#include <Components/Octree.h>



Octree::Octree() : Parent(NULL), NodesCount(0)
{
	Box = AABB();

}


Octree::Octree(Octree * inParent) : Parent(inParent), NodesCount(0)
{
	
}

Octree::~Octree()
{
	//delete?
}

void Octree::AddNode(OctreeNode * NewNode)
{

	NodeList.PushBack(NewNode);

	NewNode->SetOctant(this);


	
	//do something with counts and numbers and shiiat.

	
}

uint8 Octree::isLeafNode() const
{
	return Children[0] == NULL;
}

uint8 Octree::isRootNode() const
{
	return Parent == NULL;
}

//Checks if the inBox is at least same in size with HALF of the Octree's bounding box.
uint8 Octree::isTwiceSize(const AABB & inBox) const
{
	Point BoxSize = inBox.GetSize();

	return ((BoxSize.X <= HalfSize.X) && (BoxSize.Y <= HalfSize.Y) && (BoxSize.Z <= HalfSize.Z));
}

int32 Octree::GetChildrenIndex(const AABB & inBox)
{
	int32 Result;

	//many ifs here...
	Point inBoxCenter = inBox.GetCenter();

	Point OctreeCenter = Box.GetCenter();

	if (inBoxCenter.X < OctreeCenter.X)
	{
		if (inBoxCenter.Y < OctreeCenter.Y)
		{
			if (inBoxCenter.Z < OctreeCenter.Z)
			{
				Result = BottomLeftFront;
			}
			else
			{
				Result = BottomLeftBack;
			}
		}
		else{
			if (inBoxCenter.Z < OctreeCenter.Z)
			{
				Result = TopLeftFront;
			}
			else
			{
				Result =  TopLeftBack;
			}
		}	
	}
	else
	{
		if (inBoxCenter.Y < OctreeCenter.Y)
		{
			if (inBoxCenter.Z < OctreeCenter.Z)
			{
				Result = BottomRightFront;
			}
			else
			{
				Result = BottomRightBack;
			}
		}
		else {
			if (inBoxCenter.Z < OctreeCenter.Z)
			{
				Result = TopRightFront;
			}
			else
			{
				Result = TopRightBack;
			}
		}
	}
	
	return Result;
}




// Increments the overall node count of this octree and all its parents
inline void Octree::IncrementCount()
{
	NodesCount++;


	if (Parent != 0)
	{
		Parent->IncrementCount();
	}
}

// Decrements the overall node count of this octree and all its parents
inline void Octree::DecrementCount()
{
	NodesCount--;


	if (Parent != 0)
	{
		Parent->DecrementCount();
	}
}
