#include "../include/controller.hpp"

classControl::classControl() { fd = open("/dev/input/js0", O_RDONLY); }

void classControl::update() {
    read(fd, &e, sizeof(e));
    
    
    
    
   /* 
    switch (e.type)
    {
        case 1:
            switch (e.number)
            {
                case 0:
                    a=e.value;
                    break;
                case 1:
                    b=e.value;
                    break;
                case 2:
                    x=e.value;
                    break;            
                case 3:
                    y=e.value;
                    break;            
                case 4:
                    lb=e.value;
                    break;            
                case 5:
                    rb=e.value;
                    break;            
                case 6:
                    back=e.value;
                    break;            
                case 7:
                    start=e.value;
                    break;            
                case 8:
                    home=e.value;
                    break;            
                case 9:
                    lJb=e.value;
                    break;            
                case 10:
                    rJb=e.value;
                    break;            
            }
            break;
        case 2:
            switch (e.number)
            {
                case 0:
                    lJoyX = e.value;
                    break;
                case 1:
                    lJoyY = e.value;
                    break;
                case 2:
                    rJoyX = e.value;
                    break;
                case 3:
                    rJoyY = e.value;
                    break;
                case 4:
                    rTrig = e.value;
                    break;
                case 5:
                    lTrig = e.value;
                    break;
                case 6:
                    dLeftRight = e.value;
                    break;
                case 7:
                    dUpDown = e.value;
                    break;
            }
            break;
        
    }
    */
}


void classControl::printInput() {
    int id = (e.number | e.type<<4);
    
    printf("Value: %8d Type: %#4x Number: %#4x ID: %2d \n",e.value,e.type,e.number,id);
}