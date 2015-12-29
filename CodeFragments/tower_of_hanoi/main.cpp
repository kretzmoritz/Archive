#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <unordered_set>

#include "tree.h"

/////////////////////////////////////////////////////////////////////////////////////

#define max_depth 50

/////////////////////////////////////////////////////////////////////////////////////

std::vector<std::list<int>> pegs_init, pegs_final;
std::vector<std::vector<std::list<int>>> path;
std::unordered_set<std::string> known_states;

/////////////////////////////////////////////////////////////////////////////////////

std::string get_state(std::vector<std::list<int>>& _pegs)
{
	std::ostringstream ss;

	for (unsigned i = 0; i < _pegs.size(); ++i)
	{
		for (auto it = _pegs[i].begin(); it != _pegs[i].end(); ++it)
		{
			ss << i << *it;
		}
	}

	return ss.str();
}

Node<std::vector<std::list<int>>>* hanoi_test(std::queue<Node<std::vector<std::list<int>>>*>& _bfs_nodes)
{
	std::string state_final = get_state(pegs_final);

	for (unsigned i = 0; i < _bfs_nodes.front()->data.size(); ++i)
	{
		if (!_bfs_nodes.front()->data[i].empty())
		{
			for (unsigned j = 0; j < _bfs_nodes.front()->data.size(); ++j)
			{
				if (i != j)
				{
					if (_bfs_nodes.front()->data[j].empty() || _bfs_nodes.front()->data[i].back() < _bfs_nodes.front()->data[j].back())
					{
						std::vector<std::list<int>> state_new = _bfs_nodes.front()->data;
						state_new[j].push_back(state_new[i].back());
						state_new[i].pop_back();

						auto node_new = _bfs_nodes.front()->insert(state_new);

						if (get_state(state_new) == state_final)
						{
							return node_new;
						}
					}
				}
			}
		}
	}

	return NULL;
}

void bfs()
{
	Tree<std::vector<std::list<int>>> tree(pegs_init);
	Node<std::vector<std::list<int>>>* result = NULL;
	
	std::queue<Node<std::vector<std::list<int>>>*> bfs_nodes;
	bfs_nodes.push(&tree.root);

	while (!bfs_nodes.empty())
	{
		if (result = hanoi_test(bfs_nodes))
		{
			break;
		}

		if (bfs_nodes.front()->depth < max_depth) //children are depth + 1 (so run this for < max depth)
		{
			for (unsigned i = 0; i < bfs_nodes.front()->children.size(); ++i)
			{
				std::string state = get_state(bfs_nodes.front()->children[i].data);
				if(known_states.find(state) != known_states.end())
					continue;

				known_states.emplace(state);
				bfs_nodes.push(&bfs_nodes.front()->children[i]);
			}
		}

		bfs_nodes.pop();
	}

	if (result)
	{
		path.push_back(result->data);

		while (!result->isRoot())
		{
			result = result->parent;
			path.push_back(result->data);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////

void output()
{
	if (!path.empty())
	{
		std::cout << "Length of path: " << path.size() - 1 << std::endl;

		for (size_t i = path.size() - 1; i > 0; --i)
		{
			for (size_t j = 0; j < path[i].size(); ++j) //Peg count is equal for all states
			{
				if (path[i][j].size() > path[i - 1][j].size())
				{
					std::cout << "Move disc " << path[i][j].back() + 1 << " from peg " << j + 1 << ' ';
				}
			}

			for (size_t j = 0; j < path[i].size(); ++j)
			{
				if (path[i][j].size() < path[i - 1][j].size())
				{
					std::cout << "to peg " << j + 1 << std::endl;
				}
			}
		}
	}
	else
	{
		std::cout << "No result found... " + std::to_string(max_depth + 1) + " or more moves required" << std::endl;
	}
	
	system("pause");
}

/////////////////////////////////////////////////////////////////////////////////////

int main()
{
	int n, k;
	do
	{
		std::cout << "pegs: ";
		std::cin >> k;
		std::cout << "discs: ";
		std::cin >> n;
	} while (!(n >= 1 && n <= 8) || !(k >= 3 && k <= 5));

	pegs_init.resize(k);
	pegs_final.resize(k);

	std::vector<int> input;
	input.resize(n);

	std::cout << "Init state!" << std::endl;

	//Init
	bool valid;
	do
	{
		valid = true;

		for(int i = 0; i < n; ++i)
		{
			std::cout << "disc " + std::to_string(i + 1) + " on peg: ";
			std::cin >> input[i];
			
			if (!(input[i] >= 1 && input[i] <= k))
			{
				valid = false;
				break;
			}
		}
	} while (!valid);

	for (int i = 0; i < n; ++i) //Sort discs from bottom to top (decreasing order)
	{
		pegs_init[input[i] - 1].push_front(i);
	}

	std::cout << "Final state!" << std::endl;

	//Final
	do
	{
		valid = true;

		for(int i = 0; i < n; ++i)
		{
			std::cout << "disc " + std::to_string(i + 1) + " on peg: ";
			std::cin >> input[i];
			
			if (!(input[i] >= 1 && input[i] <= k))
			{
				valid = false;
				break;
			}
		}
	} while (!valid);

	for (int i = 0; i < n; ++i)
	{
		pegs_final[input[i] - 1].push_front(i);
	}

	//CleanUp
	input.clear();

	std::cout << "Working..." << std::endl;

	//Algorithm
	bfs();

	//Result
	output();

	return 0;
}