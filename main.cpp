#include "mbed.h"
#include "ultrasonic.h"
 
 void dist(int distance)
{
    //put code here to happen when the distance is changed
    printf("Distance changed to %dmm\r\n", distance);
}
 
ultrasonic mu(D8, D9, .1, 1, &dist);    //Set the trigger pin to D8 and the echo pin to D9
                                        //have updates every .1 seconds and a timeout after 1
                                        //second, and call dist when the distance changes
 
int main()
{
    mu.startUpdates();//start mesuring the distance
    while(1)
    {
        //Do something else here
        mu.checkDistance();     //call checkDistance() as much as possible, as this is where
                                //the class checks if dist needs to be called.
    }
}