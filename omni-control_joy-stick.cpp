#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"

#define ADDRESS_MIGI_UE 0x40
#define ADDRESS_MIGI_SITA 0x50
#define ADDRESS_HIDARI_UE 0x24
#define ADDRESS_HIDARI_SITA 0x30

// DigitalIn button(USER_BUTTON);
// Serial pc(USBTX,USBRX);
I2C i2c(D14,D15);
PS3 ps3 (A0,A1);
DigitalOut sig(D13);

//プロトタイプ宣言
void get_data(void);
void send(char add,char data);
void send_all(char d_mu, char d_ms, char d_hu, char d_hs);
char move_value(double value);

//変数の初期化
int Ry;            //ジョイコン　右　y軸
int Rx;            //ジョイコン　右　x軸
int Ly;            //ジョイコン　左　y軸
int Lx;            //ジョイコン　左　x軸
int L;             //L1
int R;             //R1
int button_ue;     // ↑
int button_sita;   // ↓
int button_migi;   // →
int button_hidari; // ←

int button_maru;    // 〇
int button_sankaku; // △
int button_sikaku;  // ☐
int button_batu;    // ✕

char data;

//main関数
int main(void){    
    int moved;

    while(true){  
        //緊急停止
        if(ps3.getSELECTState()){
            sig = 1;
        }

        if(ps3.getSTARTState()){
            sig = 0;
        }

        get_data();
        printf("m:%d L:%d R:%d Lx%d Ly%d\n",button_maru,L,R,Lx,Ly);
        
        //ジョイコン処理
        moved = 0;

        if(!Lx || !Ly){
            double value_ru,value_rs,value_lu,value_ls;   //右上,右下,左上,左下
            char data_ru,data_rs,data_lu,data_ls;

            if(Lx==-64) Lx = -63;
            if(Ly==64) Ly = 63;

            //右回転を正としたときのベクトル値計算
            value_ru = 0.7071*((double)Ly-(double)Lx)*-1;//逆転
            value_ls = 0.7071*((double)Ly-(double)Lx);   //正転
            value_rs = 0.7071*((double)Ly+(double)Lx)*-1;//逆転
            value_lu = 0.7071*((double)Ly+(double)Lx);   //正転

            data_ru = move_value(value_ru);
            data_rs = move_value(value_rs);
            data_lu = move_value(value_lu);
            data_ls = move_value(value_ls);
            
            send_all(data_ru, data_rs, data_lu, data_ls);
            moved = 1;
        }
        
        //左に旋回
        if(L==1 && !moved){
            data = 0x01;
            send_all(data,data,data,data);
            moved = 1;
        }

        //右に旋回
        if(R==1 && !moved){
            data = 0xff;
            send_all(data,data,data,data);
            moved = 1;
        }
        
        if(!moved){
            data = 0x80;  
            send_all(data,data,data,data);
        }

        //●ボタン
        //よくわからんからコメントアウトしておいた
        /*
        if(button_maru && !moved){
            data = 0xff;
            send(ADDRESS_MIGI_SITA,data);
        }
        */
        
    }
}

//データ取得のための関数
void get_data(void){
    //ボタン取得
    button_ue = ps3.getButtonState(PS3::ue);
    button_sita= ps3.getButtonState(PS3::sita);
    button_migi = ps3.getButtonState(PS3::migi);
    button_hidari = ps3.getButtonState(PS3::hidari);

    L = ps3.getButtonState(PS3::L1);
    R = ps3.getButtonState(PS3::R1);

    button_maru = ps3.getButtonState(PS3::maru);

    //スティックの座標取得
    Ry = ps3.getRightJoystickYaxis();
    Rx = ps3.getRightJoystickXaxis();
    Ly = ps3.getLeftJoystickYaxis();
    Lx = ps3.getLeftJoystickXaxis();
}

//I2C送信用関数
void send(char address, char data){
    wait_us(15000);
    i2c.start();
    i2c.write(address);
    i2c.write(data);
    i2c.stop();
}

void send_all(char d_mu, char d_ms, char d_hs, char d_hu){
    send(ADDRESS_MIGI_UE,d_mu);
    send(ADDRESS_MIGI_SITA,d_ms);
    send(ADDRESS_HIDARI_SITA,d_hs);
    send(ADDRESS_HIDARI_UE,d_hu);
}

//取得座標から回転速度を求める関数
char move_value(double value){
    double uv = value;
    int rate, move;
    char resulte;

    //絶対値化
    if(value<0) uv = uv*-1;

    //割合計算(小数点以下四捨五入=>整数)
    //座標の絶対値の最高値が63と64になるので小さいほうの63に合わせて割合を出す
    rate = (double)uv/89.0954*100+0.5;
    
    //64だと100%を超えるのでその調整
    if(rate>100) rate = 100;

    //割合をもとに出力するパワーを計算(整数)
    rate = (double)rate/100.0*123.0;
    
    //正転と逆転の処理
    //引数がマイナスの時は逆転、プラスの時は正転、0の時には静止
    if(!value) move = 128;
    else if(value<0) move = 124-rate;
    else move = rate+ 132;

    //型変更
    resulte = (char)move;
    return resulte;
}
