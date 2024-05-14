
//aux serial reader
String fxs_getstr(String query){
  Serial.println();
  Serial.println(query);
  while (!Serial.available()) {} //wait for data available
  String ret = Serial.readString();  //read until timeout
  ret.trim();
  Serial.println(ret);
  return ret;
}
int fxs_getint(String query){
    Serial.println();
    Serial.println(query);
    Serial.setTimeout(5000);
    return Serial.parseInt();
}

