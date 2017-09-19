#include "EventLoop.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

int now()
{
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void Vow::then(function<void()> cb)
{
	if (resolved)
	{
		cb();
		return;
	}

	callbacks.push_back(cb);
}

void Vow::resolve()
{
	if (resolved)
		return;

	resolved = true;

	for (int i = 0; i < callbacks.size(); ++i)
		callbacks[i]();
}

EventLoop::EventLoop()
{
	time = now();
}

Vow* EventLoop::setTimeout(int delay, function<void()> cb)
{
	auto vow = new Vow();

	callbacks.push_back([=]()
	{
		if (!vow->resolved)
			cb();

		delete vow;
	});

	callbackTimes.push_back(delay + time);

	return vow;
}

Vow* EventLoop::setInterval(int delay, function<void()> cb)
{
	auto vow = new Vow();

	callbacks.push_back([=]()
	{
		if (vow->resolved)
		{
			delete vow;
			return;
		}

		cb();

		setInterval(delay, cb, vow);
	});

	callbackTimes.push_back(delay + time);

	return vow;
}

Vow* EventLoop::setInterval(int delay, function<void()> cb, Vow* vow)
{
	callbacks.push_back([=]()
	{
		if (vow->resolved)
		{
			delete vow;
			return;
		}

		cb();

		setInterval(delay, cb, vow);
	});

	callbackTimes.push_back(delay + time);

	return vow;
}

Vow* EventLoop::nextTick(function<void()> cb)
{
	auto vow = new Vow();

	callbacks.push_back([=]()
	{
		if (!vow->resolved)
			cb();

		delete vow;
	});

	callbackTimes.push_back(time);

	return vow;
}

void EventLoop::clearInterval(Vow* vow)
{
	vow->resolve();
}

void EventLoop::clearTimeout(Vow* vow)
{
	vow->resolve();
}

void EventLoop::start()
{
	if (running)
		return;

	running = true;

	while (true)
	{
		if (!running)
			break;

		++ticks;

		time = now();

		for (int i = 0; i < callbacks.size(); ++i)
		{
			int cbTime = callbackTimes[i];

			if (time <= cbTime)
				continue;

			callbacks[i]();
			callbacks.erase(callbacks.begin() + i);
			callbackTimes.erase(callbackTimes.begin() + i);

			--i;
		}

		this_thread::sleep_for(milliseconds(1));
	}
}

void EventLoop::stop()
{
	if (!running)
		return;

	running = false;
}