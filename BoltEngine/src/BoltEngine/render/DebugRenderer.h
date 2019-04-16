#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <debug_draw.hpp>

#include "BoltEngine/Core.h"
#include "BoltEngine/logger/Logger.h"
#include "BoltEngine/render/Shader.h"
#include "BoltEngine/render/Camera.h"

namespace Bolt
{
	class BoltDebugDrawInterface : public dd::RenderInterface
	{
	private:
		Shader _shapeShader;
		Shader _outlineShader;

		GLuint _outlineVAO;
		GLuint _outlineVBO;

		Camera* _camera;
	public:
		void setup();

		virtual void drawPointList(const dd::DrawVertex* points, int count, bool depthEnabled) override;
		virtual void drawLineList(const dd::DrawVertex* lines, int count, bool depthEnabled) override;

		inline void setRenderCam(Camera* camera) { _camera = camera; }
	};
}