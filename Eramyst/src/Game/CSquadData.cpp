#include "..\..\include\Game\CSquadData.h"

//Definition of static variable and vectors
pwIniFile *CSquadData::data;
std::vector<CSquadData::squad> CSquadData::m_vSquads;

void CSquadData::init(std::string file_name)
{
	m_vSquads.clear();
	data = glDataBox->GetIniFile(file_name);
	
	std::stringstream string_typecast;
	std::string input_squad;
	std::string input_order;
	std::string units;
	std::string order_information_string;
	std::string task;
	std::string virtual_information_string;
	std::string input_salve;
	std::string salve_information_string;
	squad temp_squad;
	int temp_int;
	single_order temp_single_order;

	for(int i = 0; i < data->GetInt("squad_count"); i++)
	{
		string_typecast << i;
		string_typecast >> input_squad;
		string_typecast.clear();
		
		m_vSquads.push_back(temp_squad);
		m_vSquads[m_vSquads.size() - 1].iIndex = i;

		virtual_information_string = data->GetString("virtualCount", input_squad);
		std::stringstream ss(virtual_information_string);
		while(ss)
		{
			ss >> temp_int;
			m_vSquads[i].vVirtualCount.push_back(temp_int);

			if (ss.peek() == ',')
			{
				ss.ignore();
			}
		}
		ss.clear();

		std::vector<int> vSalve;
		for(int j = 0; j < data->GetInt("salveCount", input_squad); j++)
		{
			string_typecast << j;
			string_typecast >> input_salve;
			string_typecast.clear();

			input_salve += "salve";
			salve_information_string = data->GetString(input_salve, input_squad);
			ss.str(salve_information_string);
			while(ss)
			{
				ss >> temp_int;
				vSalve.push_back(temp_int);

				if (ss.peek() == ',')
				{
					ss.ignore();
				}
			}
			ss.clear();

			m_vSquads[i].vSalve.push_back(vSalve);
			vSalve.clear();
		}

		units = data->GetString("units", input_squad);
		ss.str(units);
		while(ss)
		{
			ss >> temp_int;
			m_vSquads[i].vID.push_back(temp_int);

			if (ss.peek() == ',')
			{
				ss.ignore();
			}
		}
		ss.clear();

		std::vector<int> order_information;
		order_information_string = data->GetString("orderInformation", input_squad);
		ss.str(order_information_string);
		while(ss)
		{
			ss >> temp_int;
			order_information.push_back(temp_int);

			if (ss.peek() == ',')
			{
				ss.ignore();
			}
		}
		ss.clear();

		for(int j = 0; j < order_information[0]; j++)
		{
			string_typecast << j;
			string_typecast >> input_order;
			
			m_vSquads[i].vAttackOrder.push_back(temp_single_order);
			m_vSquads[i].vAttackOrder[j].number_half_beats = order_information[j + 1];

			task = data->GetString(input_order, input_squad);

			ss.str(task);
			while(ss)
			{
				ss >> temp_int;
				m_vSquads[i].vAttackOrder[j].task.push_back(temp_int);

				if (ss.peek() == ',')
				{
					ss.ignore();
				}
			}
			ss.clear();

			string_typecast.clear();
		}
	}
}

CSquadData::squad CSquadData::getSquad() const
{
	return dataHandler();
}

void CSquadData::setVirtualCount(int _id, int _value, int _BPM, int _level)
{
	switch(_level)
	{
		case 0:
			switch(_BPM)
			{
				case 80:
					m_vSquads[_id].vVirtualCount[0] = _value;
					break;
				case 85:
					m_vSquads[_id].vVirtualCount[1] = _value;
					break;
				case 90:
					m_vSquads[_id].vVirtualCount[2] = _value;
					break;
				case 95:
					m_vSquads[_id].vVirtualCount[3] = _value;
					break;
				case 100:
					m_vSquads[_id].vVirtualCount[4] = _value;
					break;
				case 105:
					m_vSquads[_id].vVirtualCount[5] = _value;
					break;
				case 110:
					m_vSquads[_id].vVirtualCount[6] = _value;
					break;
				case 115:
					m_vSquads[_id].vVirtualCount[7] = _value;
					break;
				case 120:
					m_vSquads[_id].vVirtualCount[8] = _value;
					break;
			}
			break;
		case 1:
			switch(_BPM)
			{
				case 120:
					m_vSquads[_id].vVirtualCount[0] = _value;
					break;
				case 125:
					m_vSquads[_id].vVirtualCount[1] = _value;
					break;
				case 130:
					m_vSquads[_id].vVirtualCount[2] = _value;
					break;
				case 135:
					m_vSquads[_id].vVirtualCount[3] = _value;
					break;
				case 140:
					m_vSquads[_id].vVirtualCount[4] = _value;
					break;
				case 145:
					m_vSquads[_id].vVirtualCount[5] = _value;
					break;
				case 150:
					m_vSquads[_id].vVirtualCount[6] = _value;
					break;
				case 155:
					m_vSquads[_id].vVirtualCount[7] = _value;
					break;
				case 160:
					m_vSquads[_id].vVirtualCount[8] = _value;
					break;
			}
			break;
	}
}

