#include "apex.h"

void TraceLine(const Vector& start, const Vector& end, unsigned int mask, const void* ignore, int collisionGroup, int unk, trace_t* trace)
{
	typedef bool(*FnTraceLine)(const Vector&, const Vector&, unsigned int, const void*, int, int, trace_t*);
	FnTraceLine TraceLine = (FnTraceLine)(Memory::SigScan("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8D 05 ? ? ? ? 4C 89 4C 24 ? 45 33 DB", "r5apex.exe"));
	if (!TraceLine)
		return;

	TraceLine(start, end, mask, ignore, collisionGroup, unk, trace);
}

float GetFov(Angle viewang, Angle aimang)
{
	Vector ang, aim, r, u;

	AngleVectors(viewang, aim, r, u);
	AngleVectors(aimang, ang, r, u);

	return Rad2Deg(acos(aim.Dot(ang) / (aim.Dot(aim))));
}

class Player
{
public:
	float distance;
	CBaseEntity* ent;

	Player(float d, CBaseEntity* e) { distance = d, ent = e; }
};

std::deque<Player> players;

void Sort(CBaseEntity* lp)
{
	for (int i = 1; i < 61; i++)
	{
		auto ent = interfaces.entlist->GetClientEntity(i); 
		if (!ent)
			continue;

		if (ent == lp)
			continue;

		player_info_t info;
		if (!interfaces.engine->GetPlayerInfo(i, &info))
			continue;

		/*trace_t trace;
		Ray_t ray;
		ray.m_Start = lp->GetEyePos();
		ray.m_Delta = (lp->GetEyePos() - ent->GetWorldSpaceCenter());
		CTraceFilter filter;
		filter.pSkip = lp;

		interfaces.enginetrace->TraceRay(ray, 0x640400B, &filter, &trace);
		//TraceLine(lp->GetEyePos(), ent->GetWorldSpaceCenter(), 0x640400B, lp, 0, 0, &trace);
		if (trace.ent != ent)
			continue;*/

		Angle ang;
		VectorAngles(ent->GetWorldSpaceCenter() - lp->GetEyePos(), ang);
		float fov = GetFov(lp->GetAngle(), ang);

		if ((var.aimbot_fov > 0) && (fov > var.aimbot_fov))
			continue;

		float distance = lp->GetEyePos().Distance(ent->GetAbsOrigin());

		if (!players.empty())
		{
			Player best = players.at(0);
			if (best.distance < distance)
				players.push_back(Player(distance, ent));
			else
				players.push_front(Player(distance, ent));
		}
		else
			players.push_back(Player(distance, ent));
	}
}

CBaseEntity* lastbest = nullptr;
void Aimbot::Frame(Renderer* renderer)
{
	if (!var.aimbot_active)
		return;

	float screen_fov = 2 * atan(((float)var.screen_width / (float)var.screen_height) * (3.0f / 4.0f) * tan(Deg2Rad(110/*in-game fov*/ / 2.0f)));
	float radius = std::tan(Deg2Rad(var.aimbot_fov * 0.5f) * 2.f) / screen_fov * (var.screen_width * 0.5f);
	renderer->DrawCircle(ImVec2(var.screen_width / 2, var.screen_height / 2), radius, Color(255, 255, 255, 255));

	if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		lastbest = nullptr;
		return;
	}

	auto lp = interfaces.entlist->GetClientEntity(interfaces.engine->GetLocalPlayerID());
	if (!lp)
		return;

	Sort(lp);

	if (!players.empty())
	{
		auto best = lastbest == nullptr ? players.at(0).ent : lastbest;

		Angle ang;
		VectorAngles(best->GetWorldSpaceCenter() - lp->GetEyePos(), ang);
		lp->SetAngle(ang);

		lastbest = best;

		players.clear();
	}
}

Aimbot aimbot;