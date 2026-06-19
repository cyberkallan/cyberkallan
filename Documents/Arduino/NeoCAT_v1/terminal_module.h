#pragma once
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <vector>

class WiFiModule;
class BLEModule;
class LEDController;

class TerminalModule {
public:
  void begin(WiFiModule* wifi, BLEModule* ble, LEDController* led);
  String execute(const String& cmdLine);
  String getPrompt();
  String getWelcomeBanner();

private:
  WiFiModule* _wifi = nullptr;
  BLEModule* _ble = nullptr;
  LEDController* _led = nullptr;
  String _cwd;  // current working directory

  // Command handlers
  String _cmdHelp(const String& args);
  String _cmdLs(const String& args);
  String _cmdCd(const String& args);
  String _cmdPwd(const String& args);
  String _cmdMkdir(const String& args);
  String _cmdRm(const String& args);
  String _cmdCat(const String& args);
  String _cmdEcho(const String& args);
  String _cmdTouch(const String& args);
  String _cmdTree(const String& path, int depth);
  String _cmdDf(const String& args);
  String _cmdUname(const String& args);
  String _cmdUptime(const String& args);
  String _cmdFree(const String& args);
  String _cmdTop(const String& args);
  String _cmdHostname(const String& args);
  String _cmdDate(const String& args);
  String _cmdWhoami(const String& args);
  String _cmdEnv(const String& args);
  String _cmdIfconfig(const String& args);
  String _cmdMac(const String& args);
  String _cmdIwlist(const String& args);
  String _cmdIwconfig(const String& args);
  String _cmdDeauth(const String& args);
  String _cmdBeacon(const String& args);
  String _cmdProbe(const String& args);
  String _cmdStop(const String& args);
  String _cmdBleScan(const String& args);
  String _cmdBleSpam(const String& args);
  String _cmdBleStop(const String& args);
  String _cmdPing(const String& args);
  String _cmdArp(const String& args);
  String _cmdCowsay(const String& args);
  String _cmdMatrix(const String& args);
  String _cmdRickroll(const String& args);
  String _cmdReboot(const String& args);
  String _cmdRun(const String& args);

  // Helpers
  String _resolvePath(const String& path);
  void _createDefaults();
};

extern TerminalModule termMod;
