#include "mbed.h"
#include "QEI.h" //[https://github.com/yuina-blend/QEI]にライブラリがあります

QEI encoder( D5, D6, NC, 2048, QEI::X2_ENCODING);
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);

int pulse;
double angle;

int main(void){
    encoder.reset();
    while (true) {
        pulse = encoder.getPulses();
        angle = (360*(double)pulse)/(2048*2);
        printf("pulse:%d angle:%lf\n",pulse,angle);
    }
}
