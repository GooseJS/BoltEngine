#define DEBUG_DRAW_IMPLEMENTATION
#include "DebugRenderer.h"

namespace Bolt
{
	void BoltDebugDrawInterface::setup()
	{
		std::string vertexShaderSource =
			"#version 150\n"
			"\n"
			"in vec3 in_Position;\n"
			"in vec4 in_ColorPointSize;\n"
			"out vec4 v_Color;\n"
			"uniform mat4 mvpMatrix;\n"
			"void main()\n"
			"{\n"
			"	gl_Position		= mvpMatrix * vec4(in_Position, 1.0);\n"
			"	gl_PointSize	= in_ColorPointSize.w;\n"
			"	v_Color			= vec4(in_ColorPointSize.xyz, 1.0);\n"
			"}\n";
		std::string fragmentShaderSource =
			"#version 150\n"
			"\n"
			"in vec4 v_Color;\n"
			"out vec4 out_FragColor;\n"
			"void main()\n"
			"{\n"
			"	out_FragColor = v_Color;\n"
			"}\n";

		_outlineShader.initFromString(vertexShaderSource, fragmentShaderSource);

		glEnable(GL_PROGRAM_POINT_SIZE);

		glGenVertexArrays(1, &_outlineVAO);
		glBindVertexArray(_outlineVAO);
		glGenBuffers(1, &_outlineVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _outlineVBO);

		glBufferData(GL_ARRAY_BUFFER, DEBUG_DRAW_VERTEX_BUFFER_SIZE * sizeof(dd::DrawVertex), nullptr, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(dd::DrawVertex), nullptr);

		std::size_t offset = sizeof(float) * 3;

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(dd::DrawVertex), (void*)offsetof(dd::DrawVertex, point.r));

		_outlineShader.use();
		_outlineShader.bindAttribLocation(0, "in_Position");
		_outlineShader.bindAttribLocation(1, "in_ColorPointSize");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void BoltDebugDrawInterface::drawPointList(const dd::DrawVertex* points, int count, bool depthEnabled)
	{
		if (points == nullptr || count <= 0 || count > DEBUG_DRAW_VERTEX_BUFFER_SIZE)
			return;

		glBindVertexArray(_outlineVAO);
		_shapeShader.use();

		_shapeShader.uniform("mvpMatrix", _camera->getProjectionMatrix() * _camera->getViewMatrix());

		if (depthEnabled) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, _outlineVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(dd::DrawVertex), points);

		glDrawArrays(GL_POINTS, 0, count);
	}

	void BoltDebugDrawInterface::drawLineList(const dd::DrawVertex* lines, int count, bool depthEnabled)
	{
		if (lines == nullptr || count <= 0 || count > DEBUG_DRAW_VERTEX_BUFFER_SIZE)
			return;

		glBindVertexArray(_outlineVAO);
		_outlineShader.use();

		_outlineShader.uniform("mvpMatrix", _camera->getProjectionMatrix() * _camera->getViewMatrix());

		if (depthEnabled) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, _outlineVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(dd::DrawVertex), lines);

		glDrawArrays(GL_LINES, 0, count);
	}
}