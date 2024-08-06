#define ENABLE_MODULE_FIREBASE_HANDLER
#define ENABLE_MODULE_WHATSAPP_BOT
#define ENABLE_MODULE_TASK_HANDLER
#define ENABLE_MODULE_TIMER_DURATION
#define ENABLE_MODULE_TIMER_TASK
#define ENABLE_MODULE_SERIAL_HARD
#define ENABLE_MODULE_SERIAL_SWAP
#define ENABLE_MODULE_DIGITAL_INPUT
#define ENABLE_MODULE_DIGITAL_OUTPUT
#define ENABLE_MODULE_LCD_MENU
#define ENABLE_MODULE_DATETIME_NTP

#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_DS18B20
#define ENABLE_SENSOR_ULTRASONIC

#include "Kinematrix.h"
// #include "WiFiManager.h"

////////// Utility //////////
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;  // Offset for WIB (UTC+7)
const int daylightOffset_sec = 0;

// WiFiManager wm;
TaskHandle task;
WhatsappBot bot;
FirebaseModule firebase;
FirebaseAuthentication auth;

TimerDuration waterPumpTimer;
TimerDuration phUpDownTimer;
TimerDuration servoOnTimer;

////////// Sensor //////////
SensorModule sensor;
MovingAverageFilter heightFilter(30);

////////// Communication //////////
HardSerial usbSerial;
SwapSerial comSerial;

////////// Input Module //////////
DigitalIn buttonDown(36);
DigitalIn buttonOk(39);

////////// Output Module //////////
LcdMenu menu(0x27, 20, 4);
DigitalOut buzzer(5);
DigitalOut relayPompaKuras(27, true);
DigitalOut relayPompaIsi(26, true);
DigitalOut relayHeater(25, true);
DigitalOut relayD(33, true);

////////// Global Variable //////////
struct SystemData {
  const float voltageAmoniaThreshold = 1.5;
  const float voltageTurbidityThreshold = 0.75;

  float maxTemperature;
  float maxHeight;
  float maxAmonia;
  float maxPh;
  float maxTurbidity;

  float minTemperature;
  float minHeight;
  float minAmonia;
  float minPh;
  float minTurbidity;

  int historyTemperature = 0;
  int historyHeight = 0;
  int historyAmonia = 0;
  int historyPh = 0;
  int historyTurbidity = 0;

  int historyTemperatureBefore = 0;
  int historyHeightBefore = 0;
  int historyAmoniaBefore = 0;
  int historyPhBefore = 0;
  int historyTurbidityBefore = 0;

  float temperature;
  float height;
  float amonia;
  float ph;
  float turbidity;

  String statusAmonia;
  String statusTurbidity;

  int debugLevel = 0;
  int firebaseEnable = 1;

  int systemEnable = 1;

  int waterPumpState = 0;
  int waterPumpTrigger = 0;
  int phOnOffState = 0;
  int phOnOffTrigger = 0;
  int servoDegree = 70;
};

SystemData var;