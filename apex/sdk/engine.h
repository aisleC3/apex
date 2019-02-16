#pragma once

struct player_info_t
{
	//char	_0x0000[0x0008];	// 0x0000
	char	name[128];			// 0x0008
	char	_0x0028[0x0228];	// 0x0028
};

class CEngineClient
{
public:
	bool GetPlayerInfo(int index, player_info_t* info)
	{
		return getvfunc<bool(__thiscall*)(void*, int, player_info_t*)>(this, 23)(this, index, info);
	}

	int GetLocalPlayerID()
	{
		return getvfunc<int(__thiscall*)(void*)>(this, 36)(this);
	}

	void SetViewAngles(Vector& ang)
	{
		return getvfunc<void(__thiscall*)(void*, Vector&)>(this, 55)(this, ang);
	}
};