#pragma once

#include "CNodeBase.h"

class CNodeHex16 : public CNodeBase
{
public:
	CNodeHex16();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return 2; }

	virtual int Draw(ViewInfo& View, int x, int y);
};