#include <stack>

#include "chunk_manager.h"
#include "../gfx/render/gfx_renderer.h"
#include "../../math/helper.h"
#include "../../utility/ini_reader.h"

ChunkManager::ChunkManager()
	: m_drawOctree(false)
{
	IniReader settings("settings/syl_vox.ini");

	BLOCK_RENDER_SIZE = settings.readFloat("Voxel", "BLOCK_RENDER_SIZE");
	BLOCK_RENDER_SIZE = BLOCK_RENDER_SIZE > 0.0f ? BLOCK_RENDER_SIZE : 1.0f;

	CHUNK_SIZE = settings.readInt("Voxel", "CHUNK_SIZE");
	CHUNK_SIZE = CHUNK_SIZE > 0 ? CHUNK_SIZE : 1;

	WORLD_SIZE = settings.readInt("Voxel", "WORLD_SIZE");
	WORLD_SIZE = Helper::getNextPowerOf2(WORLD_SIZE > 0 ? WORLD_SIZE : 1);

	MAX_CHUNK_ACTIONS_PER_FRAME = settings.readInt("Voxel", "MAX_CHUNK_ACTIONS_PER_FRAME");
	MAX_CHUNK_ACTIONS_PER_FRAME = MAX_CHUNK_ACTIONS_PER_FRAME > 0 ? MAX_CHUNK_ACTIONS_PER_FRAME : 1;

	float bounds = (BLOCK_RENDER_SIZE * CHUNK_SIZE * WORLD_SIZE) / 2.0f;
	uint depth = static_cast<uint>(log(WORLD_SIZE) / log(2));

	m_octree = new Octree<Chunk*>(Point(-bounds, -bounds, bounds), Point(bounds, bounds, -bounds), depth);
	
	std::queue<OctNode<Chunk*>*> parents;
	parents.push(m_octree->getRoot());

	do
	{
		if (parents.front()->getDepth() == depth)
		{
			parents.front()->m_data = new Chunk(CHUNK_SIZE);
			m_setup.push(parents.front());
		}
		else
		{
			for (uint i = 0; i < NL_Max; ++i)
			{
				parents.push(m_octree->addNode(parents.front(), static_cast<NodeLocation>(i)));
			}
		}

		parents.pop();
	} while (!parents.empty());
}

ChunkManager::~ChunkManager()
{
	for (auto it = m_octree->begin_dfs(); it != m_octree->end_dfs(); ++it)
	{
		if ((*it)->getDepth() == m_octree->getDepth())
		{
			delete (*it)->m_data;
		}
	}

	delete m_octree;
}

void ChunkManager::drawOctree(float _unused)
{
	m_drawOctree = !m_drawOctree;
}

void ChunkManager::update(GfxRenderer* _renderer)
{
	uint actions_setup = 0;
	uint actions_rebuild = 0;

	while (!m_setup.empty() && actions_setup < MAX_CHUNK_ACTIONS_PER_FRAME)
	{
		Vec3f position(	m_setup.front()->getPosition().x - m_setup.front()->getDelta().x, 
						m_setup.front()->getPosition().y - m_setup.front()->getDelta().y, 
						m_setup.front()->getPosition().z - m_setup.front()->getDelta().z);

		setup(m_setup.front()->m_data, position);

		m_rebuild.push(m_setup.front());
		m_setup.pop();

		++actions_setup;
	}

	while (!m_rebuild.empty() && actions_rebuild < MAX_CHUNK_ACTIONS_PER_FRAME)
	{
		auto node = m_rebuild.front();
		m_rebuild.pop();

		bool shouldRender = node->m_data->shouldRender();

		Vec3f position(	node->getPosition().x - node->getDelta().x, 
						node->getPosition().y - node->getDelta().y, 
						node->getPosition().z - node->getDelta().z);

		node->m_data->rebuild(position, BLOCK_RENDER_SIZE);

		int renderFlag = shouldRender - node->m_data->shouldRender();

		while (node->getParent())
		{
			node->getParent()->m_data -= renderFlag;
			node = node->getParent();
		}

		++actions_rebuild;
	}

	std::stack<OctNode<Chunk*>*> nodes;

	if (m_octree->getRoot()->m_data)
	{
		nodes.push(m_octree->getRoot());

		while (!nodes.empty())
		{
			auto current = nodes.top();
			nodes.pop();

			if (current->getDepth() == m_octree->getDepth())
			{
				if (current->m_data->shouldRender())
				{
					current->m_data->render(_renderer);
				}

				if (m_drawOctree)
				{
					_renderer->boundingBox(	Vec3f(current->getPosition().x, current->getPosition().y, current->getPosition().z), 
											Vec3f(current->getDelta().x, current->getDelta().y, current->getDelta().z));
				}
			}
			else
			{
				OctNode<Chunk*>* child;

				for (int i = 7; i >= 0; --i)
				{
					child = current->getChild(static_cast<NodeLocation>(i));

					if (child)
					{
						if (child->m_data)
						{
							nodes.push(child);
						}
						else if (m_drawOctree)
						{
							_renderer->boundingBox(	Vec3f(child->getPosition().x, child->getPosition().y, child->getPosition().z), 
													Vec3f(child->getDelta().x, child->getDelta().y, child->getDelta().z));
						}
					}
				}
			}
		}
	}
}