#include "..\..\include\Game\CColoss.h"

//Definition of static variables
CColossData CColoss::ci_coloss_data;
pwIniFile *CColoss::modifier_data;

float CColoss::m_fOriginX;
float CColoss::m_fOriginY;

sf::Text CColoss::text_negative_points;
float CColoss::m_fFadeOutText;

float CColoss::m_fHealth;
bool CColoss::m_bImmortal;
int CColoss::m_iModifier;
int CColoss::m_iBlockedUnits;
int CColoss::m_iSteps;
int CColoss::m_iBPMIncreaseCount;
bool CColoss::m_bBoss;
bool CColoss::m_bBossFinished;

int CColoss::m_iMultiplierDuration;
int CColoss::m_iBPMDuration;
float CColoss::m_fHealthIncrease;

pwCollisionRect CColoss::m_hitableCollision;

std::vector<CColoss::coloss_piece> CColoss::m_vColossMove;
std::vector<CColoss::coloss_piece> CColoss::m_vColossBlock;
std::vector<CColoss::coloss_piece> CColoss::m_vColossRagemode;

sf::RectangleShape CColoss::rectScreenFlash;
float CColoss::m_fFadeOutFlash;
bool CColoss::bInit;
bool CColoss::bFinalPointsAdded;

void CColoss::init()
{
	constructColoss();

	m_hitableCollision.SetSize(350.0f, 950.0f);
	m_hitableCollision.setPosition(m_vColossMove[0].animation.getPosition().x, m_fOriginY);

	text_negative_points.setFont(*glDataBox->GetFont("pecita.otf"));
    text_negative_points.setCharacterSize(100);
	text_negative_points.setColor(sf::Color(0, 0, 0, 0));

	rectScreenFlash.setSize(sf::Vector2f(1920.0f, 1080.0f));
	rectScreenFlash.setFillColor(sf::Color(255, 255, 255, 0));
}

