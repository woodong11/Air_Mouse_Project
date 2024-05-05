#include "serial.h"
#include <stdio.h>
int main()
{
    //serial init
    Serial* serial = new Serial("COM9", 115200);
    if (!serial->isConnected()) return 0;

    char buf[255];
    while (1) {
        serial->readLine(buf, 255);

        short ax, ay, az, cmd;

        //파싱에 성공한 변수 return
        int parsed = sscanf(buf, "%d,%d,%d,%d", &ax, &ay, &az, &cmd);
        //if (parsed != 4) {
        //    continue;  // If parsing failed or incomplete, skip to the next iteration
        //}



        int off_x = -(ax / 1000) * 3;
        int off_y = (ay / 1000) * 3;

        printf("mouse position: %d, %d, %d, %d\n", off_x, off_y, az, cmd);

        if (az == 1) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 10, 10, 0, 0);	//왼쪽 누르기
            Sleep(100);
            mouse_event(MOUSEEVENTF_LEFTUP, 10, 10, 0, 0);		//왼쪽 떼기
            Sleep(500);
        }
        

        else if (az == 2) {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 10, 10, 0, 0);	//오른쪽 누르기
            Sleep(100);
            mouse_event(MOUSEEVENTF_RIGHTUP, 10, 10, 0, 0);		//오른쪽 떼기
            Sleep(500);
        }

        


        POINT p;
        GetCursorPos(&p);
        SetCursorPos(p.x + off_x, p.y + off_y);

    }
    return 0;
}
