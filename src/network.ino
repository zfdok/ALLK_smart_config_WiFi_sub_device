#include "config.h"
bool autoConfig_wifi()
{
  WiFi.begin();
  for (int i = 0; i < 40; i++)
  {
    int wifiStatus = WiFi.status();
    if (wifiStatus == WL_CONNECTED)
    {
      Serial.println("autoconnected!");
      WiFi.setAutoConnect(true);
      return true;
    }
    else
    {
      delay(500);
      Serial.println("waiting for wifi...");
      showWifi(i%4);
    }
  }
  Serial.println("connot connected!");
  return false;
}

void smartWifiConfig()
{
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("start smartWifiConfig");
  WiFi.beginSmartConfig();
  showWifiSmart();
  while (!WiFi.isConnected())
  {
    Serial.print(".");
    delay(1000);
    if (WiFi.smartConfigDone())
    {
      Serial.println("smartWifiConfig done");
      Serial.printf("SSID:%s", WiFi.SSID().c_str());
      Serial.printf("PSW:%s", WiFi.psk().c_str());
      // break;
      delay(500);
      ESP.restart();
    }
  }
  screenState = MAIN_TEMP_SCREEN;
}

/*-------------------------------获取位置信息-------------------------------------*/
void getLBSLocation()
{
  Serial.println("getting NTP...");
  if (http_client.begin(time_req))
  {
    Serial.println("HTTPclient setUp done!");
  }

  int http_code = http_client.GET();
  Serial.println(http_code);
  if (http_code > 0)
  {
    Serial.printf("HTTP get code: %d\n", http_code);
    if (http_code == HTTP_CODE_OK)
    {
      time_rsp = http_client.getString();
      Serial.println(time_rsp);
    }
    else
    {
      Serial.printf("fail to get cityWeather,code:%d\n", http_code);
    }
  }
  deserializeJson(doc, time_rsp);
  JsonObject root = doc.as<JsonObject>();
  const String timestamp_from_ali = root["sysTime2"];
  Serial.println(timestamp_from_ali);
  locationE = 117.09;
  locationN = 36.12;
  locationA = 55;
  timeLastNTP_Y = timestamp_from_ali.substring(0, 4).toInt();
  timeLastNTP_M = timestamp_from_ali.substring(5, 7).toInt();
  timeLastNTP_D = timestamp_from_ali.substring(8, 10).toInt();
  timeLastNTP_h = timestamp_from_ali.substring(11, 13).toInt();
  timeLastNTP_m = timestamp_from_ali.substring(14, 16).toInt();
  timeLastNTP_s = timestamp_from_ali.substring(17, 19).toInt();
  timeNow_Y = timeLastNTP_Y;
  timeNow_M = timeLastNTP_M;
  timeNow_D = timeLastNTP_D;
  timeNow_h = timeLastNTP_h;
  timeNow_m = timeLastNTP_m;
  timeNow_s = timeLastNTP_s;
  Serial.printf("%d-%d-%d %d:%d:%d\r\n", timeLastNTP_Y, timeLastNTP_M, timeLastNTP_D, timeLastNTP_h, timeLastNTP_m, timeLastNTP_s);
  Serial.printf("%d-%d-%d %d:%d:%d\r\n", timeNow_Y, timeNow_M, timeNow_D, timeNow_h, timeNow_m, timeNow_s);

  rtc.adjust(DateTime(timeNow_Y, timeNow_M, timeNow_D, timeNow_h, timeNow_m, timeNow_s));
  DateTime now = rtc.now();
  now_unixtime = now.unixtime();
  // now_unixtime64 = now_unixtime;
  time_last_async_stamp = millis();
  Serial.printf("rtc time now: %d-%d-%d %d:%d:%d\r\n", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  EEPROM.writeULong(39, now_unixtime);
  EEPROM.commit();
}