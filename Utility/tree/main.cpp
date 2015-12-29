#include <iostream>

#include "octree.h"

int main()
{
	Point p0(-0.5f, -0.5f, -0.5f);
	Point p1(1.5f, 1.5f, 1.5f);

	Octree<int> t(p0, p1, 3);
	
	auto node0 = t.addNode(t.getRoot(), NL_FRONT_BOTTOM_LEFT);
	auto node1 = t.addNode(node0, NL_FRONT_BOTTOM_LEFT);
	auto node2 = t.addNode(node0, NL_FRONT_BOTTOM_RIGHT);
	auto node3 = t.addNode(node0, NL_FRONT_TOP_LEFT);
	auto node4 = t.addNode(node0, NL_FRONT_TOP_RIGHT);
	auto node5 = t.addNode(node4, NL_BACK_TOP_RIGHT);

	auto node6 = t.addNode(t.getRoot(), NL_FRONT_BOTTOM_RIGHT);
	auto node7 = t.addNode(node6, NL_FRONT_BOTTOM_LEFT);
	auto node8 = t.addNode(node6, NL_FRONT_BOTTOM_RIGHT);
	auto node9 = t.addNode(node7, NL_FRONT_BOTTOM_LEFT);
	auto node10 = t.addNode(node7, NL_FRONT_BOTTOM_RIGHT);

	std::cout << t.getNodeClosestToPosition(Point(-0.5f, -0.5f, -0.5f)) << std::endl;

	for (Octree<int>::Iterator_dfs it = t.begin_dfs(); it != t.end_dfs(); ++it)
	{
		std::cout << (*it)->getDepth() << ' ' << (*it)->getPosition().x << ' ' << (*it)->getPosition().y << ' ' << (*it)->getPosition().z << ' ' << (*it) << std::endl;
	}

	system("pause");
}