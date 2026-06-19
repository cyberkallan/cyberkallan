#include "terminal_module.h"
#include "wifi_module.h"
#include "ble_module.h"
#include "led_controller.h"
#include "config_manager.h"

TerminalModule termMod;

void TerminalModule::begin(WiFiModule* wifi, BLEModule* ble, LEDController* led) {
  _wifi = wifi;
  _ble = ble;
  _led = led;
  _cwd = "/";

  if (!LittleFS.begin(true)) {
    Serial.println(F("[ERROR] LittleFS Mount Failed"));
    return;
  }
  _createDefaults();
}

void TerminalModule::_createDefaults() {
  if (!LittleFS.exists("/home")) LittleFS.mkdir("/home");
  if (!LittleFS.exists("/tools")) LittleFS.mkdir("/tools");
  if (!LittleFS.exists("/etc")) LittleFS.mkdir("/etc");
  if (!LittleFS.exists("/tmp")) LittleFS.mkdir("/tmp");

  auto writeFile = [](const char* path, const char* msg) {
    if (!LittleFS.exists(path)) {
      File f = LittleFS.open(path, "w");
      if (f) { f.print(msg); f.close(); }
    }
  };

  writeFile("/tools/deauth_all.sh", "iwlist\ndeauth all\n");
  writeFile("/tools/recon.sh", "iwlist\nblescan\nifconfig\n");
  writeFile("/tools/stealth.sh", "stop\nblestop\nmatrix off\n");
  writeFile("/tools/flood.sh", "beacon Ransomware_C&C\\nBotnet_Node_73\\nTrojan_Dropper\\nMetasploit_Bind\\nKali_Linux_Root\n");
  writeFile("/etc/hostname", "neocat");
  writeFile("/etc/motd", "Welcome to NeoCAT Terminal v1.0\nType 'help' for available commands.");
}

String TerminalModule::getWelcomeBanner() {
  return String(F(
    " _  _         ___   _ _____\n"
    "| \\| |___ ___/ __| /_\\_   _|\n"
    "| .` / -_) _ \\__ \\/ _ \\| |\n"
    "|_|\\_\\___\\___/___/_/ \\_\\_|\n\n"
    "NeoCAT Terminal v1.0 | ESP32-S3\n"
    "Firmware by Arjun TM | @imarjunarz\n"
    "Type 'help' for commands."
  ));
}

String TerminalModule::getPrompt() {
  return "\x1b[32mneocat\x1b[0m:\x1b[34m" + _cwd + "\x1b[0m$ ";
}

String TerminalModule::_resolvePath(const String& path) {
  if (path.length() == 0) return _cwd;
  String p = path;
  if (!p.startsWith("/")) p = _cwd + (String(_cwd.endsWith("/") ? "" : "/") + p);
  
  // Basic cleanup
  while(p.indexOf("//") >= 0) p.replace("//", "/");
  
  // Handle ..
  while(p.indexOf("/..") >= 0) {
    int idx = p.indexOf("/..");
    if (idx == 0) { p = "/"; break; } // At root
    int parentIdx = p.lastIndexOf("/", idx - 1);
    if (parentIdx >= 0) {
      p = p.substring(0, parentIdx) + p.substring(idx + 3);
      if (p.length() == 0) p = "/";
    } else {
      p = "/"; break;
    }
  }
  
  // Handle .
  p.replace("/./", "/");
  if(p.endsWith("/.")) p = p.substring(0, p.length() - 2);
  
  if (p.length() > 1 && p.endsWith("/")) p = p.substring(0, p.length() - 1);
  return p;
}