void CColoss::constructColoss()
{
	// Origin position
	m_fOriginX = ci_coloss_data.getPosition().x;
	m_fOriginY = ci_coloss_data.getPosition().y;

	// Frames per second
	float fFramesPerSecond = ci_coloss_data.getFramesPerSecond();

	// Resize vectors
	m_vColossMove.resize(6);
	m_vColossBlock.resize(4);
	m_vColossRagemode.resize(3);
	
	// Move
	m_vColossMove[0].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getMoveAnimation().torso_texture)); // Torso
	m_vColossMove[0].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().torso_offset.x + ci_coloss_data.getMoveAnimation().torso_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().torso_offset.y + ci_coloss_data.getMoveAnimation().torso_origin.y);
	m_vColossMove[0].animation.setOrigin(ci_coloss_data.getMoveAnimation().torso_origin.x, ci_coloss_data.getMoveAnimation().torso_origin.y);
	m_vColossMove[0].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossMove[0].animation.SetTotalFrames(ci_coloss_data.getMoveAnimation().torso_frames);
	m_vColossMove[0].animation.SetFrameSize((int)ci_coloss_data.getMoveAnimation().torso_size.width, (int)ci_coloss_data.getMoveAnimation().torso_size.height);
	m_vColossMove[0].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getMoveAnimation().torso_frames - 1);
	m_vColossMove[0].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossMove[0].layer_offset = ci_coloss_data.getMoveAnimation().torso_layer_offset;

	m_vColossMove[1].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getMoveAnimation().backarm_texture)); // Back arm
	m_vColossMove[1].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().backarm_offset.x + ci_coloss_data.getMoveAnimation().backarm_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().backarm_offset.y + ci_coloss_data.getMoveAnimation().backarm_origin.y);
	m_vColossMove[1].animation.setOrigin(ci_coloss_data.getMoveAnimation().backarm_origin.x, ci_coloss_data.getMoveAnimation().backarm_origin.y);
	m_vColossMove[1].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossMove[1].animation.SetTotalFrames(ci_coloss_data.getMoveAnimation().backarm_frames);
	m_vColossMove[1].animation.SetFrameSize((int)ci_coloss_data.getMoveAnimation().backarm_size.width, (int)ci_coloss_data.getMoveAnimation().backarm_size.height);
	m_vColossMove[1].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getMoveAnimation().backarm_frames - 1);
	m_vColossMove[1].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossMove[1].layer_offset = ci_coloss_data.getMoveAnimation().backarm_layer_offset;

	m_vColossMove[2].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getMoveAnimation().frontarm_texture)); // Front arm
	m_vColossMove[2].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().frontarm_offset.x + ci_coloss_data.getMoveAnimation().frontarm_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().frontarm_offset.y + ci_coloss_data.getMoveAnimation().frontarm_origin.y);
	m_vColossMove[2].animation.setOrigin(ci_coloss_data.getMoveAnimation().frontarm_origin.x, ci_coloss_data.getMoveAnimation().frontarm_origin.y);
	m_vColossMove[2].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossMove[2].animation.SetTotalFrames(ci_coloss_data.getMoveAnimation().frontarm_frames);
	m_vColossMove[2].animation.SetFrameSize((int)ci_coloss_data.getMoveAnimation().frontarm_size.width, (int)ci_coloss_data.getMoveAnimation().frontarm_size.height);
	m_vColossMove[2].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getMoveAnimation().frontarm_frames - 1);
	m_vColossMove[2].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossMove[2].layer_offset = ci_coloss_data.getMoveAnimation().frontarm_layer_offset;

	m_vColossMove[3].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getMoveAnimation().backleg_texture)); // Back leg
	m_vColossMove[3].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().backleg_offset.x + ci_coloss_data.getMoveAnimation().backleg_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().backleg_offset.y + ci_coloss_data.getMoveAnimation().backleg_origin.y);
	m_vColossMove[3].animation.setOrigin(ci_coloss_data.getMoveAnimation().backleg_origin.x, ci_coloss_data.getMoveAnimation().backleg_origin.y);
	m_vColossMove[3].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossMove[3].animation.SetTotalFrames(ci_coloss_data.getMoveAnimation().backleg_frames);
	m_vColossMove[3].animation.SetFrameSize((int)ci_coloss_data.getMoveAnimation().backleg_size.width, (int)ci_coloss_data.getMoveAnimation().backleg_size.height);
	m_vColossMove[3].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getMoveAnimation().backleg_frames - 1);
	m_vColossMove[3].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossMove[3].layer_offset = ci_coloss_data.getMoveAnimation().backleg_layer_offset;

	m_vColossMove[4].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getMoveAnimation().frontleg_texture)); // Front leg
	m_vColossMove[4].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().frontleg_offset.x + ci_coloss_data.getMoveAnimation().frontleg_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().frontleg_offset.y + ci_coloss_data.getMoveAnimation().frontleg_origin.y);
	m_vColossMove[4].animation.setOrigin(ci_coloss_data.getMoveAnimation().frontleg_origin.x, ci_coloss_data.getMoveAnimation().frontleg_origin.y);
	m_vColossMove[4].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossMove[4].animation.SetTotalFrames(ci_coloss_data.getMoveAnimation().frontleg_frames);
	m_vColossMove[4].animation.SetFrameSize((int)ci_coloss_data.getMoveAnimation().frontleg_size.width, (int)ci_coloss_data.getMoveAnimation().frontleg_size.height);
	m_vColossMove[4].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getMoveAnimation().frontleg_frames - 1);
	m_vColossMove[4].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossMove[4].layer_offset = ci_coloss_data.getMoveAnimation().frontleg_layer_offset;

	m_vColossMove[5].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getMoveAnimation().tail_texture)); // Tail
	m_vColossMove[5].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().tail_offset.x + ci_coloss_data.getMoveAnimation().tail_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().tail_offset.y + ci_coloss_data.getMoveAnimation().tail_origin.y);
	m_vColossMove[5].animation.setOrigin(ci_coloss_data.getMoveAnimation().tail_origin.x, ci_coloss_data.getMoveAnimation().tail_origin.y);
	m_vColossMove[5].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossMove[5].animation.SetTotalFrames(ci_coloss_data.getMoveAnimation().tail_frames);
	m_vColossMove[5].animation.SetFrameSize((int)ci_coloss_data.getMoveAnimation().tail_size.width, (int)ci_coloss_data.getMoveAnimation().tail_size.height);
	m_vColossMove[5].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getMoveAnimation().tail_frames - 1);
	m_vColossMove[5].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossMove[5].layer_offset = ci_coloss_data.getMoveAnimation().tail_layer_offset;

	// Block
	m_vColossBlock[0].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getBlockAnimation().top_texture)); // Top
	m_vColossBlock[0].animation.setOrigin(ci_coloss_data.getBlockAnimation().top_origin.x, ci_coloss_data.getBlockAnimation().top_origin.y);
	m_vColossBlock[0].animation.setPosition(m_fOriginX + ci_coloss_data.getBlockAnimation().top_offset.x, m_fOriginY + ci_coloss_data.getBlockAnimation().top_offset.y);
	m_vColossBlock[0].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossBlock[0].animation.SetTotalFrames(ci_coloss_data.getBlockAnimation().top_frames);
	m_vColossBlock[0].animation.SetFrameSize((int)ci_coloss_data.getBlockAnimation().top_size.width, (int)ci_coloss_data.getBlockAnimation().top_size.height);
	m_vColossBlock[0].animation.AddAnimation(C_ANIM_IDLE, 0, 0);
	m_vColossBlock[0].animation.AddAnimation(C_ANIM_BLOCK, ci_coloss_data.getBlockAnimation().top_blockStart, ci_coloss_data.getBlockAnimation().top_blockEnd);
	m_vColossBlock[0].animation.AddAnimation(C_ANIM_PULLBACK, 0, 0);
	m_vColossBlock[0].animation.SetCurrentAnimation(C_ANIM_IDLE);
	m_vColossBlock[0].layer_offset = -1;

	m_vColossBlock[1].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getBlockAnimation().middle_higher_texture)); // Middle higher
	m_vColossBlock[1].animation.setOrigin(ci_coloss_data.getBlockAnimation().middle_higher_origin.x, ci_coloss_data.getBlockAnimation().middle_higher_origin.y);
	m_vColossBlock[1].animation.setPosition(0.0f, 0.0f);
	m_vColossBlock[1].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossBlock[1].animation.SetTotalFrames(ci_coloss_data.getBlockAnimation().middle_higher_frames);
	m_vColossBlock[1].animation.SetFrameSize((int)ci_coloss_data.getBlockAnimation().middle_higher_size.width, (int)ci_coloss_data.getBlockAnimation().middle_higher_size.height);
	m_vColossBlock[1].animation.AddAnimation(C_ANIM_IDLE, 0, 0);
	m_vColossBlock[1].animation.AddAnimation(C_ANIM_BLOCK, ci_coloss_data.getBlockAnimation().middle_higher_blockStart, ci_coloss_data.getBlockAnimation().middle_higher_blockEnd);
	m_vColossBlock[1].animation.AddAnimation(C_ANIM_PULLBACK, ci_coloss_data.getBlockAnimation().middle_higher_pullbackStart, ci_coloss_data.getBlockAnimation().middle_higher_pullbackEnd);
	m_vColossBlock[1].animation.SetCurrentAnimation(C_ANIM_IDLE);
	m_vColossBlock[1].layer_offset = ci_coloss_data.getMoveAnimation().frontarm_layer_offset; // Frontarm block
	m_vColossBlock[1].animation.setColor(sf::Color(255, 255, 255, 0));

	m_vColossBlock[2].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getBlockAnimation().middle_lower_texture)); // Middle lower
	m_vColossBlock[2].animation.setOrigin(ci_coloss_data.getBlockAnimation().middle_lower_origin.x, ci_coloss_data.getBlockAnimation().middle_lower_origin.y);
	m_vColossBlock[2].animation.setPosition(0.0f, 0.0f);
	m_vColossBlock[2].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossBlock[2].animation.SetTotalFrames(ci_coloss_data.getBlockAnimation().middle_lower_frames);
	m_vColossBlock[2].animation.SetFrameSize((int)ci_coloss_data.getBlockAnimation().middle_lower_size.width, (int)ci_coloss_data.getBlockAnimation().middle_lower_size.height);
	m_vColossBlock[2].animation.AddAnimation(C_ANIM_IDLE, 0, 0);
	m_vColossBlock[2].animation.AddAnimation(C_ANIM_BLOCK, ci_coloss_data.getBlockAnimation().middle_lower_blockStart, ci_coloss_data.getBlockAnimation().middle_lower_blockEnd);
	m_vColossBlock[2].animation.AddAnimation(C_ANIM_PULLBACK, ci_coloss_data.getBlockAnimation().middle_lower_pullbackStart, ci_coloss_data.getBlockAnimation().middle_lower_pullbackEnd);
	m_vColossBlock[2].animation.SetCurrentAnimation(C_ANIM_IDLE);
	m_vColossBlock[2].layer_offset = ci_coloss_data.getMoveAnimation().backarm_layer_offset; // Backarm block
	m_vColossBlock[2].animation.setColor(sf::Color(255, 255, 255, 0));

	m_vColossBlock[3].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getBlockAnimation().bottom_texture)); // Bottom
	m_vColossBlock[3].animation.setOrigin(ci_coloss_data.getBlockAnimation().bottom_origin.x, ci_coloss_data.getBlockAnimation().bottom_origin.y);
	m_vColossBlock[3].animation.setPosition(0.0f, 0.0f);
	m_vColossBlock[3].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossBlock[3].animation.SetTotalFrames(ci_coloss_data.getBlockAnimation().bottom_frames);
	m_vColossBlock[3].animation.SetFrameSize((int)ci_coloss_data.getBlockAnimation().bottom_size.width, (int)ci_coloss_data.getBlockAnimation().bottom_size.height);
	m_vColossBlock[3].animation.AddAnimation(C_ANIM_IDLE, 0, 0);
	m_vColossBlock[3].animation.AddAnimation(C_ANIM_BLOCK, ci_coloss_data.getBlockAnimation().bottom_blockStart, ci_coloss_data.getBlockAnimation().bottom_blockEnd);
	m_vColossBlock[3].animation.AddAnimation(C_ANIM_PULLBACK, ci_coloss_data.getBlockAnimation().bottom_pullbackStart, ci_coloss_data.getBlockAnimation().bottom_pullbackEnd);
	m_vColossBlock[3].animation.SetCurrentAnimation(C_ANIM_IDLE);
	m_vColossBlock[3].layer_offset = ci_coloss_data.getMoveAnimation().tail_layer_offset; // Tail block
	m_vColossBlock[3].animation.setColor(sf::Color(255, 255, 255, 0));

	// Ragemode
	m_vColossRagemode[0].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getRagemodeAnimation().piece0_texture)); // Piece 0
	m_vColossRagemode[0].animation.setPosition(m_fOriginX + ci_coloss_data.getRagemodeAnimation().piece0_offset.x, m_fOriginY + ci_coloss_data.getRagemodeAnimation().piece0_offset.y);
	m_vColossRagemode[0].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossRagemode[0].animation.SetTotalFrames(ci_coloss_data.getRagemodeAnimation().piece0_frames);
	m_vColossRagemode[0].animation.SetFrameSize((int)ci_coloss_data.getRagemodeAnimation().piece0_size.width, (int)ci_coloss_data.getRagemodeAnimation().piece0_size.height);
	m_vColossRagemode[0].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getRagemodeAnimation().piece0_frames - 1);
	m_vColossRagemode[0].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossRagemode[0].layer_offset = ci_coloss_data.getRagemodeAnimation().piece0_layer_offset;
	m_vColossRagemode[0].animation.setColor(sf::Color(255, 255, 255, 0));

	m_vColossRagemode[1].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getRagemodeAnimation().piece1_texture)); // Piece 1
	m_vColossRagemode[1].animation.setPosition(m_fOriginX + ci_coloss_data.getRagemodeAnimation().piece1_offset.x, m_fOriginY + ci_coloss_data.getRagemodeAnimation().piece1_offset.y);
	m_vColossRagemode[1].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossRagemode[1].animation.SetTotalFrames(ci_coloss_data.getRagemodeAnimation().piece1_frames);
	m_vColossRagemode[1].animation.SetFrameSize((int)ci_coloss_data.getRagemodeAnimation().piece1_size.width, (int)ci_coloss_data.getRagemodeAnimation().piece1_size.height);
	m_vColossRagemode[1].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getRagemodeAnimation().piece1_frames - 1);
	m_vColossRagemode[1].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossRagemode[1].layer_offset = ci_coloss_data.getRagemodeAnimation().piece1_layer_offset;
	m_vColossRagemode[1].animation.setColor(sf::Color(255, 255, 255, 0));

	m_vColossRagemode[2].animation.setTexture(*glDataBox->GetTexture(ci_coloss_data.getRagemodeAnimation().piece2_texture)); // Piece 2
	m_vColossRagemode[2].animation.setPosition(m_fOriginX + ci_coloss_data.getRagemodeAnimation().piece2_offset.x, m_fOriginY + ci_coloss_data.getRagemodeAnimation().piece2_offset.y);
	m_vColossRagemode[2].animation.SetFramesPerSecond(fFramesPerSecond);
	m_vColossRagemode[2].animation.SetTotalFrames(ci_coloss_data.getRagemodeAnimation().piece2_frames);
	m_vColossRagemode[2].animation.SetFrameSize((int)ci_coloss_data.getRagemodeAnimation().piece2_size.width, (int)ci_coloss_data.getRagemodeAnimation().piece2_size.height);
	m_vColossRagemode[2].animation.AddAnimation(C_ANIM_MOVE, 0, ci_coloss_data.getRagemodeAnimation().piece2_frames - 1);
	m_vColossRagemode[2].animation.SetCurrentAnimation(C_ANIM_MOVE);
	m_vColossRagemode[2].layer_offset = ci_coloss_data.getRagemodeAnimation().piece2_layer_offset;
	m_vColossRagemode[2].animation.setColor(sf::Color(255, 255, 255, 0));
}

