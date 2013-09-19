#include "..\..\include\Game\CColossData.h"

//Definition of static variables
pwIniFile *CColossData::data;

CColossData::position CColossData::m_position;
float CColossData::m_fFramesPerSecond;
CColossData::move CColossData::m_MoveAnimation;
CColossData::block CColossData::m_BlockAnimation;
CColossData::ragemode CColossData::m_RagemodeAnimation;

std::vector<CColossData::tween> CColossData::back_arm_tween;
std::vector<CColossData::tween> CColossData::front_arm_tween;
std::vector<CColossData::tween> CColossData::tail_tween;

void CColossData::init()
{
	std::stringstream string_typecast;
	std::string temp;
	tween tween;
	data = glDataBox->GetIniFile("colossdata.ini");

	// General
	m_position.x = data->GetFloat("positionX");
	m_position.y = data->GetFloat("positionY");
	m_fFramesPerSecond = data->GetFloat("framesPerSecond");

	// Move
	m_MoveAnimation.torso_texture = data->GetString("torso_texture", "move");
	m_MoveAnimation.torso_frames = data->GetInt("torso_frames", "move");
	m_MoveAnimation.torso_layer_offset = data->GetInt("torso_layer_offset", "move");
	m_MoveAnimation.torso_size.width = data->GetFloat("torso_width", "move");
	m_MoveAnimation.torso_size.height = data->GetFloat("torso_height", "move");
	m_MoveAnimation.torso_origin.x = data->GetFloat("torso_origin_X", "move");
	m_MoveAnimation.torso_origin.y = data->GetFloat("torso_origin_Y", "move");
	m_MoveAnimation.torso_offset.x = data->GetFloat("torso_offset_X", "move");
	m_MoveAnimation.torso_offset.y = data->GetFloat("torso_offset_Y", "move");

	m_MoveAnimation.backarm_texture = data->GetString("backarm_texture", "move");
	m_MoveAnimation.backarm_frames = data->GetInt("backarm_frames", "move");
	m_MoveAnimation.backarm_layer_offset = data->GetInt("backarm_layer_offset", "move");
	m_MoveAnimation.backarm_size.width = data->GetFloat("backarm_width", "move");
	m_MoveAnimation.backarm_size.height = data->GetFloat("backarm_height", "move");
	m_MoveAnimation.backarm_origin.x = data->GetFloat("backarm_origin_X", "move");
	m_MoveAnimation.backarm_origin.y = data->GetFloat("backarm_origin_Y", "move");
	m_MoveAnimation.backarm_offset.x = data->GetFloat("backarm_offset_X", "move");
	m_MoveAnimation.backarm_offset.y = data->GetFloat("backarm_offset_Y", "move");

	m_MoveAnimation.frontarm_texture = data->GetString("frontarm_texture", "move");
	m_MoveAnimation.frontarm_frames = data->GetInt("frontarm_frames", "move");
	m_MoveAnimation.frontarm_layer_offset = data->GetInt("frontarm_layer_offset", "move");
	m_MoveAnimation.frontarm_size.width = data->GetFloat("frontarm_width", "move");
	m_MoveAnimation.frontarm_size.height = data->GetFloat("frontarm_height", "move");
	m_MoveAnimation.frontarm_origin.x = data->GetFloat("frontarm_origin_X", "move");
	m_MoveAnimation.frontarm_origin.y = data->GetFloat("frontarm_origin_Y", "move");
	m_MoveAnimation.frontarm_offset.x = data->GetFloat("frontarm_offset_X", "move");
	m_MoveAnimation.frontarm_offset.y = data->GetFloat("frontarm_offset_Y", "move");

	m_MoveAnimation.backleg_texture = data->GetString("backleg_texture", "move");
	m_MoveAnimation.backleg_frames = data->GetInt("backleg_frames", "move");
	m_MoveAnimation.backleg_layer_offset = data->GetInt("backleg_layer_offset", "move");
	m_MoveAnimation.backleg_size.width = data->GetFloat("backleg_width", "move");
	m_MoveAnimation.backleg_size.height = data->GetFloat("backleg_height", "move");
	m_MoveAnimation.backleg_origin.x = data->GetFloat("backleg_origin_X", "move");
	m_MoveAnimation.backleg_origin.y = data->GetFloat("backleg_origin_Y", "move");
	m_MoveAnimation.backleg_offset.x = data->GetFloat("backleg_offset_X", "move");
	m_MoveAnimation.backleg_offset.y = data->GetFloat("backleg_offset_Y", "move");

	m_MoveAnimation.frontleg_texture = data->GetString("frontleg_texture", "move");
	m_MoveAnimation.frontleg_frames = data->GetInt("frontleg_frames", "move");
	m_MoveAnimation.frontleg_layer_offset = data->GetInt("frontleg_layer_offset", "move");
	m_MoveAnimation.frontleg_size.width = data->GetFloat("frontleg_width", "move");
	m_MoveAnimation.frontleg_size.height = data->GetFloat("frontleg_height", "move");
	m_MoveAnimation.frontleg_origin.x = data->GetFloat("frontleg_origin_X", "move");
	m_MoveAnimation.frontleg_origin.y = data->GetFloat("frontleg_origin_Y", "move");
	m_MoveAnimation.frontleg_offset.x = data->GetFloat("frontleg_offset_X", "move");
	m_MoveAnimation.frontleg_offset.y = data->GetFloat("frontleg_offset_Y", "move");

	m_MoveAnimation.tail_texture = data->GetString("tail_texture", "move");
	m_MoveAnimation.tail_frames = data->GetInt("tail_frames", "move");
	m_MoveAnimation.tail_layer_offset = data->GetInt("tail_layer_offset", "move");
	m_MoveAnimation.tail_size.width = data->GetFloat("tail_width", "move");
	m_MoveAnimation.tail_size.height = data->GetFloat("tail_height", "move");
	m_MoveAnimation.tail_origin.x = data->GetFloat("tail_origin_X", "move");
	m_MoveAnimation.tail_origin.y = data->GetFloat("tail_origin_Y", "move");
	m_MoveAnimation.tail_offset.x = data->GetFloat("tail_offset_X", "move");
	m_MoveAnimation.tail_offset.y = data->GetFloat("tail_offset_Y", "move");

	// Block
	m_BlockAnimation.top_texture = data->GetString("top_texture", "block");
	m_BlockAnimation.top_frames = data->GetInt("top_frames", "block");
	m_BlockAnimation.top_size.width = data->GetFloat("top_width", "block");
	m_BlockAnimation.top_size.height = data->GetFloat("top_height", "block");
	m_BlockAnimation.top_origin.x = data->GetFloat("top_origin_X", "block");
	m_BlockAnimation.top_origin.y = data->GetFloat("top_origin_Y", "block");
	m_BlockAnimation.top_offset.x = data->GetFloat("top_offset_X", "block");
	m_BlockAnimation.top_offset.y = data->GetFloat("top_offset_Y", "block");
	m_BlockAnimation.top_blockStart = data->GetInt("top_blockStart", "block");
	m_BlockAnimation.top_blockEnd = data->GetInt("top_blockEnd", "block");

	m_BlockAnimation.middle_higher_texture = data->GetString("middle_higher_texture", "block");
	m_BlockAnimation.middle_higher_frames = data->GetInt("middle_higher_frames", "block");
	m_BlockAnimation.middle_higher_size.width = data->GetFloat("middle_higher_width", "block");
	m_BlockAnimation.middle_higher_size.height = data->GetFloat("middle_higher_height", "block");
	m_BlockAnimation.middle_higher_origin.x = data->GetFloat("middle_higher_origin_X", "block");
	m_BlockAnimation.middle_higher_origin.y = data->GetFloat("middle_higher_origin_Y", "block");
	m_BlockAnimation.middle_higher_blockStart = data->GetInt("middle_higher_blockStart", "block");
	m_BlockAnimation.middle_higher_blockEnd = data->GetInt("middle_higher_blockEnd", "block");
	m_BlockAnimation.middle_higher_pullbackStart = data->GetInt("middle_higher_pullbackStart", "block");
	m_BlockAnimation.middle_higher_pullbackEnd = data->GetInt("middle_higher_pullbackEnd", "block");

	m_BlockAnimation.middle_lower_texture = data->GetString("middle_lower_texture", "block");
	m_BlockAnimation.middle_lower_frames = data->GetInt("middle_lower_frames", "block");
	m_BlockAnimation.middle_lower_size.width = data->GetFloat("middle_lower_width", "block");
	m_BlockAnimation.middle_lower_size.height = data->GetFloat("middle_lower_height", "block");
	m_BlockAnimation.middle_lower_origin.x = data->GetFloat("middle_lower_origin_X", "block");
	m_BlockAnimation.middle_lower_origin.y = data->GetFloat("middle_lower_origin_Y", "block");
	m_BlockAnimation.middle_lower_blockStart = data->GetInt("middle_lower_blockStart", "block");
	m_BlockAnimation.middle_lower_blockEnd = data->GetInt("middle_lower_blockEnd", "block");
	m_BlockAnimation.middle_lower_pullbackStart = data->GetInt("middle_lower_pullbackStart", "block");
	m_BlockAnimation.middle_lower_pullbackEnd = data->GetInt("middle_lower_pullbackEnd", "block");

	m_BlockAnimation.bottom_texture = data->GetString("bottom_texture", "block");
	m_BlockAnimation.bottom_frames = data->GetInt("bottom_frames", "block");
	m_BlockAnimation.bottom_size.width = data->GetFloat("bottom_width", "block");
	m_BlockAnimation.bottom_size.height = data->GetFloat("bottom_height", "block");
	m_BlockAnimation.bottom_origin.x = data->GetFloat("bottom_origin_X", "block");
	m_BlockAnimation.bottom_origin.y = data->GetFloat("bottom_origin_Y", "block");
	m_BlockAnimation.bottom_blockStart = data->GetInt("bottom_blockStart", "block");
	m_BlockAnimation.bottom_blockEnd = data->GetInt("bottom_blockEnd", "block");
	m_BlockAnimation.bottom_pullbackStart = data->GetInt("bottom_pullbackStart", "block");
	m_BlockAnimation.bottom_pullbackEnd = data->GetInt("bottom_pullbackEnd", "block");

	// Ragemode
	m_RagemodeAnimation.piece0_texture = data->GetString("0piece_texture", "ragemode");
	m_RagemodeAnimation.piece0_frames = data->GetInt("0piece_frames", "ragemode");
	m_RagemodeAnimation.piece0_layer_offset = data->GetInt("0piece_layer_offset", "ragemode");
	m_RagemodeAnimation.piece0_size.width = data->GetFloat("0piece_width", "ragemode");
	m_RagemodeAnimation.piece0_size.height = data->GetFloat("0piece_height", "ragemode");
	m_RagemodeAnimation.piece0_offset.x = data->GetFloat("0piece_offset_X", "ragemode");
	m_RagemodeAnimation.piece0_offset.y = data->GetFloat("0piece_offset_Y", "ragemode");

	m_RagemodeAnimation.piece1_texture = data->GetString("1piece_texture", "ragemode");
	m_RagemodeAnimation.piece1_frames = data->GetInt("1piece_frames", "ragemode");
	m_RagemodeAnimation.piece1_layer_offset = data->GetInt("1piece_layer_offset", "ragemode");
	m_RagemodeAnimation.piece1_size.width = data->GetFloat("1piece_width", "ragemode");
	m_RagemodeAnimation.piece1_size.height = data->GetFloat("1piece_height", "ragemode");
	m_RagemodeAnimation.piece1_offset.x = data->GetFloat("1piece_offset_X", "ragemode");
	m_RagemodeAnimation.piece1_offset.y = data->GetFloat("1piece_offset_Y", "ragemode");

	m_RagemodeAnimation.piece2_texture = data->GetString("2piece_texture", "ragemode");
	m_RagemodeAnimation.piece2_frames = data->GetInt("2piece_frames", "ragemode");
	m_RagemodeAnimation.piece2_layer_offset = data->GetInt("2piece_layer_offset", "ragemode");
	m_RagemodeAnimation.piece2_size.width = data->GetFloat("2piece_width", "ragemode");
	m_RagemodeAnimation.piece2_size.height = data->GetFloat("2piece_height", "ragemode");
	m_RagemodeAnimation.piece2_offset.x = data->GetFloat("2piece_offset_X", "ragemode");
	m_RagemodeAnimation.piece2_offset.y = data->GetFloat("2piece_offset_Y", "ragemode");

	// Tweens
	for(int i = 0; i < m_MoveAnimation.torso_frames; i++)
	{
		// Back arm tween
		string_typecast << i;
		string_typecast >> temp;
		string_typecast.clear();
		
		temp += "x_back_arm";
		tween.x = data->GetFloat(temp, "move");
		temp.clear();

		string_typecast << i;
		string_typecast >> temp;
		string_typecast.clear();

		temp += "y_back_arm";
		tween.y = data->GetFloat(temp, "move");
		temp.clear();

		back_arm_tween.push_back(tween);

		// Front arm tween
		string_typecast << i;
		string_typecast >> temp;
		string_typecast.clear();
		
		temp += "x_front_arm";
		tween.x = data->GetFloat(temp, "move");
		temp.clear();

		string_typecast << i;
		string_typecast >> temp;
		string_typecast.clear();

		temp += "y_front_arm";
		tween.y = data->GetFloat(temp, "move");
		temp.clear();

		front_arm_tween.push_back(tween);

		// Tail tween
		string_typecast << i;
		string_typecast >> temp;
		string_typecast.clear();
		
		temp += "x_tail";
		tween.x = data->GetFloat(temp, "move");
		temp.clear();

		string_typecast << i;
		string_typecast >> temp;
		string_typecast.clear();

		temp += "y_tail";
		tween.y = data->GetFloat(temp, "move");
		temp.clear();

		tail_tween.push_back(tween);
	}
}