String TerminalModule::execute(const String& cmdLine) {
  String out;
  out.reserve(256);
  
  String cmd = cmdLine;
  cmd.trim();
  if (cmd.length() == 0) return "";

  // Check for output redirection
  int redirIdx = cmd.indexOf(" > ");
  String outFile = "";
  if (redirIdx > 0) {
    outFile = cmd.substring(redirIdx + 3);
    outFile.trim();
    cmd = cmd.substring(0, redirIdx);
    cmd.trim();
  }

  int spaceIdx = cmd.indexOf(' ');
  String baseCmd = (spaceIdx > 0) ? cmd.substring(0, spaceIdx) : cmd;
  String args = (spaceIdx > 0) ? cmd.substring(spaceIdx + 1) : "";
  args.trim();

  // Route commands
  if (baseCmd == "help") out = _cmdHelp(args);
  else if (baseCmd == "ls") out = _cmdLs(args);
  else if (baseCmd == "cd") out = _cmdCd(args);
  else if (baseCmd == "pwd") out = _cmdPwd(args);
  else if (baseCmd == "mkdir") out = _cmdMkdir(args);
  else if (baseCmd == "rm") out = _cmdRm(args);
  else if (baseCmd == "cat") out = _cmdCat(args);
  else if (baseCmd == "echo") out = _cmdEcho(args);
  else if (baseCmd == "touch") out = _cmdTouch(args);
  else if (baseCmd == "tree") out = _cmdTree(args, 0); // Simplified to use standard args
  else if (baseCmd == "df") out = _cmdDf(args);
  else if (baseCmd == "uname") out = _cmdUname(args);
  else if (baseCmd == "uptime") out = _cmdUptime(args);
  else if (baseCmd == "free") out = _cmdFree(args);
  else if (baseCmd == "top") out = _cmdTop(args);
  else if (baseCmd == "hostname") out = _cmdHostname(args);
  else if (baseCmd == "date") out = _cmdDate(args);
  else if (baseCmd == "whoami") out = _cmdWhoami(args);
  else if (baseCmd == "env") out = _cmdEnv(args);
  else if (baseCmd == "ifconfig") out = _cmdIfconfig(args);
  else if (baseCmd == "mac") out = _cmdMac(args);
  else if (baseCmd == "iwlist") out = _cmdIwlist(args);
  else if (baseCmd == "iwconfig") out = _cmdIwconfig(args);
  else if (baseCmd == "deauth") out = _cmdDeauth(args);
  else if (baseCmd == "beacon") out = _cmdBeacon(args);
  else if (baseCmd == "probe") out = _cmdProbe(args);
  else if (baseCmd == "stop") out = _cmdStop(args);
  else if (baseCmd == "blescan") out = _cmdBleScan(args);
  else if (baseCmd == "blespam") out = _cmdBleSpam(args);
  else if (baseCmd == "blestop") out = _cmdBleStop(args);
  else if (baseCmd == "ping") out = _cmdPing(args);
  else if (baseCmd == "arp") out = _cmdArp(args);
  else if (baseCmd == "cowsay") out = _cmdCowsay(args);
  else if (baseCmd == "matrix") out = _cmdMatrix(args);
  else if (baseCmd == "rickroll") out = _cmdRickroll(args);
  else if (baseCmd == "reboot") out = _cmdReboot(args);
  else if (baseCmd == "run" || baseCmd == "sh") out = _cmdRun(args);
  else out = "neocat: command not found: " + baseCmd;

  if (outFile.length() > 0) {
    String outPath = _resolvePath(outFile);
    File f = LittleFS.open(outPath, "w");
    if (!f) return "neocat: cannot create file '" + outPath + "'";
    f.print(out);
    f.close();
    return ""; // Nothing to console if redirected
  }

  // Safety truncation
  if (out.length() > 4096) {
    out = out.substring(0, 4000) + "\n... (output truncated)";
  }
  
  return out;
}

String TerminalModule::_cmdHelp(const String& args) {
  if (args.length() > 0) return "Use 'help' to see all commands. Specific help not yet implemented.";
  
  return String(F(
    "\x1b[36mFILESYSTEM:\x1b[0m\n"
    "  ls, cd, pwd, mkdir, rm, cat, echo, touch, df, tree\n"
    "\x1b[36mSYSTEM:\x1b[0m\n"
    "  uname, uptime, free, top, hostname, date, whoami, env, reboot\n"
    "\x1b[36mNETWORK/WIFI:\x1b[0m\n"
    "  ifconfig, mac, iwlist, iwconfig, deauth, beacon, probe, stop\n"
    "\x1b[36mBLE:\x1b[0m\n"
    "  blescan, blespam, blestop\n"
    "\x1b[36mUTILS:\x1b[0m\n"
    "  ping, arp, run/sh\n"
    "\x1b[36mFUN:\x1b[0m\n"
    "  cowsay, matrix, rickroll\n\n"
    "Example: echo hello > /home/test.txt"
  ));
}

