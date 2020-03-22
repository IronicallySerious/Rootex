#include "text_viewer.h"

#include "editor/editor.h"

#include "resource_loader.h"

#include "imgui.h"

void TextViewer::drawFileInfo()
{
}

ResourceFile* TextViewer::load(const FilePath& filePath)
{
	m_TextResourceFile = ResourceLoader::CreateTextResourceFile(filePath.string());
	return m_TextResourceFile;
}

void TextViewer::unload()
{
}

void TextViewer::draw()
{
	drawFileInfo();
	if (m_TextResourceFile->isDirty())
	{
		ImGui::TextColored(Editor::GetSingleton()->getColors().m_Warning, "File may be changed on disk");
		ImGui::SameLine();
		if (ImGui::Button("Reload"))
		{
			m_TextResourceFile->reload();
		}
	}
	ImGui::Separator();
	ImGui::TextUnformatted(m_TextResourceFile->getString().c_str());
}