CColoss::position CColoss::getPosition()
{
	position temp;
	temp.x = m_fOriginX;
	temp.y = m_fOriginY;
	return temp;
}

void CColoss::setPosition(float _x, float _y)
{
	m_fOriginX = _x;
	m_fOriginY = _y;

	m_vColossMove[0].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().torso_offset.x + ci_coloss_data.getMoveAnimation().torso_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().torso_offset.y + ci_coloss_data.getMoveAnimation().torso_origin.y);
	m_vColossMove[1].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().backarm_offset.x + ci_coloss_data.getMoveAnimation().backarm_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().backarm_offset.y + ci_coloss_data.getMoveAnimation().backarm_origin.y);
	m_vColossMove[2].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().frontarm_offset.x + ci_coloss_data.getMoveAnimation().frontarm_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().frontarm_offset.y + ci_coloss_data.getMoveAnimation().frontarm_origin.y);
	m_vColossMove[3].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().backleg_offset.x + ci_coloss_data.getMoveAnimation().backleg_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().backleg_offset.y + ci_coloss_data.getMoveAnimation().backleg_origin.y);
	m_vColossMove[4].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().frontleg_offset.x + ci_coloss_data.getMoveAnimation().frontleg_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().frontleg_offset.y + ci_coloss_data.getMoveAnimation().frontleg_origin.y);
	m_vColossMove[5].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().tail_offset.x + ci_coloss_data.getMoveAnimation().tail_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().tail_offset.y + ci_coloss_data.getMoveAnimation().tail_origin.y);

	m_hitableCollision.setPosition(m_vColossMove[0].animation.getPosition().x, m_fOriginY);
}

