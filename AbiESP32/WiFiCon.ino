void wifiConTask() {
  task.setInitCoreID(1);
  task.createTask(10000, [](void* pvParameter) {
    auth.apiKey = "AIzaSyDryRLCTAZKSOrftT8WvewhWIc9ohsy1bc";
    auth.databaseURL = "https://abiramaapps-default-rtdb.firebaseio.com/";
    auth.projectID = "abiramaapps";

    auth.user.email = "admin@gmail.com";
    auth.user.password = "admin123";

    // if (!wm.autoConnect("AbirawaAP", ""))  // 192.168.4.1
    // Serial.println("| Failed to connect");
    firebase.connectToWiFi("TIMEOSPACE", "1234Saja");
    // firebase.connectToWiFi("abi", "abirawa1945");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    firebase.init(&auth);

    bot.init("+6282142161193", "5362229");

    disableCore1WDT();
    buzzer.toggleInit(100, 2);

    for (;;) {
      if (firebase.isConnect() && var.firebaseEnable) {
        static uint32_t dataSendTimer;
        if (millis() - dataSendTimer >= 2000) {

          JsonDocument setJson;
          setJson["ketinggian"] = String(var.height, 2);
          setJson["mq"] = var.statusAmonia + " | " + String(var.amonia, 2);
          setJson["ntu"] = var.statusTurbidity + " | " + String(var.turbidity, 2);
          setJson["ph"] = String(var.ph, 2);
          setJson["temperature"] = String(var.temperature, 2);
          firebase.setJson("/data", setJson, FirebaseModule::resultStatusCallback);

          firebase.getJson(
            "/limit", [](JsonVariant getJson) {
              // serializeJsonPretty(getJson, Serial);

              var.maxTemperature = getJson["maxTemperature"].as<String>().toFloat();
              var.maxHeight = getJson["maxKetinggian"].as<String>().toFloat();
              var.maxAmonia = getJson["maxMq"].as<String>().toFloat();
              var.maxPh = getJson["maxPh"].as<String>().toFloat();
              var.maxTurbidity = getJson["maxNtu"].as<String>().toFloat();

              var.minTemperature = getJson["minTemperature"].as<String>().toFloat();
              var.minHeight = getJson["minKetinggian"].as<String>().toFloat();
              var.minAmonia = getJson["minMq"].as<String>().toFloat();
              var.minPh = getJson["minPh"].as<String>().toFloat();
              var.minTurbidity = getJson["minNtu"].as<String>().toFloat();
            },
            FirebaseModule::resultStatusCallback);
          dataSendTimer = millis();
        }

        if (var.historyTemperature != var.historyTemperatureBefore) {
          uint32_t epoch = getTime();
          epoch += gmtOffset_sec;
          DateTimeNTP currentTime(epoch);
          static String timestampPrint;
          timestampPrint = currentTime.timestamp();

          firebase.pushJson(
            "/riwayat_alert", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ketinggian"] = "";
              pushJson["mq"] = "";
              pushJson["ntu"] = "";
              pushJson["ph"] = "";
              pushJson["temperature"] = String(var.temperature, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/temperature_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["temperature_chart"] = String(var.temperature, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);
          var.historyTemperatureBefore = var.historyTemperature;
        }

        if (var.historyHeight != var.historyHeightBefore) {
          uint32_t epoch = getTime();
          epoch += gmtOffset_sec;
          DateTimeNTP currentTime(epoch);
          static String timestampPrint;
          timestampPrint = currentTime.timestamp();

          // firebase.pushJson(
          //   "/riwayat_alert", [](JsonVariant pushJson) -> JsonVariant {
          //     pushJson["ketinggian"] = String(var.height, 2);
          //     pushJson["mq"] = "";
          //     pushJson["ntu"] = "";
          //     pushJson["ph"] = "";
          //     pushJson["temperature"] = "";
          //     pushJson["timestamp"] = timestampPrint;
          //     return pushJson;
          //   },
          //   FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/ketinggian_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ketinggian_chart"] = String(var.height, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);
          var.historyHeightBefore = var.historyHeight;
        }

        if (var.historyAmonia != var.historyAmoniaBefore) {
          bot.sendMessages("Kadar Amonia Terlalu Tinggi !!", [](String res, String err) {
            if (!err.isEmpty()) {
              buzzer.off();
              buzzer.toggleInit(100, 5);
            }
          });

          uint32_t epoch = getTime();
          epoch += gmtOffset_sec;
          DateTimeNTP currentTime(epoch);
          static String timestampPrint;
          timestampPrint = currentTime.timestamp();

          firebase.pushJson(
            "/riwayat_alert", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ketinggian"] = "";
              pushJson["mq"] = String(var.amonia, 2);
              pushJson["ntu"] = "";
              pushJson["ph"] = "";
              pushJson["temperature"] = "";
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/mq_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["mq_chart"] = String(var.amonia, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);
          var.historyAmoniaBefore = var.historyAmonia;
        }

        if (var.historyPh != var.historyPhBefore) {
          uint32_t epoch = getTime();
          epoch += gmtOffset_sec;
          DateTimeNTP currentTime(epoch);
          static String timestampPrint;
          timestampPrint = currentTime.timestamp();

          firebase.pushJson(
            "/riwayat_alert", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ketinggian"] = "";
              pushJson["mq"] = "";
              pushJson["ntu"] = "";
              pushJson["ph"] = String(var.ph, 2);
              pushJson["temperature"] = "";
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/ph_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ph_chart"] = String(var.ph, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);
          var.historyPhBefore = var.historyPh;
        }

        if (var.historyTurbidity != var.historyTurbidityBefore) {
          uint32_t epoch = getTime();
          epoch += gmtOffset_sec;
          DateTimeNTP currentTime(epoch);
          static String timestampPrint;
          timestampPrint = currentTime.timestamp();

          firebase.pushJson(
            "/riwayat_alert", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ketinggian"] = "";
              pushJson["mq"] = "";
              pushJson["ntu"] = String(var.turbidity, 2);
              pushJson["ph"] = "";
              pushJson["temperature"] = "";
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/kekeruhan_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["kekeruhan_chart"] = String(var.turbidity, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);
          var.historyTurbidityBefore = var.historyTurbidity;
        }

        static uint32_t dataSendHistoryTimer;
        if (millis() - dataSendHistoryTimer >= 60000 * 2) {
          uint32_t epoch = getTime();
          epoch += gmtOffset_sec;
          DateTimeNTP currentTime(epoch);
          static String timestampPrint;
          timestampPrint = currentTime.timestamp();

          firebase.pushJson(
            "/kekeruhan_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["kekeruhan_chart"] = String(var.turbidity, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/ketinggian_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ketinggian_chart"] = String(var.height, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/mq_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["mq_chart"] = String(var.amonia, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/ph_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["ph_chart"] = String(var.ph, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);

          firebase.pushJson(
            "/temperature_grafik", [](JsonVariant pushJson) -> JsonVariant {
              pushJson["temperature_chart"] = String(var.temperature, 2);
              pushJson["timestamp"] = timestampPrint;
              return pushJson;
            },
            FirebaseModule::resultStatusCallback);
          dataSendHistoryTimer = millis();
        }
      }
      task.delay(20);
    }
  });

  task.createTask(6000, sensorUpdate);
}