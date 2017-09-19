# EventLoop | A Javascript style event loop for C++11

### Getting started
This C++11 library allows developers to create Javascript style asynchronous event loops. To use this library in your project, simply drop "EventLoop.cpp" and "EventLoop.h" into your source folder.

```
#include <iostream>
#include "EventLoop.h"

using namespace std;

int main()
{
    auto loop = EventLoop();

    //create an interval that executes once every 33 milliseconds, or 30 frame per second
    auto interval = loop.setInterval(33, [&]()
    {
        cout << loop.ticks << endl;
    });

    //clear the above interval after 2 seconds
    auto timeout = loop.setTimeout(2000, [&]()
    {
        loop.clearInterval(interval);
        
        cout << "interval cleared" << endl;
    });
    
    //print "hello world" after 3 seconds
    auto timeout2 = loop.setTimeout(3000, [&]()
    {
        cout << "hello world" << endl;
    });
    
    //cancel the above timeout when event loop hits the next tick, so that nothing gets printed;
    auto timeout3 = loop.nextTick([&]()
    {
        loop.clearTimeout(timeout2);
        
        //alternatively, you can also do this
        //timeout2->resolve();
    });

    loop.start();

    return 0;
}
```
