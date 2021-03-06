#include "config.h"

//启动界面
void showWelcome()
{
  display.setFont(ArialMT_Plain_10);
  for (size_t i = 0; i < 64; i++)
  {
    display.clear();
    display.drawXbm(128 - 2 * i, 16, 32, 32, logo_logo);
    display.drawXbm(32 + 128 - 2 * i, 20, 24, 24, logo_an);
    display.drawXbm(56 + 128 - 2 * i, 20, 24, 24, logo_leng);
    display.drawXbm(80 + 128 - 2 * i, 20, 24, 24, logo_ke);
    display.drawXbm(104 + 128 - 2 * i, 20, 24, 24, logo_ji);
    display.drawString(12, 51 + 64 - i, "wwww.anleng-tec.com");
    display.display();
  }
  delay(800);
}
//状态栏
void showStatusBar()
{
  if (workingState == WORKING)
    display.drawIco16x16(0, 0, F16x16_rec);
  else
    display.drawIco16x16(0, 0, F16x16_space);
  if (workingState == BLE_ON && workingState != WORKING)
    display.drawIco16x16(16, 0, F16x16_bluetooth);
  else
    display.drawIco16x16(16, 0, F16x16_space);
  if (qualifiedState == QUALITIFY_RIGHT && workingState == WORKING)
    display.drawIco16x16(32, 0, F16x16_right);
  else
    display.drawIco16x16(32, 0, F16x16_space);
  if (lockState == LOCKED && workingState == WORKING)
    display.drawIco16x16(48, 0, F16x16_lock);
  else
    display.drawIco16x16(48, 0, F16x16_space);
  display.setFont(Roboto_Condensed_12);
  char timestr[20];
  snprintf(timestr, 20, "%02d:%02d:%02d", rtc.now().hour(), rtc.now().minute(), rtc.now().second());
  display.drawString(64, 0, timestr);
  display.drawIco16x16(112, 0, F16x16_b100);
  display.drawHorizontalLine(0, 16, 128);
  display.drawHorizontalLine(0, 53, 128);
  display.setFont(Dialog_plain_8);
  display.drawString(0, 56, "REC:");
  char rec_str[10];
  sprintf(rec_str,"%d",rec_count);
  display.drawString(20, 56, rec_str);
  char datestr[20];
  snprintf(datestr, 20, "%d-%d-%d", rtc.now().year(), rtc.now().month(), rtc.now().day());
  display.drawString(78, 56, datestr);
}
//温度显示界面
void showTemp()
{
  char tempParam[32];
  sprintf(tempParam, "%.1f°C", currentTemp);
  display.clear();
  showStatusBar();
  display.drawXbm(0, 20, 16, 32, temp_icon);
  display.setFont(Roboto_Condensed_38);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(72, 12, tempParam);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.display();
}
//湿度显示界面
void showHumi()
{
  char humiParam[32];
  sprintf(humiParam, "%.1f%%", currentHumi);
  display.clear();
  showStatusBar();
  display.drawXbm(0, 22, 16, 32, humi_icon);
  display.setFont(Roboto_Condensed_38);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(72, 12, humiParam);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.display();
}
//温湿度滚动界面(阻塞)
void temp_humi_Scroll()
{
  for (size_t i = 0; i < 33; i++)
  {
    char tempParam[32];
    sprintf(tempParam, "%.1f°C", currentTemp);
    char humiParam[32];
    sprintf(humiParam, "%.1f%%", currentHumi);
    display.clear();
    showStatusBar();
    if (screenState == TEMP_HUMI_SCROLL_SCREEN)
    {
      display.drawXbm(0 - i * 4, 20, 16, 32, temp_icon);
      display.setFont(Roboto_Condensed_38);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(72 - i * 4, 12, tempParam);
      display.drawXbm(0 + 128 - i * 4, 22, 16, 32, humi_icon);
      display.setFont(Roboto_Condensed_38);
      display.drawString(72 + 128 - i * 4, 12, humiParam);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
    }
    if (screenState == HUMI_TEMP_SCROLL_SCREEN)
    {
      display.drawXbm(0 - i * 4, 22, 16, 32, humi_icon);
      display.setFont(Roboto_Condensed_38);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(72 - i * 4, 12, humiParam);
      display.drawXbm(0 + 128 - i * 4, 20, 16, 32, temp_icon);
      display.setFont(Roboto_Condensed_38);
      display.drawString(72 + 128 - i * 4, 12, tempParam);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
    }
    display.display();
  }
}
//WIFI连接界面
void showWifi(int i)
{
  display.setFont(ArialMT_Plain_10);
  switch (i)
  {
  case 0:
    display.clear();
    display.drawXbm(0, 21, 16, 23, wifi_zhi);
    display.drawXbm(16, 21, 16, 23, wifi_neng);
    display.drawXbm(32, 21, 16, 23, wifi_lian);
    display.drawXbm(48, 21, 16, 23, wifi_wang);
    display.drawXbm(64, 21, 16, 23, wifi_zhong);
    display.drawXbm(96, 16, 32, 32, wifi_icon1);
    display.display();
    break;
  case 1:
    display.clear();
    display.drawXbm(0, 21, 16, 23, wifi_zhi);
    display.drawXbm(16, 21, 16, 23, wifi_neng);
    display.drawXbm(32, 21, 16, 23, wifi_lian);
    display.drawXbm(48, 21, 16, 23, wifi_wang);
    display.drawXbm(64, 21, 16, 23, wifi_zhong);
    display.drawXbm(96, 16, 32, 32, wifi_icon2);
    display.display();
    break;
  case 2:
    display.clear();
    display.drawXbm(0, 21, 16, 23, wifi_zhi);
    display.drawXbm(16, 21, 16, 23, wifi_neng);
    display.drawXbm(32, 21, 16, 23, wifi_lian);
    display.drawXbm(48, 21, 16, 23, wifi_wang);
    display.drawXbm(64, 21, 16, 23, wifi_zhong);
    display.drawXbm(96, 16, 32, 32, wifi_icon3);
    display.display();
    break;
    // case 3:
    //   display.clear();
    //   display.drawXbm(0, 22, 32, 32, wifi_icon4);
    //   display.display();
  default:
    break;
  }
}
//WIFI配网界面
void showWifiSmart()
{
  display.clear();
  display.drawXbm(0, 5, 16, 23, wifi_zhi);
  display.drawXbm(16, 5, 16, 23, wifi_neng);
  display.drawXbm(32, 5, 16, 23, wifi_pei);
  display.drawXbm(48, 5, 16, 23, wifi_wang);
  display.drawXbm(8, 34, 16, 23, wifi_qing);
  display.drawXbm(24, 34, 16, 23, wifi_sao);
  display.drawXbm(40, 34, 16, 23, wifi_ma);
  display.drawXbm(64, 0, 64, 64, wifi_smart);
  display.display();
}
//单击后的提示界面
void showTips_Screen()
{
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawXbm(24, 0, 14, 16, tips_shi2);
  display.drawXbm(44, 0, 14, 16, tips_yong);
  display.drawXbm(64, 0, 14, 16, tips_shuo);
  display.drawXbm(84, 0, 14, 16, tips_ming);
  display.drawXbm(0, 16, 14, 16, tips_chang);
  display.drawXbm(14, 16, 14, 16, tips_an);
  display.drawString(28, 16, ":");
  display.drawXbm(36, 16, 14, 16, tips_kai);
  display.drawXbm(50, 16, 14, 16, tips_shi);
  display.drawString(64, 16, "/");
  display.drawXbm(72, 16, 14, 16, tips_ting);
  display.drawXbm(86, 16, 14, 16, tips_zhi);
  display.drawXbm(100, 16, 14, 16, tips_ji);
  display.drawXbm(114, 16, 14, 16, tips_lu);
  display.drawXbm(0, 32, 14, 16, tips_shuang);
  display.drawXbm(14, 32, 14, 16, tips_ji2);
  display.drawString(28, 32, ":");
  display.drawXbm(36, 32, 14, 16, tips_jin);
  display.drawXbm(50, 32, 14, 16, tips_ru);
  display.drawXbm(64, 32, 14, 16, tips_lan);
  display.drawXbm(78, 32, 14, 16, tips_ya);
  display.drawXbm(92, 32, 14, 16, tips_pei);
  display.drawXbm(106, 32, 14, 16, tips_dui);
  display.drawCircleQuads(52, 57, 6, 6);
  display.drawHorizontalLine(52, 51, 25);
  display.drawHorizontalLine(52, 63, 25);
  display.drawCircleQuads(76, 57, 6, 9);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Roboto_Condensed_12);
  display.drawString(64, 50, "OK");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.display();
}
//双击后的蓝牙界面
void showBLEconnect_Screen()
{
  display.clear();
  display.setFont(Roboto_Condensed_38);
  display.drawString(0, 0, "BLE");
  display.display();
}
//长按后的界面
void show_recStart_Screen()
{
  display.clear();
  display.drawHorizontalLine(0, 10, 80);
  display.drawHorizontalLine(48, 54, 80);
  display.drawXbm(4, 20, 24, 24, tips_recon);
  display.drawXbm(28, 20, 24, 24, tips_kai3);
  display.drawXbm(52, 20, 24, 24, tips_shi3);
  display.drawXbm(76, 20, 24, 24, tips_ji3);
  display.drawXbm(100, 20, 24, 24, tips_lu3);
  display.display();
}

