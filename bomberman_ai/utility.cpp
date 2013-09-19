#include "utility.h"

void Utility::update(CLocalState _local, CGameState _state)
{
	m_local = _local;
	m_state = _state;
}

position Utility::getPlayerPosition(int _id)
{
	position result(-1, -1); //Player not found
	
	if(m_state.m_UnitManager.GetPlayerUnits(_id, UNIT_TYPE_HERO).size() > 0)
	{
		result.x = m_state.m_UnitManager.GetPlayerUnits(_id, UNIT_TYPE_HERO)[0].m_PosX;
		result.y = m_state.m_UnitManager.GetPlayerUnits(_id, UNIT_TYPE_HERO)[0].m_PosY;
	}

	return result;
}
