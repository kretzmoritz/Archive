#ifndef SV_CONTEXT_H
#define SV_CONTEXT_H

#include "../../window/interfaces/context.h"

class SVContext : public Context
{
public:
	unsigned int getSingularTypeId() const;
	std::string getName() const;

protected:
	bool create(HDC _hDC, HGLRC& _hRC);
};

#endif