String TerminalModule::_cmdLs(const String& args) {
  String path = _resolvePath(args);
  File root = LittleFS.open(path);
  if (!root || !root.isDirectory()) {
    if (root) { root.close(); return args + "\n"; } // It's a file
    return "ls: cannot access '" + args + "': No such file or directory";
  }
  
  String res = "";
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      res += "\x1b[34m" + String(file.name()) + "\x1b[0m/\n";
    } else {
      res += String(file.name()) + "  \x1b[30m(" + String(file.size()) + "B)\x1b[0m\n";
    }
    file = root.openNextFile();
  }
  root.close();
  return res.length() > 0 ? res : "(empty directory)";
}

String TerminalModule::_cmdCd(const String& args) {
  if (args.length() == 0) { _cwd = "/home"; return ""; }
  String newPath = _resolvePath(args);
  File d = LittleFS.open(newPath);
  if (!d || !d.isDirectory()) return "cd: " + args + ": No such directory";
  _cwd = newPath;
  return "";
}

String TerminalModule::_cmdPwd(const String& args) { return _cwd; }

String TerminalModule::_cmdMkdir(const String& args) {
  if (args.length() == 0) return "mkdir: missing operand";
  String p = _resolvePath(args);
  if (LittleFS.mkdir(p)) return "";
  return "mkdir: cannot create directory '" + args + "'";
}

String TerminalModule::_cmdRm(const String& args) {
  if (args.length() == 0) return "rm: missing operand";
  bool recursive = args.startsWith("-r ");
  String target = recursive ? args.substring(3) : args;
  target.trim();
  String p = _resolvePath(target);
  
  File f = LittleFS.open(p);
  if (!f) return "rm: cannot remove '" + target + "': No such file or directory";
  
  if (f.isDirectory() && !recursive) {
    f.close();
    return "rm: cannot remove '" + target + "': Is a directory";
  }
  
  f.close();
  // Very simplistic recursive delete (doesn't handle deep nesting well, but works for simple cases)
  if (LittleFS.remove(p)) return "";
  if (LittleFS.rmdir(p)) return "";
  return "rm: failed to remove '" + target + "'";
}

String TerminalModule::_cmdCat(const String& args) {
  if (args.length() == 0) return "cat: missing operand";
  String p = _resolvePath(args);
  File f = LittleFS.open(p, "r");
  if (!f) return "cat: " + args + ": No such file or directory";
  if (f.isDirectory()) { f.close(); return "cat: " + args + ": Is a directory"; }
  
  String res = f.readString();
  f.close();
  return res;
}

String TerminalModule::_cmdEcho(const String& args) {
  return args;
}

String TerminalModule::_cmdTouch(const String& args) {
  if (args.length() == 0) return "touch: missing file operand";
  String p = _resolvePath(args);
  File f = LittleFS.open(p, "w");
  if (f) { f.close(); return ""; }
  return "touch: cannot touch '" + args + "'";
}

String TerminalModule::_cmdTree(const String& path, int depth) {
  // Simple implementation to avoid deep recursion issues
  return "Tree command not fully implemented. Use ls.";
}

String TerminalModule::_cmdDf(const String& args) {
  size_t total = LittleFS.totalBytes();
  size_t used = LittleFS.usedBytes();
  return String(F("Filesystem     Total       Used        Free\nLittleFS       ")) +
         String(total/1024) + "KB      " + String(used/1024) + "KB       " + String((total-used)/1024) + "KB";
}

String TerminalModule::_cmdUname(const String& args) { return "NeoCAT V1.0 ESP32-S3 (Xtensa LX7) FreeRTOS"; }

String TerminalModule::_cmdUptime(const String& args) {
  uint32_t s = millis() / 1000;
  uint32_t h = s / 3600; s %= 3600;
  uint32_t m = s / 60; s %= 60;
  char buf[32];
  snprintf(buf, sizeof(buf), "up %02u:%02u:%02u", h, m, s);
  return String(buf);
}