void show_recStop_Screen()
{
  display.clear();
  display.drawHorizontalLine(0, 10, 80);
  display.drawHorizontalLine(48, 54, 80);
  display.drawXbm(4, 20, 24, 24, tips_recoff);
  display.drawXbm(28, 20, 24, 24, tips_ting3);
  display.drawXbm(52, 20, 24, 24, tips_zhi3);
  display.drawXbm(76, 20, 24, 24, tips_ji3);
  display.drawXbm(100, 20, 24, 24, tips_lu3);
  display.display();
}
//屏幕状态机循环
void screen_loop()
{
  if (screen_loopEnabled)
  {
    loopnowTime = millis();
    looptimeSpan = loopnowTime - loopStartTime;
    switch (looptimeSpan)
    {
    case 0 ... 5000:
      screenState = MAIN_TEMP_SCREEN;
      break;
    case 5001 ... 5200:
      screenState = TEMP_HUMI_SCROLL_SCREEN;
      break;
    case 5201 ... 9800:
      screenState = MAIN_HUMI_SCREEN;
      break;
    case 9801 ... 10000:
      screenState = HUMI_TEMP_SCROLL_SCREEN;
      break;
    case 10001 ... INT_MAX:
      loopStartTime = millis();
      screenState = MAIN_TEMP_SCREEN;
      break;
    default:
      break;
    }
  }
}
void screen_show()
{
  switch (screenState)
  {
  case MAIN_TEMP_SCREEN:
    showTemp();
    break;
  case TEMP_HUMI_SCROLL_SCREEN:
    temp_humi_Scroll();
    break;
  case MAIN_HUMI_SCREEN:
    showHumi();
    break;
  case HUMI_TEMP_SCROLL_SCREEN:
    temp_humi_Scroll();
    break;
  case WIFI_CONNECT_SCREEN:
    showWifi(4);
    break;
  case WIFI_SMART_SCREEN:
    showWifiSmart();
    break;
  case TIPS_SCREEN:
    keyScreen_Now = millis();
    if (keyScreen_Now - keyScreen_Start < show_tip_screen_last)
      showTips_Screen();
    else
      screen_loopEnabled = true;
    break;
  case BLE_SCREEN:
    keyScreen_Now = millis();
    if (keyScreen_Now - keyScreen_Start < show_BLE_screen_last)
      showBLEconnect_Screen();
    else
      screen_loopEnabled = true;
    break;
  case REC_START_SCREEN:
    show_recStart_Screen();
    break;
  case REC_STOP_SCREEN:
    keyScreen_Now = millis();
    if (keyScreen_Now - keyScreen_Start < show_rec_stop_screen_last)
      show_recStop_Screen();
    else if (keyState != LONGPRESS_DURRING)
    {
      screen_loopEnabled = true;
    }
    break;
  default:
    break;
  }
}