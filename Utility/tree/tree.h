//////////////////////////////
// Moritz Kretz
// ToDo:
// - Iterator_bfs
//////////////////////////////

#ifndef TREE_H
#define TREE_H

#include <stack>

#pragma warning(disable:4351)

template<class T, unsigned int child_count>
class Node;

template<class T, unsigned int child_count, class U = Node<T, child_count>>
class Tree;

template<class T, unsigned int child_count>
class Node
{
	template<class T, unsigned int child_count, class U> friend class Tree;

public:
	Node(Node* _parent = nullptr)
		: m_parent(_parent), m_children(), m_data() {}

	~Node()
	{
		for (unsigned int i = 0; i < child_count; ++i)
		{
			delete m_children[i];
		}
	}

	Node* getParent() const
	{
		return m_parent;
	}

	Node* getChild(unsigned int _index) const
	{
		return m_children[_index];
	}

	T m_data;

private:
	Node* m_parent;
	Node* m_children[child_count];
};

template<class T, unsigned int child_count, class U>
class Tree
{
public:
	class Iterator_dfs;

	Tree()
		: m_root(new U)
	{
		(void)static_cast<Node<T, child_count>*>(static_cast<U*>(nullptr)); //Only derived classes of "Node" are accepted
	}

	~Tree()
	{
		delete m_root;
	}

	U* addNode(U* _parent, unsigned int _index)
	{
		if (!_parent->m_children[_index])
		{
			_parent->m_children[_index] = new U(_parent);
		}

		return static_cast<U*>(_parent->m_children[_index]);
	}

	void removeNode(U* _node)
	{
		for (unsigned int i = 0; i < child_count; ++i)
		{
			if (_node == _node->m_parent->m_children[i])
			{
				_node->m_parent->m_children[i] = nullptr;
				break;
			}
		}

		delete _node;
	}

	U* getRoot() const
	{
		return m_root;
	}

	Iterator_dfs begin_dfs() const
	{
		return Iterator_dfs(m_root);
	}

	Iterator_dfs end_dfs() const
	{
		return Iterator_dfs(nullptr);
	}

private:
	U* m_root;
};

template<class T, unsigned int child_count, class U>
class Tree<T, child_count, U>::Iterator_dfs
{
public:
	Iterator_dfs(U* _node)
		: m_node(_node)
	{
		(void)static_cast<Node<T, child_count>*>(static_cast<U*>(nullptr));
	}

	U* operator*() const
	{
		return m_node;
	}

	bool operator==(const Iterator_dfs& _it) const
	{
		return m_node == _it.m_node;
	}

	bool operator!=(const Iterator_dfs& _it) const
	{
		return m_node != _it.m_node;
	}

	Iterator_dfs& operator++()
	{
		for (unsigned int i = 0; i < child_count; ++i)
		{
			if (m_node->m_children[i])
			{
				for (unsigned int j = child_count - 2; j > i; --j)
				{
					if (m_node->m_children[j])
					{
						m_stack.push(static_cast<U*>(m_node->m_children[j]));
					}
				}

				m_node = static_cast<U*>(m_node->m_children[i]);

				return *this;
			}
		}

		if (!m_stack.empty())
		{
			m_node = m_stack.top();
			m_stack.pop();
		}
		else
		{
			m_node = nullptr;
		}

		return *this;
	}

private:
	U* m_node;
	std::stack<U*> m_stack;
};

#pragma warning(default:4351)

#endif