String TerminalModule::_cmdFree(const String& args) {
  return String(F("              total        free        min-free\nMem:        ")) +
         String(ESP.getHeapSize()) + "      " + String(ESP.getFreeHeap()) + "      " + String(ESP.getMinFreeHeap());
}

String TerminalModule::_cmdTop(const String& args) {
  return "PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND\n"
         "  1 root      20   0       0      0      0 R 100.0   2.1   0:00.00 NeoCAT_Core\n"
         "  2 root      10   0       0      0      0 S   0.0   0.5   0:00.00 LED_Anim";
}

String TerminalModule::_cmdHostname(const String& args) {
  File f = LittleFS.open("/etc/hostname", "r");
  if (f) { String n = f.readString(); f.close(); n.trim(); return n; }
  return "neocat";
}

String TerminalModule::_cmdDate(const String& args) { return "Uptime date: " + _cmdUptime(""); }
String TerminalModule::_cmdWhoami(const String& args) { return "root"; }
String TerminalModule::_cmdEnv(const String& args) {
  return "CHIP=ESP32-S3\nFREQ=" + String(ESP.getCpuFreqMHz()) + "MHz\nFLASH=" + String(ESP.getFlashChipSize()/1048576) + "MB\nSDK=" + String(ESP.getSdkVersion());
}

String TerminalModule::_cmdIfconfig(const String& args) {
  return "wlan0     Link encap:Ethernet  HWaddr " + WiFi.softAPmacAddress() + "\n" +
         "          inet addr:" + WiFi.softAPIP().toString() + "  Mask:255.255.255.0\n" +
         "          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1\n" +
         "          Stations connected: " + String(WiFi.softAPgetStationNum());
}

String TerminalModule::_cmdMac(const String& args) {
  return "WiFi AP MAC: " + WiFi.softAPmacAddress() + "\n" +
         "WiFi STA MAC: " + WiFi.macAddress();
}

String TerminalModule::_cmdIwlist(const String& args) {
  if (!_wifi) return "WiFi module not initialized";
  _wifi->startScan();
  
  if (_wifi->getScanCount() == 0) return "Scanning started... run 'iwlist' again to view results.";

  String res = "\x1b[36mID   BSSID              PWR   CH  ENC       SSID\x1b[0m\n";
  res += "--------------------------------------------------\n";
  
  for (int i = 0; i < _wifi->getScanCount(); i++) {
    const ScanResult* sr = _wifi->getScanResult(i);
    if (!sr) continue;
    
    char buf[128];
    String enc = (sr->encryption == WIFI_AUTH_OPEN) ? "OPEN" : "SECURE";
    snprintf(buf, sizeof(buf), "%-4d %-18s %-5d %-3d %-9s %s\n", 
             i, sr->bssid.c_str(), sr->rssi, sr->channel, enc.c_str(), sr->ssid.c_str());
    res += buf;
  }
  return res;
}

String TerminalModule::_cmdIwconfig(const String& args) {
  return "wlan0     IEEE 802.11bgn  ESSID:\"" + config.getSSID() + "\"\n"
         "          Mode:Master  Frequency:2.412 GHz  Tx-Power=20 dBm\n"
         "          Retry short limit:7   RTS thr:off   Fragment thr:off\n"
         "          Power Management:off";
}

String TerminalModule::_cmdDeauth(const String& args) {
  if (!_wifi) return "WiFi module not initialized";
  if (args == "all") {
    _wifi->startDeauthAll();
    return "Deauth attack started on all networks";
  }
  int target = args.toInt();
  if (args.length() > 0 && target >= 0) {
    _wifi->startDeauth(target);
    return "Deauth attack started on target " + String(target);
  }
  return "Usage: deauth <target_index|all>";
}

String TerminalModule::_cmdBeacon(const String& args) {
  if (!_wifi) return "WiFi module not initialized";
  if (args.length() == 0) return "Usage: beacon <ssid1\\nssid2...>";
  String parsed = args;
  parsed.replace("\\n", "\n");
  _wifi->startBeaconFlood(parsed);
  return "Beacon flood started";
}

String TerminalModule::_cmdProbe(const String& args) {
  if (!_wifi) return "WiFi module not initialized";
  _wifi->startProbeFlood();
  return "Probe flood started";
}

