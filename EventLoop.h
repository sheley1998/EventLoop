#pragma once

#include <vector>
#include <functional>

class Vow
{
private:
	std::vector<std::function<void()>> callbacks;

public:
	bool resolved = false;

	void then(std::function<void()> cb);

	void resolve();
};

class EventLoop
{
private:
	bool running = false;
	std::vector<std::function<void()>> callbacks;
	std::vector<int> callbackTimes;

public:
	EventLoop();

	int time = 0;
	int ticks = 0;

	Vow* setTimeout(int delay, std::function<void()> cb);

	Vow* setInterval(int delay, std::function<void()> cb);

	Vow* setInterval(int delay, std::function<void()> cb, Vow* vow);

	Vow* nextTick(std::function<void()> cb);

	void clearInterval(Vow* vow);

	void clearTimeout(Vow* vow);

	void start();

	void stop();
};