#ifndef QPCTIMER_H
#define QPCTIMER_H

#if defined(WIN32)
#include <windows.h>

class QPCTimer
{
private:
   LARGE_INTEGER iFreq;
   LARGE_INTEGER iBegin, iEnd;
public:
   QPCTimer() {}

   void start()
   {
      QueryPerformanceFrequency(&iFreq);
      QueryPerformanceCounter(&iBegin);
   }

   double elapsedMs()
   {
      QueryPerformanceCounter(&iEnd);
      return (static_cast<double>((iEnd.QuadPart-iBegin.QuadPart))/iFreq.QuadPart)*1000;
   }
};

#else
#error QPCTimer nur für Microsoft Windows verfügbar!
#endif //win32
#endif //QPCTIMER_H

