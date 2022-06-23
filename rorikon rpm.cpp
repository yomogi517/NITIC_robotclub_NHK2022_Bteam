#include "mbed.h"
#include "QEI.h"

QEI encoder( D5, D6, NC, 2048, QEI::X2_ENCODING);
Ticker flip;
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);

int pulse;
double rpm;

void get_rpm();

int main(void){
    encoder.reset();
    flip.attach(&get_rpm,50ms);
    while (true) {
        printf("%lf[rpm]\n",rpm);
    }
}

void get_rpm(){
    pulse = encoder.getPulses();
    encoder.reset();
    rpm = (60*20*(double)pulse) / (2048*2) ;
}
