#pragma once

class Menu
{
public:
	bool is_open = true;

	void Init();
	void Render();
	void AimbotTab();
	void VisualsTab();
};

extern Menu menu;