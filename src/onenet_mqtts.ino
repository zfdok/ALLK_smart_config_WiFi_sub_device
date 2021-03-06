#include "config.h"

/*-------------------------------连接平台-------------------------------*/
void onenet_connect()
{
  //连接OneNet并上传数据
  Serial.println("connecting to OneNet IOT...");
  client.setServer(mqtt_server, port);                   //设置客户端连接的服务器,连接Onenet服务器, 使用6002端口
  client.connect(mqtt_devid, mqtt_pubid, mqtt_password); //客户端连接到指定的产品的指定设备.同时输入鉴权信息
}
/*-------------------------------向云平台发送温湿度数据-------------------------------*/
void sendTempAndHumi()
{
  if (client.connected())
  {
    //先拼接出json字符串
    char param[178];
    char jsonBuf[256];
    Serial.println("tempLimit_enable");
    Serial.println(tempLimit_enable);
    Serial.println("tempUpperLimit");
    Serial.println(tempUpperLimit);
    Serial.println("tempLowerLimit");
    Serial.println(tempLowerLimit);
    if (tempLimit_enable)
    {
      (tempUpperLimit < currentTemp) ? tempUA++ : tempUA = 0;
      (tempLowerLimit > currentTemp) ? tempLA++ : tempLA = 0;
    }
    else
    {
      tempUA = tempLA = 0;
    }
    if (current_rec_State == START_RECING)
    {
      sprintf(param, "{\"temp\":{\"value\":%.2f},\"humi\":{\"value\":%.2f},\"start_time\":{\"value\":%u000},\"tempUA\":{\"value\":%ld},\"tempLA\":{\"value\":%ld}}", currentTemp, currentHumi, now_unixtime, tempUA, tempLA); //我们把要上传的数据写在param里
    }
    else
    {
      sprintf(param, "{\"temp\":{\"value\":%.2f},\"humi\":{\"value\":%.2f},\"last_time\":{\"value\":%u000},\"tempUA\":{\"value\":%ld},\"tempLA\":{\"value\":%ld}}", currentTemp, currentHumi, now_unixtime, tempUA, tempLA); //我们把要上传的数据写在param里
    }
    sprintf(jsonBuf, ONENET_POST_BODY_FORMAT, param);
    //再从mqtt客户端中发布post消息
    if (client.publish(ONENET_TOPIC_PROP_POST, jsonBuf))
    {
      Serial.print("Post message to cloud: ");
      Serial.println(jsonBuf);
      current_rec_State = KEEP_RECING;
      OneNet_connected_Time = millis();
      rec_count++;
    }
    else
    {
      Serial.println("Publish message to cloud failed!");
    }
  }
}

void getDesired()
{
  if (client.connected())
  {
    client.subscribe(ONENET_TOPIC_DESIRED_GET);
    client.subscribe(ONENET_TOPIC_DESIRED_GET_RE);
    client.setCallback(callback);
    //先拼接出json字符串
    char param[164];
    char jsonBuf[356];
    sprintf(param, "[\"temp_alarm\",\"tempL\",\"tempU\",\"period\"]"); //我们把要上传的数据写在param里
    postMsgId += 1;
    sprintf(jsonBuf, ONENET_POST_BODY_FORMAT, param);
    //再从mqtt客户端中发布post消息
    if (client.publish(ONENET_TOPIC_DESIRED_GET, jsonBuf))
    {
      Serial.print("Post message to cloud: ");
      Serial.println(jsonBuf);
    }
    else
    {
      Serial.println("Publish message to cloud failed!");
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("message rev:");
  Serial.println(topic);
  for (size_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (strstr(topic, ONENET_TOPIC_DESIRED_GET))
  {
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
      Serial.println("parse json failed");
      return;
    }
    JsonObject setAlinkMsgObj = doc.as<JsonObject>();
    tempLimit_enable = setAlinkMsgObj["data"]["temp_alarm"]["value"];
    tempUpperLimit = setAlinkMsgObj["data"]["tempU"]["value"];
    tempLowerLimit = setAlinkMsgObj["data"]["tempL"]["value"];
    sleeptime = ((int)setAlinkMsgObj["data"]["period"]["value"]) * 60000000;
    eeprom_config_set(tempLimit_enable, (uint32_t)sleeptime, tempUpperLimit, tempLowerLimit);

    // if (tempLimit_enable)
    // {
    //   if (tempUpperLimit < currentTemp || tempLowerLimit > currentTemp)
    //   {
    //     char param[178];
    //     char jsonBuf[256];
    //     if (tempUpperLimit < currentTemp)
    //     {
    //       sprintf(param, "{\"tempUA\":{\"value\":%ld}}", ++tempUA);
    //     }
    //     if (tempLowerLimit > currentTemp)
    //     {
    //       sprintf(param, "{\"tempLA\":{\"value\":%ld}}", ++tempLA);
    //     }
    //     sprintf(jsonBuf, ONENET_POST_BODY_FORMAT, param);
    //     if (client.publish(ONENET_TOPIC_PROP_POST, jsonBuf))
    //     {
    //       Serial.print("Post message to cloud: ");
    //       Serial.println(jsonBuf);
    //     }
    //     else
    //     {
    //       Serial.println("Publish message to cloud failed!");
    //     }
    //   }
    //   else
    //   {
    //     tempUA = tempLA = 0;
    //     char aparam[178];
    //     char ajsonBuf[256];
    //     sprintf(aparam, "{\"tempLA\":{\"value\":%d},\"tempUA\":{\"value\":%d}}", 0, 0);
    //     sprintf(ajsonBuf, ONENET_POST_BODY_FORMAT, aparam);
    //     if (client.publish(ONENET_TOPIC_PROP_POST, ajsonBuf))
    //     {
    //       Serial.print("Post message to cloud: ");
    //       Serial.println(ajsonBuf);
    //     }
    //     else
    //     {
    //       Serial.println("Publish message to cloud failed!");
    //     }
    //   }
    // }
  }
}