void CColoss::reset()
{
	modifier_data = glDataBox->GetIniFile("leveldata.ini");
	m_iBPMDuration = modifier_data->GetInt("BpmIncreaseSteps", CLevelData::getCurrentLevelString());
	m_iMultiplierDuration = modifier_data->GetInt("MultiplierIncreaseSteps", CLevelData::getCurrentLevelString());
	m_fHealthIncrease = modifier_data->GetFloat("HealthIncrease", CLevelData::getCurrentLevelString());
	
	m_fHealth = 100.0f;
	CInterface::get()->setCurrentLife(m_fHealth / 100);

	m_iModifier = 1;
	CInterface::get()->setCurrentModifier(m_iModifier);

	m_iBlockedUnits = 0;
	m_iSteps = 0;

	m_fOriginX = ci_coloss_data.getPosition().x;
	m_fOriginY = ci_coloss_data.getPosition().y;

	m_vColossMove[0].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().torso_offset.x + ci_coloss_data.getMoveAnimation().torso_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().torso_offset.y + ci_coloss_data.getMoveAnimation().torso_origin.y);
	m_vColossMove[1].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().backarm_offset.x + ci_coloss_data.getMoveAnimation().backarm_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().backarm_offset.y + ci_coloss_data.getMoveAnimation().backarm_origin.y);
	m_vColossMove[2].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().frontarm_offset.x + ci_coloss_data.getMoveAnimation().frontarm_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().frontarm_offset.y + ci_coloss_data.getMoveAnimation().frontarm_origin.y);
	m_vColossMove[3].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().backleg_offset.x + ci_coloss_data.getMoveAnimation().backleg_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().backleg_offset.y + ci_coloss_data.getMoveAnimation().backleg_origin.y);
	m_vColossMove[4].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().frontleg_offset.x + ci_coloss_data.getMoveAnimation().frontleg_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().frontleg_offset.y + ci_coloss_data.getMoveAnimation().frontleg_origin.y);
	m_vColossMove[5].animation.setPosition(m_fOriginX + ci_coloss_data.getMoveAnimation().tail_offset.x + ci_coloss_data.getMoveAnimation().tail_origin.x, m_fOriginY + ci_coloss_data.getMoveAnimation().tail_offset.y + ci_coloss_data.getMoveAnimation().tail_origin.y);

	m_hitableCollision.setPosition(m_vColossMove[0].animation.getPosition().x, m_fOriginY);

	setImmortal(false);
	m_iBPMIncreaseCount = 0;

	setBoss(false);
	setBossFinished(false);

	text_negative_points.setPosition(960.0f, 540.0f);
	m_fFadeOutText = 0.0f;

	m_fFadeOutFlash = -(255.0f/3.25f);
	bInit = true;
	bFinalPointsAdded = false;

	// Switch to move animation
	for(unsigned int i = 0; i < m_vColossMove.size(); i++)
	{
		m_vColossMove[i].animation.setColor(sf::Color(255, 255, 255, 255));
	}

	disableRagemodeColoss();
}

