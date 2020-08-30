//2018 국제로봇올림피아드 해양오염 방제 로봇 Ocean Rover
//Code Wrote by H.S.H.

#include <TinyGPS++.h>
#include <Servo.h>

Servo door;
Servo ext;

static const uint32_t GPSBaud = 9600;

int doorpos, extpos, doorstate, extstate = 0;
int count = 0;
String str1;

int m2pin1 = 2;
int m2pin2 = 3;
int in1pin = 4;
int in2pin = 5;
int in3pin = 6;
int in4pin = 7;
int PumpPin = 8;

int m2state, m3state, Pumpstate, forward, backward, left, right = 0;

char junk;
String inputString;


TinyGPSPlus gps; //TinyGPS++

void setup()
{
  Serial.begin(9600);          //PC Serial 통신
  Serial1.begin(GPSBaud);      //GPS 모듈 Serial 통신
  Serial2.begin(9600);         //Bluetooth Serial 통신


  pinMode(m2pin1, OUTPUT);
  pinMode(m2pin2, OUTPUT);
  pinMode(in1pin, OUTPUT);
  pinMode(in2pin, OUTPUT);
  pinMode(in3pin, OUTPUT);
  pinMode(in4pin, OUTPUT);
  pinMode(PumpPin, OUTPUT);

  ext.attach(9);
  door.attach(10);
}


void M1forward() {
  digitalWrite(in1pin, 1);
  digitalWrite(in2pin, 1);
}

void M2forward() {
  digitalWrite(in3pin, 1);
  digitalWrite(in4pin, 1);
}

void M1backward() {
  digitalWrite(in1pin, 0);
  digitalWrite(in2pin, 1);
}

void M2backward() {
  digitalWrite(in3pin, 0);
  digitalWrite(in4pin, 1);
}

void M1stop() {
  digitalWrite(in1pin, 1);
  digitalWrite(in2pin, 0);
}

void M2stop() {
  digitalWrite(in3pin, 1);
  digitalWrite(in4pin, 0);
}

void M3forward() {
  digitalWrite(m2pin1, 1);
  digitalWrite(m2pin2, 0);
}

void M3backward() {
  digitalWrite(m2pin1, 0);
  digitalWrite(m2pin2, 1);
}

void M3stop() {
  digitalWrite(m2pin1, 0);
  digitalWrite(m2pin2, 0);
}


static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}
static void printFloat(float val, bool valid, int len, int prec)
//printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial2.print('*');
    Serial.print(' ');
    Serial2.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    Serial2.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i) {
      Serial.print(' ');
      Serial2.print(' ');
    }
  }
  smartDelay(0);
}
static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  Serial.print(sz);
  Serial2.print(sz);
  smartDelay(0);
}
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
    Serial2.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
    Serial2.print(sz);
  }
  if (!t.isValid())
  {
    Serial.print(F("******** "));
    Serial2.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour() + 8, t.minute(), t.second());
    Serial.print(sz);
    Serial2.print(sz);
  }
  //printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}
static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i) {
    Serial.print(i < slen ? str[i] : ' ');
    Serial2.print(i < slen ? str[i] : ' ');
  }
  smartDelay(0);
}

void loop()
{
  //Main Function

  if (Serial2.available()) {

    inputString = (char)Serial2.read();  //Serial 수신 데이터
    Serial.println(inputString);

    if (inputString == "1") {
      //시리얼 입력 "1" - 전진
      if (forward == 0) {
        //전진 중이 아닐때 전진
        M1forward();
        M2forward();
        forward = 1;
      }
      else if (forward == 1) {
        //전진 중일때는 정지
        M1stop();
        M2stop();
        forward = 0;
      }


    } else if (inputString == "2") {
      //시리얼 입력 "2" - 후진
      if (backward == 0) {
        //후진 중이 아닐 때는 후진
        M1backward();
        M2backward();
        backward = 1;
        exit;
      }
      else if (backward == 1) {
        //후진 중일 때는 정지
        M1stop();
        M2stop();
        backward = 0;
        exit;
      }


    } else if (inputString == "3") {
      //시리얼 입력 "3" - 좌회전
      if (left == 0) {
        //좌회전 중이 아닐 때는 좌회전
        M1forward();
        M2backward();
        left = 1;
      } else if (left == 1) {
        //좌회전 중일 때는 정지
        M1stop();
        M2stop();
        left = 0;
      }


    } else if (inputString == "4") {
      //시리얼 입력 "4" - 우회전
      if (right == 0) {
        //우회전 중이 아닐 때는 우회전
        M1backward();
        M2forward();
        right = 1;
      } else if (right = 1) {
        //우회전 중일 때는 정지
        M1stop();
        M2stop();
        right = 0;
      }


    } else if (inputString == "5") {

      //TinyGPS Serial 출력
      
      printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
      printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);

      printDateTime(gps.date, gps.time);
      printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);

    } else if (inputString == "6") {
      //시리얼 입력 "6" - 펌프 토글
      
      if (Pumpstate == 0)
      {
        digitalWrite(PumpPin, 1);
        Pumpstate = 1;
      }
      else
      {
        digitalWrite(PumpPin, 0);
        Pumpstate = 0;
      }
      
    } else if (inputString == "7") {
      //시리얼 입력 "7" - 오일펜스 설치 (문 개폐 토글)
      
      if (doorstate == 0) {
        for (doorpos = 0; doorpos < 90; doorpos++) {
          door.write(doorpos);
          delay(15);
          doorstate = 1;
        }
      }
      else if (doorstate == 1) {
        for (doorpos = 90; doorpos >= 0; doorpos--) {
          door.write(doorpos);
          delay(15);
          doorstate = 0;
        }
      }
    } else if (inputString == "8") {
      //시리얼 입력 "8" - 오일펜스 설치 (오일펜스 배출)
      
      if (extstate == 0) {
        for (extpos = 180; extpos >= 0; extpos--) {
          ext.write(extpos);
          delay(10);
          extstate = 1;
        }
      }
      else if (extstate == 1) {
        for (extpos = 0; extpos < 180; extpos++) {
          ext.write(extpos);
          delay(10);
          extstate = 0;
        }
      }
    } else if (inputString == "9") {
      if (m2state == 1) {
        M3forward();
        m2state = 0;
      }
      else if (m2state == 0) {
        M3stop();
        m2state = 1;
      }
    } else if (inputString == "a") {
      if (m3state == 1) {
        M3backward();
        m3state = 0;
      }
      else if (m3state == 0) {
        M3stop();
        m3state = 1;
      }
    }
    while (Serial2.available() > 0) {
      junk = Serial2.read() ;
    }
    inputString = "";
  }
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }
  if (Serial2.available()) {
    char a = Serial2.read();

  }



}


