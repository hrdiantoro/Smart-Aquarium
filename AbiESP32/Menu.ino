void menuTaskCallback() {
  static auto mainMenu = menu.createMenu(menu.begin(6), "Temp  : ", "pH    : ", "Amonia: ", "Turbid: ", "Height: ", "Show Limit");
  menu.onSelect(mainMenu, "Show Limit", []() {
    static auto showLimitMenu = menu.createMenu(menu.begin(11), "mnTemp  : ", "mxTemp  : ", "mnHeight: ", "mxHeight: ", "mnAmonia: ", "mxAmonia: ", "mnpH    : ", "mxpH    : ", "mnTurbid: ", "mxTurbid: ", "Kembali");
    menu.formatMenu(showLimitMenu, menu.get(0), "mnTemp  : ", var.minTemperature);
    menu.formatMenu(showLimitMenu, menu.get(1), "mxTemp  : ", var.maxTemperature);
    menu.formatMenu(showLimitMenu, menu.get(2), "mnHeight: ", var.minHeight);
    menu.formatMenu(showLimitMenu, menu.get(3), "mxHeight: ", var.maxHeight);
    menu.formatMenu(showLimitMenu, menu.get(4), "mnAmonia: ", var.minAmonia);
    menu.formatMenu(showLimitMenu, menu.get(5), "mxAmonia: ", var.maxAmonia);
    menu.formatMenu(showLimitMenu, menu.get(6), "mnpH    : ", var.minPh);
    menu.formatMenu(showLimitMenu, menu.get(7), "mxpH    : ", var.maxPh);
    menu.formatMenu(showLimitMenu, menu.get(8), "mnTurbid: ", var.minTurbidity);
    menu.formatMenu(showLimitMenu, menu.get(9), "mxTurbid: ", var.maxTurbidity);
    menu.onSelect(showLimitMenu, "Kembali", []() {
      menu.clearMenu(showLimitMenu, mainMenu, menu.end());
    });
    menu.showMenu(showLimitMenu);
  });
  static uint32_t displayMainMenuTimer;
  if (millis() - displayMainMenuTimer >= 500) {
    //////////////////////////////////////////////////
    float temperature = var.temperature;
    float height = var.height;
    float amonia = var.amonia;
    float ph = var.ph;
    float turbidity = var.turbidity;

    // temperature = var.temperature + (var.temperature * 0.018 * ((float)random(-100, 100) / 100.0));
    // height = var.height + (var.height * 0.018 * ((float)random(-100, 100) / 100.0));
    // amonia = var.amonia + (var.amonia * 0.018 * ((float)random(-100, 100) / 100.0));
    // ph = var.ph + (var.ph * 0.018 * ((float)random(-100, 100) / 100.0));
    // turbidity = var.turbidity + (var.turbidity * 0.018 * ((float)random(-100, 100) / 100.0));

    menu.formatMenu(mainMenu, menu.get(0), "Temp  : %6.2f", temperature);
    menu.formatMenu(mainMenu, menu.get(1), "pH    : %6.2f", ph);
    menu.formatMenu(mainMenu, menu.get(2), "Amonia: %6s", amonia > var.voltageAmoniaThreshold ? "Tinggi" : "Normal");
    menu.formatMenu(mainMenu, menu.get(3), "Turbid: %6s", turbidity < var.voltageTurbidityThreshold ? "Keruh" : "Bersih");
    menu.formatMenu(mainMenu, menu.get(4), "Height: %6.2f", height);
    //////////////////////////////////////////////////

    // menu.formatMenu(mainMenu, menu.get(0), "Temp  : %6.2f", var.temperature);
    // menu.formatMenu(mainMenu, menu.get(1), "Height: %6.2f", var.height);
    // menu.formatMenu(mainMenu, menu.get(2), "Amonia: %6.2f", var.amonia);
    // menu.formatMenu(mainMenu, menu.get(3), "pH    : %6.2f", var.ph);
    // menu.formatMenu(mainMenu, menu.get(4), "Turbid: %6.2f", var.turbidity);
    displayMainMenuTimer = millis();
  }
  menu.showMenu(mainMenu);
}