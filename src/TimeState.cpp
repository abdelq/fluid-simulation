#include "TimeState.h"

TimeState::TimeState()
    : _time( 0 )
    , _deltaTime( 0 )
{
    _timer.start();
}

void TimeState::newFrame()
{
    _deltaTime = _timer.elapsed() / 1000.0;
    _time += _deltaTime;
    _timer.restart();
}

float TimeState::time() const
{
    return _time;
}

float TimeState::deltaTime() const
{
    return _deltaTime;
}