void CColoss::run()
{
	onHit();
	increaseBPM();
	switchToMove();
	screenShake();
	drawNegativePointsText();
	drawFlash();

	// Draw
	for(unsigned int i = 0; i < m_vColossMove.size(); i++)
	{
		glGraphics->Draw(&m_vColossMove[i].animation, 5 + m_vColossMove[i].layer_offset);
	}

	for(unsigned int i = 0; i < m_vColossBlock.size(); i++)
	{
		glGraphics->Draw(&m_vColossBlock[i].animation, 5 + m_vColossBlock[i].layer_offset);
	}

	for(unsigned int i = 0; i < m_vColossRagemode.size(); i++)
	{
		glGraphics->Draw(&m_vColossRagemode[i].animation, 5 + m_vColossRagemode[i].layer_offset);
	}

	// Check for boss kill
	if(getBossFinished() && !bFinalPointsAdded)
	{
		CInterface::get()->setCurrentScore(CInterface::get()->getCurrentScore() + (CInterface::get()->getCurrentModifier() * 40000));
		bFinalPointsAdded = true;
	}
}

void CColoss::onHit()
{
	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		if(getProjectileVector()[i]->getCollisionObject()->Collides(&m_hitableCollision) && !getProjectileVector()[i]->getDead())
		{
			if(!m_bImmortal)
			{
				modHealth(-getProjectileVector()[i]->getDamage()); // Check damage the projectile deals and subtract it from coloss health
			}

			getProjectileVector()[i]->setBounced(true); // Set bounced to true, so the projectile can only be destroyed
			getProjectileVector()[i]->bounceOrDestroy();

			setModifier(1);
			setBlocked(0);
			setSteps(0);
		}
	}
}

