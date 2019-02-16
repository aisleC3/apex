#include "../apex.h"

Renderer* Renderer::m_pInstance;

void Renderer::Initialize()
{
	ImGuiIO& io = ImGui::GetIO();

	// This is my example of font initializing
	// First font used for UI
	// Second for rendering, 32.0f is pixel size, not font size.
	//io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	//m_pFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 32.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	m_pFont = io.Fonts->AddFontDefault();
}

void Renderer::BeginScene()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("BackBuffer", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);
}

void Renderer::EndScene()
{
	ImGui::GetWindowDrawList()->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
}


void Renderer::FillRGBA(const ImVec2& pos1, const ImVec2& pos2, Color color)
{
	DWORD clr = (DWORD)((((color.a()) & 0xff) << 24) | (((color.b()) & 0xff) << 16) | (((color.g()) & 0xff) << 8) | ((color.r()) & 0xff));

	ImGui::GetWindowDrawList()->AddRectFilled(pos1, { pos1.x + pos2.x, pos1.y + pos2.y }, clr);
}

float Renderer::DrawString(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, Color color, bool center)
{
	DWORD clr = (DWORD)((((color.a()) & 0xff) << 24) | (((color.b()) & 0xff) << 16) | (((color.g()) & 0xff) << 8) | ((color.r()) & 0xff));

	ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), ImGui::GetWindowFontSize(), ImVec2(pos.x, pos.y), clr, text.c_str(), 0, 0.0f, 0);

	/*std::stringstream steam(text);
	std::string line;

	while (std::getline(steam, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());

			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y), clr, line.c_str());
		}
		else
		{
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());
			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, color.a())), line.c_str());

			ImGui::GetWindowDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y), clr, line.c_str());
		}
	}*/

	return 0;
}

void Renderer::DrawLine(const ImVec2& from, const ImVec2& to, Color color, float thickness)
{
	DWORD clr = (DWORD)((((color.a()) & 0xff) << 24) | (((color.b()) & 0xff) << 16) | (((color.g()) & 0xff) << 8) | ((color.r()) & 0xff));

	ImGui::GetWindowDrawList()->AddLine(from, to, clr, thickness);
}

void Renderer::DrawCircle(const ImVec2& position, float radius, Color color, float thickness)
{
	DWORD clr = (DWORD)((((color.a()) & 0xff) << 24) | (((color.b()) & 0xff) << 16) | (((color.g()) & 0xff) << 8) | ((color.r()) & 0xff));

	ImGui::GetWindowDrawList()->AddCircle(position, radius, clr, 25, thickness);
}

void Renderer::DrawCircleFilled(const ImVec2& position, float radius, Color color)
{
	DWORD clr = (DWORD)((((color.a()) & 0xff) << 24) | (((color.b()) & 0xff) << 16) | (((color.g()) & 0xff) << 8) | ((color.r()) & 0xff));

	ImGui::GetWindowDrawList()->AddCircleFilled(position, radius, clr, 12);
}