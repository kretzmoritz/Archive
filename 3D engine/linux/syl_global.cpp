#include "syl_global.h"






void sTime::perSecondTick(int & fpsCountArg, float & spacestationRotateYArg)
{
    if (clock() >= countSecond)
    {
        saveFPS = fpsCountArg;
        fpsCountArg = 0;
        countSecond = clock() + 1 * CLOCKS_PER_SEC;
    }

    if (clock() >= rotationTick)
    {
        spacestationRotateYArg++;
        rotationTick = clock() + 0.025 * CLOCKS_PER_SEC;
    }
}