void CColoss::increaseBPM()
{
	if(getSteps() >= m_iBPMDuration)
	{
		m_iBPMIncreaseCount++;
		if(m_iBPMIncreaseCount <= 8)
		{
			CBeatSystem::SetBPM(CBeatSystem::GetBPM() + 5);
		}

		float fCurrentHealth = getHealth();
		if(getHealth() + m_fHealthIncrease > 100.0f)
		{
			setHealth(100.0f);
		}
		else
		{
			modHealth(m_fHealthIncrease);
		}

		setSteps(0);
	}
}

void CColoss::switchToMove()
{
	for(unsigned int i = 0; i < m_vColossBlock.size(); i++)
	{
		if(m_vColossBlock[i].animation.WasPlayed() && m_vColossBlock[i].animation.GetCurrentAnimation() == C_ANIM_BLOCK && !m_vColossBlock[i].hold_animation)
		{
			m_vColossBlock[i].animation.SetFramesPerSecond(ci_coloss_data.getFramesPerSecond());
			m_vColossBlock[i].animation.PlayOnce();
			m_vColossBlock[i].animation.SetCurrentAnimation(C_ANIM_PULLBACK);
		}
		else if(m_vColossBlock[i].animation.WasPlayed() && m_vColossBlock[i].animation.GetCurrentAnimation() == C_ANIM_PULLBACK)
		{
			m_vColossBlock[i].animation.ResetCurrentAnimation();
			
			switch(i)
			{
				case 0:
					m_vColossBlock[i].animation.PlayOnce(false);
					m_vColossBlock[i].animation.SetCurrentAnimation(C_ANIM_IDLE);
					break;
				case 1: // Front arm
					m_vColossBlock[i].animation.setColor(sf::Color(255, 255, 255, 0));
					m_vColossMove[2].animation.setColor(sf::Color(255, 255, 255, 255));
					break;
				case 2: // Back arm
					m_vColossBlock[i].animation.setColor(sf::Color(255, 255, 255, 0));
					m_vColossMove[1].animation.setColor(sf::Color(255, 255, 255, 255));
					break;
				case 3: // Tail
					m_vColossBlock[i].animation.setColor(sf::Color(255, 255, 255, 0));
					m_vColossMove[5].animation.setColor(sf::Color(255, 255, 255, 255));
					break;
			}
		}
	}
}

