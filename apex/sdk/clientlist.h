#pragma once

class CClientEntityList
{
public:
	CBaseEntity* GetClientEntity(int index)
	{
		return getvfunc<CBaseEntity*(__thiscall*)(void*, int)>(this, 4)(this, index);
	}

	int NumberOfEntites()
	{
		return getvfunc<int(__thiscall*)(void*, bool)>(this, 6)(this, true);
	}
};