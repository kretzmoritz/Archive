#include "..\..\include\Game\CBossData.h"

//Definition of static variables
pwIniFile *CBossData::data;

CBossData::position CBossData::m_position;

std::vector<CBossData::boss_piece> CBossData::vBossPieces;
std::vector<CBossData::movement_piece> CBossData::vMovementPieces;
std::vector<CBossData::weapon_piece> CBossData::vWeaponPieces;
std::vector<CBossData::weapon_piece_destroyed> CBossData::vWeaponDestroyedPieces;

void CBossData::init(std::string _era)
{
	vBossPieces.clear();
	vMovementPieces.clear();
	vWeaponPieces.clear();

	boss_piece bp_temp;
	movement_piece mp_temp;
	weapon_piece wp_temp;
	weapon_piece_destroyed wpd_temp;

	std::string sCount;
	std::string sInput;
	std::stringstream string_typecast;

	data = glDataBox->GetIniFile("bossdata.ini");

	m_position.x = data->GetFloat("positionX", _era);
	m_position.y = data->GetFloat("positionY", _era);

	for(int i = 0; i < data->GetInt("boss_count", _era); i++)
	{
		string_typecast << i;
		string_typecast >> sCount;
		string_typecast.clear();

		sInput = "piece";
		sInput += sCount;
		sInput += "_texture";

		bp_temp.texture = data->GetString(sInput, _era);
		
		sInput = "piece";
		sInput += sCount;
		sInput += "_layer_offset";

		bp_temp.layer = data->GetInt(sInput, _era);
		
		sInput = "piece";
		sInput += sCount;
		sInput += "_offset_X";

		bp_temp.offset.x = data->GetFloat(sInput, _era);

		sInput = "piece";
		sInput += sCount;
		sInput += "_offset_Y";
		
		bp_temp.offset.y = data->GetFloat(sInput, _era);
		
		vBossPieces.push_back(bp_temp);
		sCount.clear();
	}

	for(int i = 0; i < data->GetInt("movement_count", _era); i++)
	{
		string_typecast << i;
		string_typecast >> sCount;
		string_typecast.clear();

		sInput = sCount;
		sInput += "movement_texture";

		mp_temp.texture = data->GetString(sInput, _era);

		sInput = sCount;
		sInput += "movement_frames";

		mp_temp.frames = data->GetInt(sInput, _era);

		sInput = sCount;
		sInput += "movement_layer_offset";

		mp_temp.layer = data->GetInt(sInput, _era);

		sInput = sCount;
		sInput += "movement_width";

		mp_temp.size.width = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "movement_height";

		mp_temp.size.height = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "movement_offset_X";

		mp_temp.offset.x = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "movement_offset_Y";

		mp_temp.offset.y = data->GetFloat(sInput, _era);

		vMovementPieces.push_back(mp_temp);
		sCount.clear();
	}

	for(int i = 0; i < data->GetInt("weapon_count", _era); i++)
	{
		string_typecast << i;
		string_typecast >> sCount;
		string_typecast.clear();

		sInput = sCount;
		sInput += "weapon_texture";

		wp_temp.texture = data->GetString(sInput, _era);

		sInput = sCount;
		sInput += "weapon_frames";

		wp_temp.frames = data->GetInt(sInput, _era);

		sInput = sCount;
		sInput += "weapon_layer_offset";

		wp_temp.layer = data->GetInt(sInput, _era);

		sInput = sCount;
		sInput += "weapon_width";

		wp_temp.size.width = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_height";

		wp_temp.size.height = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_offset_X";

		wp_temp.offset.x = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_offset_Y";

		wp_temp.offset.y = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_attack_originX";

		wp_temp.attack_origin.x = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_attack_originY";

		wp_temp.attack_origin.y = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_projectile";

		wp_temp.projectile = data->GetInt(sInput, _era);

		vWeaponPieces.push_back(wp_temp);
		sCount.clear();
	}

	for(int i = 0; i < data->GetInt("weapon_destroyed_count", _era); i++)
	{
		string_typecast << i;
		string_typecast >> sCount;
		string_typecast.clear();

		sInput = sCount;
		sInput += "weapon_destroyed_texture";

		wpd_temp.texture = data->GetString(sInput, _era);

		sInput = sCount;
		sInput += "weapon_destroyed_frames";

		wpd_temp.frames = data->GetInt(sInput, _era);

		sInput = sCount;
		sInput += "weapon_destroyed_layer_offset";

		wpd_temp.layer = data->GetInt(sInput, _era);

		sInput = sCount;
		sInput += "weapon_destroyed_width";

		wpd_temp.size.width = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_destroyed_height";

		wpd_temp.size.height = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_destroyed_offset_X";

		wpd_temp.offset.x = data->GetFloat(sInput, _era);

		sInput = sCount;
		sInput += "weapon_destroyed_offset_Y";

		wpd_temp.offset.y = data->GetFloat(sInput, _era);

		vWeaponDestroyedPieces.push_back(wpd_temp);
		sCount.clear();
	}
}