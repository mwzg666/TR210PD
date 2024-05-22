#include "main.h"
#include "led.h"


// °åÔØÖ¸Ê¾µÆ
void RunLed(u16 dt)
{   
    static u16 tm = 0;
    u16 to = 5000;
    tm += dt;

    if (tm > to)
    {
        tm = 0;
        RUN_LED(0);
    }
    else if (tm > (to-100))
    {
        RUN_LED(1);
    }
}

