/*************************************************************************
 MGLTimer.h

 Basic timer functionality. Can use either the hires timer or the standard
 Windows timer.

 Author   :   Dave Astle

 Written for More OpenGL Game Programming
*************************************************************************/

#ifndef MGLTIMER_H_INCLUDED
#define MGLTIMER_H_INCLUDED

#include <windows.h>
#ifndef MGL_HIPERFTIMER
  #include <mmsystem.h>
  #pragma comment(lib, "winmm")
#endif

namespace MGL
{
  class Timer
  {
  public:
    Timer() {}
    ~Timer() {}

  /*************************************************************************
  Init()

  If the hi-res timer is present, the tick rate is stored and the function
  returns true. Otherwise, the function returns false, and the timer should
  not be used.
  *************************************************************************/
  bool Init()
  {
  #ifdef MGL_HIPERFTIMER
    if (!QueryPerformanceFrequency(&m_ticksPerSecond))
    {
      // system doesn't support hi-res timer
      return false;
    }
  #endif

    Reset();
    return true;
  } // end Init()


  /*************************************************************************
  GetElapsedSeconds()

  Returns the number of seconds that have elapsed since the last call to
  this function.
  *************************************************************************/
  float GetElapsedSeconds()
  {
  #ifdef MGL_HIPERFTIMER
    LARGE_INTEGER currentTime;

    QueryPerformanceCounter(&currentTime);
    float seconds =  (float)(currentTime.QuadPart - m_lastTimeGES.QuadPart) / (float)m_ticksPerSecond.QuadPart;
  #else
    GLuint currentTime;

    currentTime = timeGetTime();
    float seconds =  (float)(currentTime - m_lastTimeGES) / 1000.0f;
  #endif

    // reset the timer
    m_lastTimeGES = currentTime;

    return seconds;
  } // end GetElapsedSeconds()


  /***************************************************************************
  GetFPS()

  Returns the average frames per second over elapsedFrames, which defaults to
  one. If this is not called every frame, the client should track the number
  of frames itself, and reset the value after this is called.
  ***************************************************************************/
  float GetFPS(unsigned long elapsedFrames = 1)
  {
    m_totalFrames += elapsedFrames;

  #ifdef MGL_HIPERFTIMER
    LARGE_INTEGER currentTime;

    QueryPerformanceCounter(&currentTime);
    float fps = (float)(elapsedFrames * m_ticksPerSecond.QuadPart) / (float)(currentTime.QuadPart - m_lastTimeGFPS.QuadPart);
  #else
    GLuint currentTime;

    currentTime = timeGetTime();
    float fps = (float)(elapsedFrames * 1000) / (float)(currentTime - m_lastTimeGFPS);
  #endif
    
    // reset the timer
    m_lastTimeGFPS = currentTime;

    return fps;
  } // end GetFPS


  /*************************************************************************
  GetAggregateFPS()

  Returns the average fps since the last time Reset() was called. Note that
  this is meant to be used in conjunction with LockFPS or GetFPS.
  Otherwise, the function has no way of knowing how many frames have
  actually elapsed.
  *************************************************************************/
  float GetAggregateFPS()
  {
  #ifdef MGL_HIPERFTIMER
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (float)(m_totalFrames * m_ticksPerSecond.QuadPart) / (float)(currentTime.QuadPart - m_startTime.QuadPart);
  #else
    GLuint currentTime;
    currentTime = timeGetTime();
    return (float)(m_totalFrames * 1000) / (float)(currentTime - m_startTime);
  #endif
  } // end GetAggregateFPS()


  /*************************************************************************
  LockFPS()

  Used to lock the frame rate to a set amount. This will block until enough
  time has passed to ensure that the fps won't go over the requested
  amount. Note that this can only keep the fps from going above the
  specified level; it can still drop below it. It is assumed that if used,
  this function will be called every frame. The value returned is the
  instantaneous fps, which will be <= targetFPS.
  *************************************************************************/
  float LockFPS(unsigned char targetFPS)
  {
    if (targetFPS == 0)
      targetFPS = 1;

    m_totalFrames++;

    float   fps;
  #ifdef MGL_HIPERFTIMER
    LARGE_INTEGER currentTime;

    // delay to maintain a constant frame rate
    do {
      QueryPerformanceCounter(&currentTime);
      fps = (float)m_ticksPerSecond.QuadPart/(float)(currentTime.QuadPart - m_lastTimeLFPS.QuadPart);
    } while (fps > (float)targetFPS);
  #else
    GLuint currentTime;

    // delay to maintain a constant frame rate
    do {
      currentTime = timeGetTime();
      fps = (float)1000/(float)(currentTime - m_lastTimeLFPS);
    } while (fps > (float)targetFPS);
  #endif
    // reset the timer
    m_lastTimeLFPS = m_startTime;

    return fps;
  } // end LockFPS()


  /*************************************************************************
  Reset()

  Restarts the timer
  *************************************************************************/
  void Reset()
  {
  #ifdef MGL_HIPERFTIMER
    QueryPerformanceCounter(&m_startTime);
  #else
    m_startTime = timeGetTime();
  #endif
    m_lastTimeGES = m_lastTimeGFPS = m_lastTimeLFPS = m_startTime;
    m_totalFrames = 0;
  }


  private:
  #ifdef MGL_HIPERFTIMER
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_ticksPerSecond;
    LARGE_INTEGER m_lastTimeGES;
    LARGE_INTEGER m_lastTimeGFPS;
    LARGE_INTEGER m_lastTimeLFPS;
  #else
    GLuint  m_startTime;
    GLuint  m_lastTimeGES;
    GLuint  m_lastTimeGFPS;
    GLuint  m_lastTimeLFPS;
  #endif
    unsigned long m_totalFrames;
  };

}

#endif // MGLTIMER_H_INCLUDED
