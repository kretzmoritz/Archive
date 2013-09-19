////////////////////////////////////////////////////////////
// Moritz Kretz
// 08/02/2012
// System implementation of boss data return class
////////////////////////////////////////////////////////////

#ifndef BOSSDATA
#define BOSSDATA

#define PWUSEGLOBALSINGLETONS

#include "../pwDataBox.hpp"
#include <string>
#include <vector>

class CBossData
{
	struct position
	{
		float x;
		float y;
	};

	struct size
	{
		float width;
		float height;
	};

	struct boss_piece
	{
		std::string texture;
		int layer;
		position offset;
	};

	struct movement_piece
	{
		std::string texture;
		int frames;
		int layer;
		size size;
		position offset;
	};

	struct weapon_piece
	{
		std::string texture;
		int frames;
		int layer;
		size size;
		position offset;
		position attack_origin;
		int projectile;
	};

	struct weapon_piece_destroyed
	{
		std::string texture;
		int frames;
		int layer;
		size size;
		position offset;
	};

	public:
		CBossData() {};
		~CBossData() {};

		static void init(std::string _era);

		position getPosition() const { return m_position; }
		std::vector<boss_piece> &getBossPieces() const { return vBossPieces; }
		std::vector<movement_piece> &getMovementPieces() const { return vMovementPieces; }
		std::vector<weapon_piece> &getWeaponPieces() const { return vWeaponPieces; }
		std::vector<weapon_piece_destroyed> &getWeaponDestroyedPieces() const { return vWeaponDestroyedPieces; }

	private:
		static pwIniFile *data;

		static position m_position;

		static std::vector<boss_piece> vBossPieces;
		static std::vector<movement_piece> vMovementPieces;
		static std::vector<weapon_piece> vWeaponPieces;
		static std::vector<weapon_piece_destroyed> vWeaponDestroyedPieces;
};

#endif