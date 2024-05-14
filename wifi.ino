

void wifi_connect(std::string, std::string);
void wifi_connect(std::string, std::string, std::string);
String httpGETRequest(const char*, const char*, int);
String httpGETRequest(const char*, String, int);

void wifi_verify() {
  Serial.print("Waiting");
  digitalWrite(LED_BUILTIN, LOW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void wifi_connect(const char* ssid, const char* pwd) {
  Serial.print(ssid);
  Serial.print(" : ");
  Serial.println( pwd);
  WiFi.begin(ssid, pwd);
  wifi_verify();
}

void wifi_connect(const char* ssid, const char* usr, const char* pwd) {
  printf("Connecting to %s usr %s pwd %s", ssid, usr, pwd);
  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);

  wifi_station_set_config(&wifi_config);

  // Clean up to be sure no old data is still inside
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_set_wpa2_enterprise_auth(1);

  wifi_station_set_enterprise_username((uint8*)usr, strlen(usr));
  wifi_station_set_enterprise_identity((uint8*)usr, strlen(usr));
  wifi_station_set_enterprise_password((uint8*)pwd, strlen(pwd));

  wifi_station_connect();
}


String httpGETRequest(const char* host, String tail, int port = 442) {
  return httpGETRequest(host, tail.c_str(), port);
}

//this can be better, perhaps return full http responses
String httpGETRequest(const char* host, const char* tail, int port = 442) {
  WiFiClientSecure client;
  HTTPClient http;
  String ret;

  client.setInsecure();
  //Serial.print(host);
  //Serial.println(tail);

  http.begin(client, String(host) + tail);
  Serial.println(String(http.GET()));

  ret = http.getString();
  //Serial.println(ret);
  http.end();
  return ret;
}
