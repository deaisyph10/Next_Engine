#pragma once
#include "core.h"
#include "vendor/ImGUI/imgui.h"
#include <filesystem>
#include "renderer/Texture.h"

namespace Exodus
{
	extern const std::filesystem::path AssetPath;

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}