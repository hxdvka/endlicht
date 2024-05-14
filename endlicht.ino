#include "config.h"
#include <string>
#include <cstring>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include "miniQ.h"
#include <Regexp.h>
#include <span>


// include wpa2 enterprise code
extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

//function signatures
//wifi
void wifi_connect(std::string, std::string);
void wifi_connect(std::string, std::string, std::string);

String httpGETRequest(const char*, String, int);
String httpGETRequest(const char*, const char*, int);

//utils
String fxs_getstr(String);
int fxs_getint(String);

bool requestCmd();
uint runCmd();

//Global vars for emotional support or something
miniQ* mQ = new miniQ();

//------------------------SETUP-----------------------------

//HW settings
void initHW(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
  pinMode(PIN_B, OUTPUT);  // Initialize the pin as an output
  pinMode(PIN_G ,OUTPUT);  // Initialize the pin as an output
  pinMode(PIN_R ,OUTPUT);  // Initialize the pin as an output
}

void initWF(){

  const char* ssid;
  const char* usr;
  const char* pwd;
  bool enterprise = fxs_getint("wifi mode:\n 0 personal\n 1 enterprise");

  //ask if serial in?
  //getStr not working but like who cares, that shoould be a softAP input
  //TODO  add SoftAP wifi config
  if (fxs_getint("config mode:\n 0 default\n 1 manual")) {
    //softAP(ssid, usr, pwd);
    ssid = fxs_getstr("ssid:").c_str();
    usr = enterprise ? fxs_getstr("username:").c_str() : "";
    pwd = fxs_getstr("password:").c_str();
  } else {
    ssid = _SSID;
    usr = _E_USR;
    pwd = enterprise ? _E_PWD : _PWD;
  }

  //wifi setup
  Serial.println("connecting wifi");
  wifi_set_opmode(STATION_MODE);
  enterprise ? wifi_connect(ssid, usr, pwd) : wifi_connect(ssid, pwd);
}

void setup() {

  initHW();

  delay(5000);
  Serial.println("------------7w7----------");

  //------------------------------------------------------------------------

  initWF();
  Serial.println("connected");

  //yoyo server
  Serial.println("greeting sv");
  String resp = httpGETRequest(SV_HOST, SV_URL, SV_PORT);
  //do something w server response?
  Serial.println("setup done!");
  //escalate to ws? mebby
}

void loop() {
  static unsigned long tmp_Req = millis();
  static unsigned long tmp_Cmd = millis();

  // vars || or like silly mini state machine description
  //    Q status  :: full | empty | has stuff
  //    Cmd       :: running | idlying
  // Cmd idle && Q !empty -> run something
  // Q not full -> lf command
  // Cmd running && Q full -> idle until cmd end
  // protothread? mebby??

  if (!mQ->isEmpty() && millis() > tmp_Cmd) {
    tmp_Cmd = millis() + runCmd();
  }
  if (mQ->hasRoom() && millis() > tmp_Req) {
    if (requestCmd()) {
      tmp_Req = millis() + 1500;
    } else {
      tmp_Req = millis() + 60000;
    }
  }
  if (!mQ->hasRoom() && tmp_Cmd > millis()) {
    delay(tmp_Cmd - millis());
  }
}

/*
void genericLoop( std::function<void(void)> generic( ) ){}
//https://www.randomnumberapi.com/api/v1.0/random?min=0&max=255&count=3
//https://www.randomnumberapi.com/api/v1.0/random?min=300&max=3000
*/

bool requestCmd() {
  //http request verify ok

  //ignore the following eww
  char req_rgb[100] = SV_RNURL, req_delay[100] = SV_RNURL;
  strcat(req_rgb, "?min=0&max=255&count=3");
  strcat(req_delay, "?min=300&max=3000");

  String RGB = httpGETRequest(SV_RNHOST, req_rgb, SV_PORT);
  String Delay = httpGETRequest(SV_RNHOST, req_delay, SV_PORT);
  Delay.remove(0, 1);
  RGB.remove(0, 1);

  //parsing response like, stupidly, bc why not
  uint R = RGB.toInt();
  RGB.remove(0, RGB.indexOf(",") + 1);
  uint G = RGB.toInt();
  RGB.remove(0, RGB.indexOf(",") + 1);
  uint B = RGB.toInt();
  uint D = Delay.toInt();

  return mQ->add(R, G, B, D);
}

//returns delay aka endtime
uint runCmd() {
  //do stuff and pop Q
  uint ret = 0;
  aux data;
  if (mQ->pop(data)) {
    //do stuff w data like
    //board logic
    analogWrite(PIN_B,data.B);
    analogWrite(PIN_G,data.G);
    analogWrite(PIN_R,data.R);
    ret = data.d;
  }
  return ret;
}
