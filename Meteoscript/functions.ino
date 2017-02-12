void get_temperature(String requestid) {
  float values[20];
  for (int i = 0; i < 10; i++) {
    values[i] = bme.readTempC();
    values[i + 10] = htu.readTemperature();
  }
  send_info(requestid, average(values));
}
void get_pressure(String requestid) {
  float values[10];
  for (int i = 0; i < 10; i++) {
    values[i] = bme.readFloatPressure();
  }
  send_info(requestid, average(values));
}
void get_humidity(String requestid) {
  float values[10];
  for (int i = 0; i < 10; i++) {
    values[i] = htu.readHumidity();
  }
  send_info(requestid, average(values));
}
float average(float arr[]) {
  float sum;
  for (int i = 0; i < sizeof(arr); i++) {
    sum += arr[i];
  }
  return sum / sizeof(arr);
}
void send_info(String requestid, float value) {
  Serial.print("#SRV");
  Serial.print("R");
  Serial.print(requestid);
  Serial.print("V");
  Serial.print(value);
  Serial.println(";");
}

