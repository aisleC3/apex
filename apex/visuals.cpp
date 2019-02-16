#include "apex.h"

bool WorldToScreen(Vector in, Vector& out)
{
	typedef bool(*FnWorldToScreen)(Vector& in, Vector& out);
	FnWorldToScreen WorldToScreen = (FnWorldToScreen)(interfaces.game_base + 0x634160);//(Memory::SigScan("4C 8B DC 53 56 57 48 83 EC 70", "r5apex.exe")); //E8 ? ? ? ? 84 C0 75 08 0F 57 C0 E9 ? ? ? ? 48 8B 0D ? ? ? ?
	if (!WorldToScreen)
		return false;

	return WorldToScreen(in, out);
}

struct edge
{
	edge(Vector s, Vector e) { start = s, end = e; }
	Vector start, end;
};
bool DrawBox(Renderer* renderer, CBaseEntity* ent, Color color, bool outline = false)
{
	Vector pos = ent->GetAbsOrigin();
	Angle  ang = ent->GetRotation();

	Vector mins = ent->GetMins();
	Vector maxs = ent->GetMaxs();

	std::list<edge> edges = {
		//top
		edge(maxs, Vector(maxs.x, mins.y, maxs.z)), // 1
		edge(Vector(maxs.x, mins.y, maxs.z), Vector(mins.x, mins.y, maxs.z)), // 2
		edge(Vector(mins.x, mins.y, maxs.z), Vector(mins.x, maxs.y, maxs.z)), // 3
		edge(Vector(Vector(mins.x, maxs.y, maxs.z)), maxs), // 4

		//mid
		edge(maxs, Vector(maxs.x, maxs.y, mins.z)), // 1
		edge(Vector(maxs.x, mins.y, maxs.z), Vector(maxs.x, mins.y, mins.z)), // 2
		edge(Vector(mins.x, mins.y, maxs.z), Vector(mins.x, mins.y, mins.z)), // 3
		edge(Vector(Vector(mins.x, maxs.y, maxs.z)), Vector(mins.x, maxs.y, mins.z)), // 4

		//bottom
		edge(Vector(maxs.x, maxs.y, mins.z), Vector(maxs.x, mins.y, mins.z)), // 1
		edge(Vector(maxs.x, mins.y, mins.z), Vector(mins.x, mins.y, mins.z)), // 2
		edge(Vector(mins.x, mins.y, mins.z), Vector(mins.x, maxs.y, mins.z)), // 3
		edge(Vector(Vector(mins.x, maxs.y, mins.z)), Vector(maxs.x, maxs.y, mins.z)), // 4
	};

	for (const auto& edge : edges)
	{
		Vector start, end;
		if (WorldToScreen(ent->GetAbsOrigin() + edge.start, start) && WorldToScreen(ent->GetAbsOrigin() + edge.end, end))
		{
			if (outline)
				renderer->DrawLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), Color(0, 0, 0, 255), 3);

			renderer->DrawLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), color);
		}
	}
}

void GetBoundingBox(CBaseEntity* ent, int& minx, int& miny, int& maxx, int& maxy)
{
	int w, h;
	w = var.screen_width, h = var.screen_height;

	VMatrix trans;
	AngleMatrix(ent->GetRotation(), trans);
	MatrixSetColumn(ent->GetAbsOrigin(), 3, trans);

	Vector min = ent->GetMins();
	Vector max = ent->GetMaxs();

	Vector corners[8] =
	{
		Vector(min.x, min.y, min.z),
		Vector(min.x, min.y, max.z),
		Vector(min.x, max.y, min.z),
		Vector(min.x, max.y, max.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, min.y, max.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, max.y, max.z)
	};

	minx = w * 2;
	miny = h * 2;
	maxx = -9999;
	maxy = -9999;

	Vector screen[8];

	for (int i = 0; i < 8; i++)
	{
		VectorTransform(corners[i], trans, corners[i]);

		if (WorldToScreen(corners[i], screen[i]))
		{
			minx = min(minx, screen[i].x);
			miny = min(miny, screen[i].y);
			maxx = max(maxx, screen[i].x);
			maxy = max(maxy, screen[i].y);
		}
	}

	maxx -= minx;
	maxy -= miny;
}

void DrawOutlinedRect(Renderer* renderer, int x, int y, int w, int h, Color color, int thickness = 1)
{
	renderer->DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, thickness);
	renderer->DrawLine(ImVec2(x + w, y), ImVec2(x + w, y + h), color, thickness);
	renderer->DrawLine(ImVec2(x + w, y + h), ImVec2(x, y + h), color, thickness);
	renderer->DrawLine(ImVec2(x, y + h), ImVec2(x, y), color, thickness);
}

void Visuals::Players(Renderer* renderer)
{
	if (!var.visuals_players_active)
		return;

	for (int i = 1; i < 61; i++)
	{
		CBaseEntity* ent = interfaces.entlist->GetClientEntity(i);
		if (!ent)
			continue;

		if (i == interfaces.engine->GetLocalPlayerID())
			continue;

		player_info_t info;
		if (!interfaces.engine->GetPlayerInfo(i, &info))
			continue;

		int x, y, w, h;
		GetBoundingBox(ent, x, y, w, h);

		if (var.visuals_players_box)
		{
			if (!var.visuals_players_box_3d)
			{
				if (var.visuals_players_box_outline)
					DrawOutlinedRect(renderer, x, y, w, h, Color(0, 0, 0, 255), 3);

				DrawOutlinedRect(renderer, x, y, w, h, Color(0, 191, 255, 255));
			}
			else
				DrawBox(renderer, ent, Color(0, 191, 255, 255), var.visuals_players_box_outline);
		}

		Vector screen;
		if (WorldToScreen(ent->GetAbsOrigin(), screen))
		{
			if(var.visuals_players_name)
				renderer->DrawString(renderer->m_pFont, info.name, ImVec2(screen.x, screen.y), 13, Color(255, 255, 255, 255));

			if (var.visuals_players_snapline)
				renderer->DrawLine(ImVec2(var.screen_width / 2, var.screen_height), ImVec2(screen.x, screen.y), Color(255, 255, 255, 255));
		}

		if (var.visuals_players_hpbar)
		{
			int health = max(0, min(ent->GetHealth(), 100));
			float health_precent = ceil((((double)ent->GetHealth()) / max(100, ent->GetHealth())) * 100);
			int height = (h * health_precent) / 100;
			Color health_color = Color(min((510 * (100 - health_precent)) / 100, 255), min((510 * health_precent) / 100, 255), 0, 232);

			renderer->FillRGBA(ImVec2(x - 6, y - 1), ImVec2(4, h + 2), Color(10, 10, 10, 115));
			renderer->FillRGBA(ImVec2(x - 5, y + (h - height)), ImVec2(2, height), health_color);
		}
	}
}

void Visuals::Entities(Renderer* renderer)
{

}

Visuals visuals;