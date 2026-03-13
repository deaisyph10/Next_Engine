#pragma once
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Renderer/SubTexture2D.h"
#include "systems/Camera.h"
#include "renderer/EditorCamera.h"
#include "scene/Component.h"


namespace Exodus
{
	class Renderer2D
	{
	public:
		// Initialization and scene management
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		
		// Flushes the current batch and resets the batch data
		static void Flush();
		static void NextBatch();
		static void StartBatch();

		// Draw quads with POSITION, SIZE, COLOR or TEXTURE
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor = 1.0f);
		
		// Draw quads with TRANSFORM (MAT4) and COLOR or TEXTURE
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingfactor = 1.0f, int entityID = -1);
		
		// Draw quads with POSITION, SIZE, ROTATION and COLOR or TEXTURE
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, float tilingfactor, const Ref<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, float tilingfactor, const Ref<Texture2D>& texture);

		// Draw sprite with TRANSFORM (MAT4) and SpriteRendererComponent
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID = -1);
		
		// Draw circle with TRANSFORM (MAT4) and COLOR
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float Thickness = 1.0f, float Fade = 0.005f, int entityID = -1);
		
		// Draw line with POSITION and COLOR
		static void DrawLine(const glm::vec3& p0,const glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		
		// Draw rect with POSITION, SIZE and COLOR or TRANSFORM (MAT4) and COLOR
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		// REMOVE LATER
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, float tilingfactor, const Ref<SubTexture2D>& subtexture);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, float tilingfactor, const Ref<SubTexture2D>& subtexture);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingfactor = 1.0f, int entityID = -1);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:

	};
}