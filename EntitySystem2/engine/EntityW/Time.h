#pragma once

namespace EntityW {
	class Time
	{
		long time;
	public:
		Time(long time) : time(time) {};
		long asMicroseconds() { return time; }
		int asMilliseconds() { return time / 1000; }
		float asSeconds() {
			return (float)time / 1000000.f;
		}
	};

}