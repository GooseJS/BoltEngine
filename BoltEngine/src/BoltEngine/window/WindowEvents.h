#pragma once

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"
#include "BoltEngine/event/IEvent.h"

namespace Bolt
{
	struct EventWindowOpen : public IEvent
	{
	public:
		EventWindowOpen() {}
	};

	struct EventWindowResize : public IEvent
	{
		int _newWidth;
		int _newHeight;
	public:
		EventWindowResize(int newWidth, int newHeight) : _newWidth(newWidth), _newHeight(newHeight) {}
		inline int newWidth() const { return _newWidth; }
		inline int newHeight() const { return _newHeight; }
	};

	struct EventWindowClose : public IEvent
	{
	public:
		EventWindowClose() {}
	};
}