void CColoss::screenShake()
{
	if((int)m_vColossMove[4].animation.GetCurrentFrame() == 0 || (int)m_vColossMove[3].animation.GetCurrentFrame() == 29)
	{
		glGraphics->ShakeScreen(0.5f, 2, 40.0f);
	}
}

void CColoss::drawNegativePointsText()
{
	glGraphics->Draw(&text_negative_points, 2);

	if(m_fFadeOutText > 0.0f)
	{
		text_negative_points.setColor(sf::Color(0, 0, 0, (int)m_fFadeOutText));
		m_fFadeOutText -= 50.0f * glGraphics->GetLastFrameTime();

		text_negative_points.setPosition(text_negative_points.getPosition().x + 12.0f * glGraphics->GetLastFrameTime(), text_negative_points.getPosition().y - 12.0f * glGraphics->GetLastFrameTime());
	}
	else
	{
		text_negative_points.setColor(sf::Color(0, 0, 0, 0));
		text_negative_points.setPosition(960.0f, 540.0f);
	}
}

void CColoss::enableFlash()
{
	if(!bInit)
	{
		m_fFadeOutFlash = 255.0f;
	}
}

void CColoss::drawFlash()
{
	bInit = false;

	glGraphics->DrawOverlay(&rectScreenFlash, 1000);

	if(m_fFadeOutFlash > -(255.0f/3.25f))
	{
		if(m_fFadeOutFlash > 255.0f - (255.0f/3.25f))
		{
			rectScreenFlash.setFillColor(sf::Color(255, 255, 255, 255));
			m_fFadeOutFlash -= (255.0f/3.25f) * glGraphics->GetLastFrameTime();
		}
		else
		{
			rectScreenFlash.setFillColor(sf::Color(255, 255, 255, (int)(m_fFadeOutFlash + (255.0f/3.25f))));
			m_fFadeOutFlash -= (255.0f/3.25f) * glGraphics->GetLastFrameTime();
		}
	}
	else
	{
		rectScreenFlash.setFillColor(sf::Color(255, 255, 255, 0));
	}
}

void CColoss::disableRagemodeColoss()
{
	for(unsigned int i = 0; i < m_vColossRagemode.size(); i++)
	{
		m_vColossRagemode[i].animation.setColor(sf::Color(255, 255, 255, 0));
	}
}