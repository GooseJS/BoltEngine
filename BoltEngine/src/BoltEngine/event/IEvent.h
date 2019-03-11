#pragma once

namespace Bolt
{
	struct IEvent
	{
	private:
		bool _handled = false;
	public:
		void handle(bool handled) { _handled = handled; }
		bool handled() const { return _handled; }
		IEvent() {} ;
		virtual ~IEvent() {}
	};

	struct IEventCancellable : public IEvent
	{
	private:
		bool _cancelled = false;
	public:
		void cancel(bool cancelled) { _cancelled = cancelled; }
		bool cancelled() const { return _cancelled; }
		IEventCancellable() {}
		virtual ~IEventCancellable() {}
	};
}
