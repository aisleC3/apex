#pragma once

class Renderer
{
public:
	void Initialize();

	void BeginScene();
	void EndScene();

	void FillRGBA(const ImVec2& pos1, const ImVec2& pos2, Color color);
	float DrawString(ImFont* pFont, const std::string& text, const ImVec2& position, float size, Color color, bool center = false);
	void DrawLine(const ImVec2& from, const ImVec2& to, Color color, float thickness = 1.0f);
	void DrawCircle(const ImVec2& position, float radius, Color color, float thickness = 1.0f);
	void DrawCircleFilled(const ImVec2& position, float radius, Color color);

	ImFont* m_pFont;

	static Renderer* m_pInstance;
};