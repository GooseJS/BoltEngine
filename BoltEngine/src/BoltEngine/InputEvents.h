#pragma once

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"
#include "BoltEngine/event/IEvent.h"

namespace Bolt
{
	struct KeyboardKeyEvent : public IEvent
	{
	private:
		const int _key;
		const int _scancode;
		const int _mods;
		const int _action;
	public:
		KeyboardKeyEvent(int key, int scancode, int mods, int action) : _key(key), _scancode(scancode), _mods(mods), _action(action) {}
		int key() const { return _key; }
		int scancode() const { return _scancode; }
		int mods() const { return _mods; }
		int action() const { return _action; }
	};

	struct EventKeyPressed : public KeyboardKeyEvent
	{
	public:
		EventKeyPressed(int key, int scancode, int mods) : KeyboardKeyEvent(key, scancode, mods, GLFW_PRESS) {}
	};

	struct EventKeyReleased : public KeyboardKeyEvent
	{
	public:
		EventKeyReleased(int key, int scancode, int mods) : KeyboardKeyEvent(key, scancode, mods, GLFW_RELEASE) {}
	};

	struct EventKeyRepeat : public KeyboardKeyEvent
	{
	public:
		EventKeyRepeat(int key, int scancode, int mods) : KeyboardKeyEvent(key, scancode, mods, GLFW_REPEAT) {}
	};

	struct MouseButtonEvent : public IEvent
	{
	private:
		const int _button;
		const float _x;
		const float _y;
		const int _action;
	public:
		MouseButtonEvent(int button, float x, float y, int action) : _button(button), _x(x), _y(y), _action(action) {}
		int button() const { return _button; }
		float x() const { return _x; }
		float y() const { return _y; }
		int action() const { return _action; }
	};

	struct EventMouseButtonPressed : public MouseButtonEvent
	{
	public:
		EventMouseButtonPressed(int button, float x, float y) : MouseButtonEvent(button, x, y, GLFW_PRESS) {}
	};

	struct EventMouseButtonReleased : public MouseButtonEvent
	{
	public:
		EventMouseButtonReleased(int button, float x, float y) : MouseButtonEvent(button, x, y, GLFW_RELEASE) {}
	};

	struct EventMouseMove : public IEvent
	{
	private:
		const float _x;
		const float _y;
		const float _deltaX;
		const float _deltaY;
	public:
		EventMouseMove(float x, float y, float deltaX, float deltaY) : _x(x), _y(y), _deltaX(deltaX), _deltaY(deltaY) {}
		float x() const { return _x; }
		float y() const { return _y; }
		float deltaX() const { return _deltaX; }
		float deltaY() const { return _deltaY; }
	};

	struct EventMouseScroll : public IEvent
	{
	private:
		const float _x;
		const float _y;
	public:
		EventMouseScroll(float x, float y) : _x(x), _y(y) {}
		float x() const { return _x; }
		float y() const { return _y; }
	};
}