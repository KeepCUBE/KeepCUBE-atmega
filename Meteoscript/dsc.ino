Command::Command(char starting, char ending): startChar(starting), endChar(ending) {}
Command::Command(String m): msg(m) {}
Command::Command(char &m): msg(String(m)) {}
Command::Command() {}

void Command::readSerial() {
  char letter;
  long hashes = 0;
  long semicolons = 0;
  String message = "";
  while ((char)Serial.peek() != startChar && Serial.available() > 0) {
    Serial.read();
  }
  do {
    letter = Serial.read();
    message += letter;
    if (letter == startChar) {
      hashes++;
    }
    if (letter == endChar) {
      semicolons++;
    }
  } while (semicolons != hashes && Serial.available() > 0);
  while (Serial.available() != 0) {
    Serial.read();
    if ((char)Serial.peek() == startChar) {
      break;
    }
  }
  if (semicolons == hashes && hashes > 1) {
    ok = false;
    multiple = true;
    incorrect = false;
    msg = message;
  }
  else if (semicolons == hashes && hashes == 1) {
    ok = true;
    multiple = false;
    incorrect = false;
    msg = message;
  }
  else {
    ok = false;
    multiple = false;
    incorrect = true;
  }
}
String Command::getIdentifier() {
  return (String)msg.charAt(1) + (String)msg.charAt(2) + (String)msg.charAt(3);
}
String Command::getParameterValue(const char parameterIdentifier) {
  String str;

  if (isUppercaseLetter(parameterIdentifier)) {
    for (int i = 0; str != "#"; i++) {
      str = getParameter(i);
      if (str.charAt(0) == parameterIdentifier) {
        str.remove(0, 1);
        return str;
      }
    }
  }
  return "";
}
String Command::getParameter(int nth) {
  int length = msg.length() + 1;
  int start, round = 0;
  for (int i = 4; i < length; i++) {
    if ((char)msg.charAt(i) == startChar) {
      while ((char)msg.charAt(i) != endChar) {
        i++;
      }
    }
    if (isUppercaseLetter(msg.charAt(i))) {
      if (round == nth) {
        start = i;
        break;
      }
      round++;
    }
  }
  String param = (String)msg.charAt(start);
  for (int i = start + 1; !isUppercaseLetter(msg.charAt(i)); i++) {
    if ((char)msg.charAt(i) == endChar) {
      break;
    }
    else if ((char)msg.charAt(i) == startChar) {
      for (int j = i - 1; (char)msg.charAt(j) != endChar; j++) {
        param += (String)msg.charAt(j + 1);
      }
    }
    else {
      param += (String)msg.charAt(i);
    }
  }
  return param;
}
bool Command::isUppercaseLetter(char m) {
  return m >= 65 && m <= 90;
}
String Command::toString() {
  return (String)msg;
}
