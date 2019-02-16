#pragma once

class CBaseEntity
{
public:
	int GetIndex()
	{
		return *(int*)(this + 0x8);
	}

	Vector GetAbsOrigin()
	{
		return *(Vector*)(this + 0x12C);
	}

	Vector GetWorldSpaceCenter()
	{
		auto vec = GetAbsOrigin();
		vec.z += (GetMins().z + GetMaxs().z) / 2;
		return vec;
	}

	Vector GetMins()
	{
		return *(Vector*)(this + 0x450);
	}

	Vector GetMaxs()
	{
		return *(Vector*)(this + 0x45c);
	}

	int GetHealth()
	{
		return *(int*)(this + 0x3D4);
	}

	Angle GetRotation()
	{
		return *(Angle*)(this + 0x414);
	}

	void SetAngle(Angle ang)
	{
		*(float*)(this + 0x20B8) = ang.x;
		*(float*)(this + 0x20BC) = ang.y;
	}

	Angle GetAngle()
	{
		float x = *(float*)(this + 0x20B8);
		float y = *(float*)(this + 0x20BC);
		return Angle(x, y, 0);
	}

	Vector GetEyePos() // 48 83 EC 38 48 8B 01 48 8D 54 24 20 FF 90
	{
		char v4[64];
		auto v1 = (int *)(*(__int64(__fastcall **)(__int64, char *))(*(uintptr_t *)this + 0x610))((uintptr_t)this, (char*)&v4[0]);
		return *(Vector*)(v1);
	};
};