#ifndef BLOCK_H
#define BLOCK_H

enum BlockType
{
	BT_Default,
	BT_Dirt,
	BT_Grass,
	BT_Rock,
	BT_MAX
};

struct Block
{
	Block();

	bool m_active;
	BlockType m_type;
};

#endif