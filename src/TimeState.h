#ifndef TIMESTATE_H
#define TIMESTATE_H

#include <QTime>

/* TimeState contains the information about the time (in seconds)
 * for the current frame. deltaTime is the difference
 * in time between the current and the previous frame.
 */

class TimeState
{
public:
    TimeState();

    void newFrame();
    float time() const;
    float deltaTime() const;

private:
    QTime _timer;
    float _time;
    float _deltaTime;
};

#endif // TIMESTATE_H
