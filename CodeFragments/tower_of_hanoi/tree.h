#ifndef TREE_H
#define TREE_H

#include <vector>

template<class T>
class Node
{
public:
	Node(T _data, Node* _parent = NULL, int _depth = 0)
		: data(_data), parent(_parent), depth(_depth)
	{

	}

	bool isRoot()
	{
		return parent == NULL;
	}

	bool isLeaf()
	{
		return children.empty();
	}

	Node* insert(T _data)
	{
		children.push_back(Node(_data, this, depth + 1));

		return &children[children.size() - 1];
	}

	T data;

	Node* parent;
	std::vector<Node> children;

	int depth;
};

template<class T>
class Tree
{
public:
	Tree(T _data)
		: root(_data)
	{

	}

	void clear()
	{
		root.children.clear();
	}

	Node<T> root;
};

#endif