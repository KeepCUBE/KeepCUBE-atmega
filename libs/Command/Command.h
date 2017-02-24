class Command {
  public:
    // Constructors
    Command(char starting, char ending);
    Command(String m);
    Command(char &m);
    Command();
    
    void set(String s);
    void readSerial();
    String getIdentifier();
    void setIdentifier(String identifier);
    String getParam(const char parameterIdentifier);
    void setParam(const char parameterIdentifier, int value);
    void setParam(const char parameterIdentifier, float value);
    void setParam(const char parameterIdentifier, String value);
    void delParam(const char parameterIdentifier);
    bool hasParam(const char parameterIdentifier);
    
    
    String toString();
    
    //bool verify();
    
    
    
    
  private:
  
    String msg = "";
  
    bool isUppercaseLetter(char m);
    
    bool ok = false;
    bool multiple = false;
    bool incorrect = false;

    char startChar = '#';
    char endChar = ';';
    
    
    
};




Command::Command(char starting, char ending): startChar(starting), endChar(ending) {}
Command::Command(String m): msg(m) {}
Command::Command(char &m): msg(String(m)) {}
Command::Command() {}


void Command::readSerial() {
    char letter;
    //long length = 0;
    long hashes = 0;
    long semicolons = 0;
    String message = "";
    
    //deleting unwanted letters before command
    while ((char)Serial.peek() != startChar && Serial.available() > 0) {
        Serial.read();
    }
    
    //reading command, detecting number of letters, hashes and semicolons,
    do {
        //length++;
        letter = Serial.read();
        
        message += letter;
        
        if (letter == startChar) {
            hashes++;
        }
        if (letter == endChar) {
            semicolons++;
        }
        
    } while (semicolons != hashes && Serial.available() > 0);
    
    //remove unwanted chars after command, checking if next char is start of new command
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
        //return message;
    }
    else if (semicolons == hashes && hashes == 1) {
        ok = true;
        multiple = false;
        incorrect = false;
        msg = message;
        //return message;
    }
    else {
        ok = false;
        multiple = false;
        incorrect = true;
        
        //return "incorrect";
        //return NULL;
    }
}

String Command::getIdentifier() {
    return (String)msg.charAt(1) + (String)msg.charAt(2) + (String)msg.charAt(3);
}

void Command::setIdentifier(String identifier) {
    if (isUppercaseLetter(identifier.charAt(0)) && isUppercaseLetter(identifier.charAt(1)) && isUppercaseLetter(identifier.charAt(2))) {
        if (msg == "") {
            msg = startChar + identifier + endChar;
        }
        else {
            msg.setCharAt(1, identifier.charAt(0));
            msg.setCharAt(2, identifier.charAt(1));
            msg.setCharAt(3, identifier.charAt(2));
        }
    }
}

String Command::getParam(const char parameterIdentifier) {
    int length = msg.length() + 1;
    int start, round = 0;
    //bool cmdAsParameter = false;
    
    // Find beginning of parameter
    for (int i = 4; i < length; i++) {
        if ((char)msg.charAt(i) == startChar) {
            while ((char)msg.charAt(i) != endChar) {
                i++;
				// TODO: cmdAsParameter = true;
            }
        }
        
        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) {
            start = i + 1;
            break;
        }
    }
    
    //start = msg.substring(4).indexOf(parameterIdentifier) + 1;
    
      
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

void Command::set(String s) {
    msg = s;
}

void Command::setParam(const char parameterIdentifier, int value) {
    setParam(parameterIdentifier, (String)value);
}

void Command::setParam(const char parameterIdentifier, float value) {
    setParam(parameterIdentifier, (String)value);
}

void Command::setParam(const char parameterIdentifier, String value) {
    if (msg == "" || msg.length() < 5) return;

    for (int i = 4; i <= msg.length(); i++) {
        if ((char)msg.charAt(i) == startChar) {
            while ((char)msg.charAt(i) != endChar) {
                i++;
                // TODO: cmdAsParameter = true;
            }
        }

        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) {
            String a = msg.substring(0, i + 1);
            String b = msg.substring(i + 1);

            while (!isUppercaseLetter(b.charAt(0))) {
                if (b.charAt(0) == endChar) break;
                b.remove(0, 1);
            }

            msg = a + value + b;
            return;
        }
    }

    msg.remove(msg.length() - 1);
    msg = msg + (String)parameterIdentifier + value + (String)endChar;
}

void Command::delParam(const char parameterIdentifier) {
    if (msg == "" || msg.length() < 7) return;

    for (int i = 4; i <= msg.length(); i++) {
        if ((char)msg.charAt(i) == startChar) {
            while ((char)msg.charAt(i) != endChar) {
                i++;
                // TODO: cmdAsParameter = true;
            }
        }

        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) {
            while (!isUppercaseLetter(msg.charAt(i + 1))) {
                if (msg.charAt(i) == endChar) return;
                msg.remove(i, 1);
            }
            msg.remove(i, 1);
            return;
        }
    }
}

bool Command::hasParam(const char parameterIdentifier) {
    for (int i = 4; i <= msg.length(); i++) {
        if ((char)msg.charAt(i) == startChar) {
            while ((char)msg.charAt(i) != endChar) {
                i++;
                // TODO: cmdAsParameter = true;
            }
        }

        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) return true;
    }
    return false;
}




bool Command::isUppercaseLetter(char m) {
    return m >= 65 && m <= 90;
}

String Command::toString() {
    return (String)msg;
}
