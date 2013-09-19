////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/10/2012
// System implementation of squad data return class
////////////////////////////////////////////////////////////

#ifndef SQUADDATA
#define SQUADDATA

#define PWUSEGLOBALSINGLETONS

#include "../pwDataBox.hpp"
#include <string>
#include <sstream>
#include <vector>

class CSquadData
{
	public:
		struct single_order
		{
			float fTaskTime;
			int number_half_beats;
			std::vector<int> task;
		};

		struct squad
		{
			int iIndex;
			std::vector<int> vVirtualCount;
			std::vector< std::vector<int> > vSalve;
			std::vector<int> vID;
			std::vector<single_order> vAttackOrder;
		};

		CSquadData() {};
		~CSquadData() {};

		static void init(std::string file_name);
		squad getSquad() const;

		static void setVirtualCount(int _id, int _value, int _BPM, int _level);
		static void modVirtualCount(int _id, int _value, int _BPM, int _level);
		static int getVirtualCount(int _id, int _BPM, int _level);

	private:
		squad dataHandler() const;

		static pwIniFile *data;
		static std::vector<squad> m_vSquads;
};

#endif