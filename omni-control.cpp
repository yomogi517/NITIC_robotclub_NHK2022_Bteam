//回転(旋回)が速すぎるかもしれない

#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"

#define MOTOR1 0x00 //右前に使うモーターのモータードライバのアドレスぶち込め
#define MOTOR2 0x00 //左前に使うモーターのモータードライバのアドレスぶち込め
#define MOTOR3 0x00 //左後に使うモーターのモータードライバのアドレスぶち込め
#define MOTOR4 0x00 //右後に使うモーターのモータードライバのアドレスぶち込め

I2C i2c(D14,D15); //適宜ピンを確認して変えてください
DigitalOut sig(D13);
PS3 ps3(D8,D2);
int send(char addr,char data);

int main(){
    sig = 0;
    while (true) {
    
        //セレクトボタンで遠隔緊急停止
        if(ps3.getSELECTState()){
            sig = 1;
        }
        
        //スタートボタンで緊急停止解除
        if(ps3.getSTARTState()){
            sig = 0;
        }
    
        if(ps3.getButtonState(PS3::ue)){
            if(ps3.getButtonState(PS3::migi)){
                //右前方向
                send(MOTOR2,0x00);
                send(MOTOR4,0xff);
                
            }else if(ps3.getButtonState(PS3::hidari)){
                //左前方向
                send(MOTOR1,0xff);
                send(MOTOR3,0x00);
                
            }else{
                //前進
                send(MOTOR1,0xff);
                send(MOTOR2,0x00);
                send(MOTOR3,0x00);
                send(MOTOR4,0xff);
            }
            
        }else if(ps3.getButtonState(PS3::sita)){
            if(ps3.getButtonState(PS3::migi)){
                //右後方向
                send(MOTOR1,0x00);
                send(MOTOR3,0xff);
                
            }else if(PS3::hidari){
                //左後方向
                send(MOTOR2,0xff);
                send(MOTOR4,0x00);
                
            }else{
                //後進
                send(MOTOR1,0x00);
                send(MOTOR2,0xff);
                send(MOTOR3,0xff);
                send(MOTOR4,0x00);
            }
            
        }else if(ps3.getButtonState(PS3::hidari)){
            //左方向
            send(MOTOR1,0xff);
            send(MOTOR2,0xff);
            send(MOTOR3,0x00);
            send(MOTOR4,0x00);
            
        }else if(ps3.getButtonState(PS3::migi)){
            //右方向
            send(MOTOR1,0x00);
            send(MOTOR2,0x00);
            send(MOTOR3,0xff);
            send(MOTOR4,0xff);
            
        }else if(ps3.getButtonState(PS3::R1) || ps3.getButtonState(PS3::R2)){
            //右回転　(時計回り)　R1、R2どちらでもok
            send(MOTOR1,0x00);
            send(MOTOR2,0x00);
            send(MOTOR3,0x00);
            send(MOTOR4,0x00);
            
        }else if(ps3.getButtonState(PS3::L1) || ps3.getButtonState(PS3::L2)){
            //左回転 (反時計回り)　L1、L2どちらでもok
            send(MOTOR1,0xff);
            send(MOTOR2,0xff);
            send(MOTOR3,0xff);
            send(MOTOR4,0xff);
            
        }else{
            send(MOTOR1,0x80);
            send(MOTOR2,0x80);
            send(MOTOR3,0x80);
            send(MOTOR4,0x80);
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
