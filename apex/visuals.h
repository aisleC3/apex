#pragma once

class Visuals
{
public:
	bool Init();
	void Players(Renderer* renderer);
	void Entities(Renderer* renderer);
};

extern Visuals visuals;