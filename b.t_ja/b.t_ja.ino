#include<Wire.h>
#include<SoftwareSerial.h>
#include<math.h>
const int MPU=0x68;//MPU6050 I2C주소

SoftwareSerial BTSerial(2, 3); // SoftwareSerial(RX, TX)
byte buffer[1024]; // 데이터를 수신 받을 버퍼
int bufferPosition; // 버퍼에 데이타를 저장할 때 기록할 위치

int AcX[50],AcY[50],AcZ[50];
int i=0;
int vib =2;
int shock=0;
int cnt=0;
int numx[50],numy[50],numz[50];
void get6050();

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);//MPU6050 을 동작 대기 모드로 변경
  Wire.endTransmission(true);
  BTSerial.begin(9600);
  Serial.begin(9600);
   pinMode(2,INPUT);
  bufferPosition = 0;
}

void loop()

{
  //받아온 센서값을 출력합니다.
   get6050();//센서값 갱신
 long measurement = Shock_Judge();

if(shock ==1 && cnt ==1)
{ 
 BTSerial.println("warming");
}


 

  delay(1000);

}

void get6050()
{

  Wire.beginTransmission(MPU);//MPU6050 호출
  Wire.write(0x3B);//AcX 레지스터 위치 요청
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);//14byte의 데이터를 요청
 
 
  AcX[i]=Wire.read()<<8|Wire.read();//두개의 나뉘어진 바이트를 하나로 이어붙입니다.
  AcY[i]=Wire.read()<<8|Wire.read();
  AcZ[i]=Wire.read()<<8|Wire.read();

 
  numx[i]= AcX[i];
  numy[i]= AcY[i];
  numz[i]= AcZ[i];
  
  
   if((abs(abs(numx[i])-abs(numx[i-1])) <=1000 && abs(abs(numx[i]) - abs(numx[i -2])) <=1000 )
    && (abs(abs(numy[i]) - abs(numy[i -1])) <=1000 && abs(abs(numy[i]) - abs(numy[i -2])) <=1000)
    &&(abs(abs(numz[i]) - abs(numz[i -1])) <=1000 && abs(abs(numz[i]) - abs(numz[i -2])) <=1000))
    {
      cnt =0;// 0 이면 가만히
     }
    else
    cnt=1; // 1 이면 움직임
   
 i++;
 
}
long Shock_Judge() //충격측정판단함수
{
  delay(1000);
  long measurement=pulseIn (vib, HIGH); //충격측정값 변수
  if(measurement>1000) //받은 충격이 80000이 넘을경우
  {
   shock=1;
   }
  else 
     shock =0;
  return measurement;
}

  


