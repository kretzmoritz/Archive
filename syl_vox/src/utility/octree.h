//////////////////////////////
// Moritz Kretz
// ToDo:
// - Shrink, grow
// - Change bounds and depth
// - Neighbour access O(1)
//////////////////////////////

#ifndef OCTREE_H
#define OCTREE_H

#include "tree.h"

enum NodeLocation
{
	NL_FRONT_BOTTOM_LEFT,
	NL_FRONT_BOTTOM_RIGHT,
	NL_FRONT_TOP_LEFT,
	NL_FRONT_TOP_RIGHT,
	NL_BACK_BOTTOM_LEFT,
	NL_BACK_BOTTOM_RIGHT,
	NL_BACK_TOP_LEFT,
	NL_BACK_TOP_RIGHT,
	NL_Max
};

struct Point
{
	Point(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
		: x(_x), y(_y), z(_z) {}

	bool operator==(const Point& _p) const
	{
		return x == _p.x && y == _p.y && z == _p.z;
	}

	bool operator<(const Point& _p) const
	{
		return x < _p.x && y < _p.y && z < _p.z;
	}

	bool operator<=(const Point& _p) const
	{
		return x <= _p.x && y <= _p.y && z <= _p.z;
	}

	bool operator>(const Point& _p) const
	{
		return x > _p.x && y > _p.y && z > _p.z;
	}

	bool operator>=(const Point& _p) const
	{
		return x >= _p.x && y >= _p.y && z >= _p.z;
	}

	Point operator+(const Point& _p) const
	{
		return Point(x + _p.x, y + _p.y, z + _p.z);
	}

	Point operator-(const Point& _p) const
	{
		return Point(x - _p.x, y - _p.y, z - _p.z);
	}

	Point operator*(const float _c) const
	{
		return Point(x * _c, y * _c, z * _c);
	}
	
	Point operator/(const float _c) const
	{
		return Point(x / _c, y / _c, z / _c);
	}

	float x, y, z;
};

template<class T>
class Octree;

template<class T>
class OctNode : public Node<T, NL_Max>
{
	template<class T> friend class Octree;

public:
	OctNode(OctNode* _parent = nullptr)
		: Node<T, NL_Max>(_parent), m_tree()
	{
		m_depth = _parent ? _parent->m_depth + 1 : 0;
	}

	OctNode* getParent() const
	{
		return static_cast<OctNode*>(Node<T, NL_Max>::getParent());
	}

	OctNode* getChild(NodeLocation _location) const
	{
		return static_cast<OctNode*>(Node<T, NL_Max>::getChild(_location));
	}
	
	Point getPosition() const
	{
		return m_position;
	}

	Point getDelta() const
	{
		return m_tree->m_delta / static_cast<float>(pow(2, m_depth));
	}

	unsigned int getDepth() const
	{
		return m_depth;
	}

private:
	Octree<T>* m_tree;

	Point m_position;
	unsigned int m_depth;
};

template<class T>
class Octree : public Tree<T, NL_Max, OctNode<T>>
{
	template<class T> friend class OctNode;

public:
	Octree(Point _min, Point _max, unsigned int _depth)
		: m_delta((_max - _min) / 2.0f), m_depth(_depth)
	{
		getRoot()->m_tree = this;
		getRoot()->m_position = _min + m_delta;
	}

	OctNode<T>* addNode(OctNode<T>* _parent, NodeLocation _location) //Name hiding
	{
		if (this != _parent->m_tree || m_depth == _parent->getDepth())
		{
			return nullptr;
		}

		OctNode<T>* node = Tree<T, NL_Max, OctNode<T>>::addNode(_parent, _location);
		
		if (node->m_tree)
		{
			return node;
		}

		node->m_tree = this;
		Point delta = node->getDelta();

		switch(_location)
		{
		case NL_FRONT_BOTTOM_LEFT:
			node->m_position = _parent->m_position - delta;
			break;
		case NL_FRONT_BOTTOM_RIGHT:
			node->m_position = _parent->m_position + Point(delta.x, -delta.y, -delta.z);
			break;
		case NL_FRONT_TOP_LEFT:
			node->m_position = _parent->m_position + Point(-delta.x, delta.y, -delta.z);
			break;
		case NL_FRONT_TOP_RIGHT:
			node->m_position = _parent->m_position + Point(delta.x, delta.y, -delta.z);
			break;
		case NL_BACK_BOTTOM_LEFT:
			node->m_position = _parent->m_position + Point(-delta.x, -delta.y, delta.z);
			break;
		case NL_BACK_BOTTOM_RIGHT:
			node->m_position = _parent->m_position + Point(delta.x, -delta.y, delta.z);
			break;
		case NL_BACK_TOP_LEFT:
			node->m_position = _parent->m_position + Point(-delta.x, delta.y, delta.z);
			break;
		case NL_BACK_TOP_RIGHT:
			node->m_position = _parent->m_position + delta;
			break;
		}
		
		return node;
	}

	OctNode<T>* getNodeClosestToPosition(Point _position) const
	{
		return findNodeClosestToPosition(_position, getRoot());
	}

	unsigned int getDepth() const
	{
		return m_depth;
	}

private:
	OctNode<T>* findNodeClosestToPosition(Point _position, OctNode<T>* _node) const
	{
		if (_node)
		{
			Point delta = _node->getDelta();

			if (_position >= _node->m_position - delta && _position < _node->m_position + delta)
			{
				for (unsigned int i = 0; i < NL_Max; ++i)
				{
					if (OctNode<T>* result = findNodeClosestToPosition(_position, _node->getChild(static_cast<NodeLocation>(i))))
					{
						return result;
					}
				}

				return _node;
			}
		}

		return nullptr;
	}

	Point m_delta;
	unsigned int m_depth;
};

#endif