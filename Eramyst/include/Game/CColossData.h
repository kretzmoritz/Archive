////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/24/2012
// System implementation of coloss data return class
////////////////////////////////////////////////////////////

#ifndef COLOSSDATA
#define COLOSSDATA

#define PWUSEGLOBALSINGLETONS

#include "../pwDataBox.hpp"
#include <string>
#include <vector>

class CColossData
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

	struct tween
	{
		float x;
		float y;
	};

	struct move
	{
		std::string torso_texture;
		int torso_frames;
		int torso_layer_offset;
		size torso_size;
		position torso_origin;
		position torso_offset;

		std::string backarm_texture;
		int backarm_frames;
		int backarm_layer_offset;
		size backarm_size;
		position backarm_origin;
		position backarm_offset;

		std::string frontarm_texture;
		int frontarm_frames;
		int frontarm_layer_offset;
		size frontarm_size;
		position frontarm_origin;
		position frontarm_offset;

		std::string backleg_texture;
		int backleg_frames;
		int backleg_layer_offset;
		size backleg_size;
		position backleg_origin;
		position backleg_offset;

		std::string frontleg_texture;
		int frontleg_frames;
		int frontleg_layer_offset;
		size frontleg_size;
		position frontleg_origin;
		position frontleg_offset;

		std::string tail_texture;
		int tail_frames;
		int tail_layer_offset;
		size tail_size;
		position tail_origin;
		position tail_offset;
	};

	struct block
	{
		std::string top_texture;
		int top_frames;
		size top_size;
		position top_origin;
		position top_offset;
		int top_blockStart;
		int top_blockEnd;

		std::string middle_higher_texture;
		int middle_higher_frames;
		size middle_higher_size;
		position middle_higher_origin;
		int middle_higher_blockStart;
		int middle_higher_blockEnd;
		int middle_higher_pullbackStart;
		int middle_higher_pullbackEnd;

		std::string middle_lower_texture;
		int middle_lower_frames;
		size middle_lower_size;
		position middle_lower_origin;
		int middle_lower_blockStart;
		int middle_lower_blockEnd;
		int middle_lower_pullbackStart;
		int middle_lower_pullbackEnd;

		std::string bottom_texture;
		int bottom_frames;
		size bottom_size;
		position bottom_origin;
		int bottom_blockStart;
		int bottom_blockEnd;
		int bottom_pullbackStart;
		int bottom_pullbackEnd;
	};

	struct ragemode
	{
		std::string piece0_texture;
		int piece0_frames;
		int piece0_layer_offset;
		size piece0_size;
		position piece0_offset;

		std::string piece1_texture;
		int piece1_frames;
		int piece1_layer_offset;
		size piece1_size;
		position piece1_offset;

		std::string piece2_texture;
		int piece2_frames;
		int piece2_layer_offset;
		size piece2_size;
		position piece2_offset;
	};

	public:
		CColossData() {};
		~CColossData() {};

		static void init();

		position getPosition() const { return m_position; }
		float getFramesPerSecond() const { return m_fFramesPerSecond; }
		const move &getMoveAnimation() const { return m_MoveAnimation; }
		const block &getBlockAnimation() const { return m_BlockAnimation; }
		const ragemode &getRagemodeAnimation() const { return m_RagemodeAnimation; }
		const std::vector<tween> &getBackArmTween() const { return back_arm_tween; }
		const std::vector<tween> &getFrontArmTween() const { return front_arm_tween; }
		const std::vector<tween> &getTailTween() const { return tail_tween; }

	private:
		static pwIniFile *data;

		static position m_position;
		static float m_fFramesPerSecond;
		static move m_MoveAnimation;
		static block m_BlockAnimation;
		static ragemode m_RagemodeAnimation;

		static std::vector<tween> back_arm_tween;
		static std::vector<tween> front_arm_tween;
		static std::vector<tween> tail_tween;
};

#endif