String TerminalModule::_cmdStop(const String& args) {
  if (!_wifi) return "WiFi module not initialized";
  _wifi->stopAttack();
  return "WiFi attacks stopped";
}

String TerminalModule::_cmdBleScan(const String& args) {
  if (!_ble) return "BLE module not initialized";
  _ble->startScan(5);

  if (_ble->getDeviceCount() == 0) return "Scanning BLE started... run 'blescan' again to view results.";

  String res = "\x1b[35mID   MAC                PWR   NAME\x1b[0m\n";
  res += "--------------------------------------------------\n";
  
  for (int i = 0; i < _ble->getDeviceCount(); i++) {
    const BLEDeviceInfo* d = _ble->getDevice(i);
    if (!d) continue;
    char buf[128];
    String name = d->name.length() > 0 ? d->name : "<Unknown>";
    snprintf(buf, sizeof(buf), "%-4d %-18s %-5d %s\n", 
             i, d->address.c_str(), d->rssi, name.c_str());
    res += buf;
  }
  return res;
}

String TerminalModule::_cmdBleSpam(const String& args) {
  if (!_ble) return "BLE module not initialized";
  if (args == "apple") _ble->startSpam(BLE_SPAM_APPLE);
  else if (args == "samsung") _ble->startSpam(BLE_SPAM_SAMSUNG);
  else if (args == "windows") _ble->startSpam(BLE_SPAM_WINDOWS);
  else if (args == "all") _ble->startSpam(BLE_SPAM_ALL);
  else return "Usage: blespam <apple|samsung|windows|all>";
  return "BLE spam started: " + args;
}

String TerminalModule::_cmdBleStop(const String& args) {
  if (!_ble) return "BLE module not initialized";
  _ble->stopSpam();
  return "BLE spam stopped";
}

String TerminalModule::_cmdPing(const String& args) {
  return "PING " + args + " (56 data bytes)\n"
         "64 bytes from " + args + ": icmp_seq=1 ttl=64 time=1.2ms\n"
         "64 bytes from " + args + ": icmp_seq=2 ttl=64 time=0.9ms\n"
         "--- " + args + " ping statistics ---\n"
         "2 packets transmitted, 2 received, 0% packet loss";
}

String TerminalModule::_cmdArp(const String& args) {
  return "Address                  HWtype  HWaddress           Flags Mask            Iface\n"
         "192.168.4.2              ether   AA:BB:CC:DD:EE:FF   C                     wlan0";
}

String TerminalModule::_cmdCowsay(const String& args) {
  String msg = args.length() > 0 ? args : "Moo.";
  String line = ""; for(int i=0; i<msg.length()+2; i++) line += "-";
  return " " + line + "\n< " + msg + " >\n " + line + "\n"
         "        \\   ^__^\n"
         "         \\  (oo)\\_______\n"
         "            (__)\\       )\\/\\\n"
         "                ||----w |\n"
         "                ||     ||";
}

String TerminalModule::_cmdMatrix(const String& args) {
  if (!_led) return "LED module not initialized";
  if (args == "off") { _led->setMode(LED_IDLE); return "Matrix off"; }
  _led->setMode(LED_SCANNING); // Closest to matrix effect
  return "Matrix engaged";
}

String TerminalModule::_cmdRickroll(const String& args) {
  if (!_wifi) return "WiFi module not initialized";
  _wifi->startBeaconRickroll(args);
  return "Never gonna give you up...";
}

String TerminalModule::_cmdReboot(const String& args) {
  // Need to yield and let the current task finish
  // A real impl would set a flag to reboot in the loop
  return "Reboot command received (implement flag in main loop)";
}

String TerminalModule::_cmdRun(const String& args) {
  if (args.length() == 0) return "run: missing file operand";
  String p = _resolvePath(args);
  File f = LittleFS.open(p, "r");
  if (!f) return "run: " + args + ": No such file or directory";
  
  String out = "";
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    if (line.length() > 0 && !line.startsWith("#")) {
      out += "\x1b[33m+ " + line + "\x1b[0m\n";
      String res = execute(line);
      if (res.length() > 0) out += res + "\n";
    }
  }
  f.close();
  return out;
}
