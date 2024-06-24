#pragma once

//#include <map>

enum Colour { RED, BLACK };

template<typename T1, typename T2>
class Pair {

	using FirstType = T1;
	using SecondType = T2;

	Pair(const Pair&);
	Pair(Pair&&);


	FirstType First;
	SecondType Second;
};

template<typename Key, typename T>
class TMap
{


public:
	using KeyType = Key;
	using MappedType = T;

	//using _Node = typename _Mybase::_Node;
	//using _Nodeptr = typename _Mybase::_Nodeptr;


	using ValueType = Pair < KeyType, MappedType>;
	//using SizeType //=  ?? probably Pair.SizeType...
	//using DifferenceType //= ??  
	using Pointer = ValueType * ;
	using ConstPointer = ValueType const *;

	using Reference = ValueType & ;
	using ConstReference = ValueType const &;

private:

	class TreeNode
	{
	public:

		TreeNode();

		TreeNode(Key inKey, T inValue, TreeNode * inParent);

		TreeNode(Pair<Key, T> inPair, TreeNode * inParent);
	
	public:
		Colour NodeColour;
		KeyType NodeKey;
		MappedType NodeValue;

		TreeNode *Left;
		TreeNode *Right;

		TreeNode *Parent;
	};


public:
	TMap();

	//non recursive
	TreeNode * Find(KeyType KeyToFind);

	//recursive
	//Node * Find(KeyType KeyToFind, Node * RootNode);

	TreeNode * Insert(KeyType InKey, MappedType InValue);

	TreeNode * Insert(Pair<KeyType, MappedType> InPair);

	
	//operators

	MappedType operator[](KeyType Index);

	TMap<KeyType, MappedType>& operator=(TMap<KeyType, MappedType> & Copy) noexcept;


	TMap<KeyType, MappedType>& operator=(TMap<KeyType, MappedType> && Move) noexcept;
	
	

private:

	//similar to find. Returns the last node it finds (new possible father of a new node)
	TreeNode * Search(KeyType KeyToFind, TreeNode * RootNode);

	void Move(TMap && Right);

private:
	TreeNode * Root;
};

template<typename Key, typename T>
inline TMap<Key, T>::TMap()
{

	//Root = new Node("test", 1);
	std::cout << "New Map yeah\n";
}

template<typename Key, typename T>
inline typename TMap<Key, T>::TreeNode * TMap<Key, T>::Find(KeyType KeyToFind)
{
	TreeNode * TempRoot = Root;
	//Non recursive
	while (TempRoot != nullptr) {

		if (KeyToFind == TempRoot->NodeKey)
		{
			return TempRoot;
		}
		else if (KeyToFind < TempRoot->NodeKey)
		{
			TempRoot = TempRoot->Left;
		}
		else if (KeyToFind > TempRoot->NodeKey)
		{
			TempRoot = TempRoot->Right;
		}
	}

	return nullptr;
}

template<typename Key, typename T>
inline typename TMap<Key, T>::TreeNode * TMap<Key, T>::Insert(KeyType InKey, MappedType InValue)
{
	TreeNode * TempRoot = Root;
	if (Root == nullptr)
	{
		Root = new TreeNode(InKey, InValue, nullptr); //TODO: check if failed

		TempRoot = Root;
	}
	else
	{
		TreeNode * TempParent = nullptr;

		bool Direction = 0; //0 is left, 1 is right

		while (TempRoot != nullptr)
		{
			TempParent = TempRoot;

			if (InKey < TempRoot->NodeKey)
			{
				TempRoot = TempRoot->Left;
				Direction = 0;
			}
			else if (InKey > TempRoot->NodeKey)
			{
				TempRoot = TempRoot->Right;
				Direction = 1;
			}
		}
		TempRoot = new TreeNode(InKey, InValue, TempParent); //TODO: check if failed
		if (Direction)
		{
			TempParent->Right = TempRoot;
		}
		else
		{
			TempParent->Left = TempRoot;
		}
	}

	return TempRoot;
}


template<typename Key, typename T>
inline typename TMap<Key, T>::TreeNode * TMap<Key, T>::Insert(Pair<Key, T> InPair)
{
	TreeNode * TempRoot = Root;

	if (Root == nullptr)
	{
		Root = new TreeNode(InPair);
	}
	else
	{
		while (TempRoot != nullptr)
		{

			if (TempRoot == nullptr)
			{
				TempRoot = new TreeNode(InPair);
			}
			else if (InPair.First < TempRoot->NodeKey)
			{
				TempRoot = TempRoot->Left;
			}
			else if (InPair.First > TempRoot->NodeKey)
			{
				TempRoot = TempRoot->Right;
			}
		}
	}
	return TempRoot;
}


//------------------------------------------------------------------------------
//		operators
//------------------------------------------------------------------------------

template<typename Key, typename T>
inline T TMap<Key, T>::operator[](Key Index)
{
	TreeNode * Temp = Find(Index);

	if (Temp)
	{
		return Temp->NodeValue;
	}
	return Insert(Index,T())->NodeValue;
}


template<typename Key, typename T>
inline TMap<Key, T>& TMap<Key, T>::operator=(TMap<Key, T>& Copy) noexcept
{
	this->Root = Copy.Root;

	return *this;
}

//TODO: fix this!
template<typename Key, typename T>
inline typename TMap<Key, T> & TMap<Key, T>::operator=(TMap<Key, T> && Move) noexcept
{
	if (this != *Move)
	{
		//Clear the old Root

		this->Root = nullptr;
		
		this->Move(Move);

		//this->Root = Move->Root;
	}

	return (*this);
}




//------------------------------------------------------------------------------
//		private methods
//------------------------------------------------------------------------------




//TODO: delete this???
template<typename Key, typename T>
inline typename TMap<Key, T>::TreeNode * TMap<Key, T>::Search(KeyType KeyToFind, TreeNode * RootNode)
{

	if (KeyToFind == RootNode->NodeKey)
	{
		return RootNode;
	}
	else if (KeyToFind < RootNode->NodeKey)
	{
		return Find(KeyToFind, RootNode->Left);
	}
	else if (KeyToFind > RootNode->NodeKey)
	{
		return Find(KeyToFind, RootNode->Right);
	}

	return nullptr;
}

template<typename Key, typename T>
inline void TMap<Key, T>::Move(TMap && Right)
{
	this =  (static_cast<std::remove_reference_t<TMap>&&>(Right));
}

template<typename Key, typename T>
inline TMap<Key, T>::TreeNode::TreeNode() :
	NodeColour(BLACK),
	NodeKey(),
	NodeValue(),
	Left(nullptr),
	Right(nullptr),
	Parent(nullptr)
{
}

template<typename Key, typename T>
inline TMap<Key, T>::TreeNode::TreeNode(Key inKey, T inValue, TreeNode* inParent) :
	NodeColour(BLACK),
	NodeKey(inKey),
	NodeValue(inValue),
	Left(nullptr),
	Right(nullptr),
	Parent(inParent)
{

}

template<typename Key, typename T>
inline TMap<Key, T>::TreeNode::TreeNode(Pair<Key, T> inPair, TreeNode* inParent) :
	NodeColour(BLACK),
	NodeKey(inPair.First),
	NodeValue(inPair.Second),
	Left(nullptr),
	Right(nullptr),
	Parent(inParent)
{
}
