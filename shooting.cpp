
#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"

#define MOTOR5 0x50 //クラッチに使うモーターのモータードライバのアドレスぶち込め
#define MOTOR6 0x40 //クランクに使うモーターのモータードライバのアドレスぶち込め

I2C i2c(D14,D15); //適宜ピンを確認して変えてください
DigitalOut sig(D13);
PS3 ps3(A0,A1);
int send(char addr,char data);

int main(){
    int clutch=0;//クラッチON/OFF
    sig = 0;
    while (true) {
    
        //セレクトボタンで遠隔緊急停止
        if(ps3.getSELECTState()){
            sig = 1;
            clutch=0;
        }
        
        //スタートボタンで緊急停止解除
        if(ps3.getSTARTState()){
            sig = 0;
        }
    
        //〇もしくはL2ボタンでクラッチ回転開始、✕ボタンでクラッチ停止
        if(ps3.getButtonState(PS3::batu)){
            clutch=0;//クラッチOFF
        }else if(ps3.getButtonState(PS3::maru)||ps3.getButtonState(PS3::L2)){
            clutch=1;//クラッチON
        }

        if(clutch==1){
            send(MOTOR5,0xff);//最高出力
        }else{
            send(MOTOR5,0x80);//停止
        }

        //R2ボタンでクランク回転、□で逆回転
        if(ps3.getButtonState(PS3::R2)){
            send(MOTOR6,0x90);
        }else if(ps3.getButtonState(PS3::sikaku)){
            send(MOTOR6,0x70);
        }else{
            send(MOTOR6,0x80);
        }
    }
}

int send(char addr, char data){
    i2c.start();
    i2c.write(addr);
    i2c.write(data);
    i2c.stop();
    wait_us(50000);         
    return 0;
}