void CSquadData::modVirtualCount(int _id, int _value, int _BPM, int _level)
{
	switch(_level)
	{
		case 0:
			switch(_BPM)
			{
				case 80:
					m_vSquads[_id].vVirtualCount[0] += _value;
					break;
				case 85:
					m_vSquads[_id].vVirtualCount[1] += _value;
					break;
				case 90:
					m_vSquads[_id].vVirtualCount[2] += _value;
					break;
				case 95:
					m_vSquads[_id].vVirtualCount[3] += _value;
					break;
				case 100:
					m_vSquads[_id].vVirtualCount[4] += _value;
					break;
				case 105:
					m_vSquads[_id].vVirtualCount[5] += _value;
					break;
				case 110:
					m_vSquads[_id].vVirtualCount[6] += _value;
					break;
				case 115:
					m_vSquads[_id].vVirtualCount[7] += _value;
					break;
				case 120:
					m_vSquads[_id].vVirtualCount[8] += _value;
					break;
			}
			break;
		case 1:
			switch(_BPM)
			{
				case 120:
					m_vSquads[_id].vVirtualCount[0] += _value;
					break;
				case 125:
					m_vSquads[_id].vVirtualCount[1] += _value;
					break;
				case 130:
					m_vSquads[_id].vVirtualCount[2] += _value;
					break;
				case 135:
					m_vSquads[_id].vVirtualCount[3] += _value;
					break;
				case 140:
					m_vSquads[_id].vVirtualCount[4] += _value;
					break;
				case 145:
					m_vSquads[_id].vVirtualCount[5] += _value;
					break;
				case 150:
					m_vSquads[_id].vVirtualCount[6] += _value;
					break;
				case 155:
					m_vSquads[_id].vVirtualCount[7] += _value;
					break;
				case 160:
					m_vSquads[_id].vVirtualCount[8] += _value;
					break;
			}
			break;
	}
}

int CSquadData::getVirtualCount(int _id, int _BPM, int _level)
{
	switch(_level)
	{
		case 0:
			switch(_BPM)
			{
				case 80:
					return m_vSquads[_id].vVirtualCount[0];
					break;
				case 85:
					return m_vSquads[_id].vVirtualCount[1];
					break;
				case 90:
					return m_vSquads[_id].vVirtualCount[2];
					break;
				case 95:
					return m_vSquads[_id].vVirtualCount[3];
					break;
				case 100:
					return m_vSquads[_id].vVirtualCount[4];
					break;
				case 105:
					return m_vSquads[_id].vVirtualCount[5];
					break;
				case 110:
					return m_vSquads[_id].vVirtualCount[6];
					break;
				case 115:
					return m_vSquads[_id].vVirtualCount[7];
					break;
				case 120:
					return m_vSquads[_id].vVirtualCount[8];
					break;
			}
			break;
		case 1:
			switch(_BPM)
			{
				case 120:
					return m_vSquads[_id].vVirtualCount[0];
					break;
				case 125:
					return m_vSquads[_id].vVirtualCount[1];
					break;
				case 130:
					return m_vSquads[_id].vVirtualCount[2];
					break;
				case 135:
					return m_vSquads[_id].vVirtualCount[3];
					break;
				case 140:
					return m_vSquads[_id].vVirtualCount[4];
					break;
				case 145:
					return m_vSquads[_id].vVirtualCount[5];
					break;
				case 150:
					return m_vSquads[_id].vVirtualCount[6];
					break;
				case 155:
					return m_vSquads[_id].vVirtualCount[7];
					break;
				case 160:
					return m_vSquads[_id].vVirtualCount[8];
					break;
			}
			break;
	}

	return 0;
}

CSquadData::squad CSquadData::dataHandler() const
{
	int random = rand() % m_vSquads.size();
	return m_vSquads[random];
}