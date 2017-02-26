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
    
    String encodeString(String str);
    String decodeString(String str);
  
    
    
    
  private:
  
    String msg = "";
    
    
    
    
    
    bool isHex(String str);
    bool isUppercaseLetter(char m);
    
    bool ok = false;
    bool multiple = false;
    bool incorrect = false;
    
    char startChar = '#';
    char endChar = ';';
    char wrapChar = '&';
    char escChar = '\\';
    
    
    
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
    bool strAsParam = false;
    
    // Find beginning of parameter
    for (int i = 4; i < length; i++) {
        if (msg.charAt(i) == wrapChar) {
            i++;
            while (msg.charAt(i) != wrapChar && msg.charAt(i - 1) == escChar) {
                i++;
            }
        }
        
        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) {
            msg.charAt(i + 1) == wrapChar ? strAsParam = true : strAsParam = false;
            start = i;
            break;
        }
    }
    
    String param = "";
    for (int i = start + 1; !isUppercaseLetter(msg.charAt(i)); i++) {
        if (strAsParam) {
            while (true) {
                param += (String)msg.charAt(i);
                i++;
                //Serial.print(msg.charAt(i));
                
                if (msg.charAt(i) == wrapChar && msg.charAt(i - 1) != escChar && isUppercaseLetter(msg.charAt(i + 1))) {
                    param += (String)msg.charAt(i);
                    break;
                }
            }
            return decodeString(param);
        }
        
        if (msg.charAt(i) == endChar) {
            break;
        }
        else if (msg.charAt(i) == startChar) {
            for (int j = i - 1; msg.charAt(j) != endChar; j++) {
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
    if (msg == "" || msg.length() < 5) return;

    for (int i = 4; i <= msg.length(); i++) {
        if (msg.charAt(i) == wrapChar) {
            i++;
            while (msg.charAt(i) != wrapChar && msg.charAt(i - 1) == escChar) {
                i++;
            }
        }

        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) {
            String a = msg.substring(0, i + 1);
            String b = msg.substring(i + 1);

            while (!isUppercaseLetter(b.charAt(0))) {
                
                
                if (b.charAt(0) == escChar && b.charAt(1) == wrapChar) {
                    b.remove(0, 1);
                    b.remove(0, 1);
                } else {
                    b.remove(0, 1);
                }
                
                if (b.charAt(0) == endChar) break;
                
                if (b.charAt(0) == wrapChar) {
                    b.remove(0, 1);
                    break;
                }
            }

            msg = a + (String)value + b;
            return;
        }
    }

    msg.remove(msg.length() - 1);
    msg = msg + (String)parameterIdentifier + value + (String)endChar;
}


void Command::setParam(const char parameterIdentifier, float value) {
    
}


void Command::setParam(const char parameterIdentifier, String value) {
    if (msg == "" || msg.length() < 5) return;

    for (int i = 4; i <= msg.length(); i++) {
        if (msg.charAt(i) == wrapChar) {
            i++;
            while (msg.charAt(i) != wrapChar && msg.charAt(i - 1) == escChar) {
                i++;
            }
        }
        
        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) {
            String a = msg.substring(0, i + 1);
            String b = msg.substring(i + 1);

            while (!isUppercaseLetter(b.charAt(0))) {
                
                
                if (b.charAt(0) == escChar && b.charAt(1) == wrapChar) {
                    b.remove(0, 1);
                    b.remove(0, 1);
                } else {
                    b.remove(0, 1);
                }
                
                if (b.charAt(0) == endChar) break;
                
                if (b.charAt(0) == wrapChar) {
                    b.remove(0, 1);
                    break;
                }
            }

            msg = a + encodeString(value) + b;
            return;
        }
    }

    msg.remove(msg.length() - 1);
    msg = msg + (String)parameterIdentifier + encodeString(value) + (String)endChar;
}


void Command::delParam(const char parameterIdentifier) {
    if (msg == "" || msg.length() < 7) return;

    for (int i = 4; i <= msg.length(); i++) {
        if (msg.charAt(i) == wrapChar) {
            i++;
            while (msg.charAt(i) != wrapChar && msg.charAt(i - 1) == escChar) {
                i++;
            }
        }

        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) {
            if (msg.charAt(i + 1) == wrapChar) {
                while(true) {
                    if (msg.charAt(i + 2) == wrapChar && msg.charAt(i + 1) != escChar) {
                        break;
                    }
                    msg.remove(i, 1);
                }
                msg.remove(i, 1);
                msg.remove(i, 1);
                msg.remove(i, 1);
                return;
            }
            
            
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
        if (msg.charAt(i) == wrapChar) {
            i++;
            while (msg.charAt(i) != wrapChar && msg.charAt(i - 1) == escChar) {
                i++;
            }
        }

        if (isUppercaseLetter(msg.charAt(i)) && msg.charAt(i) == parameterIdentifier) return true;
    }
    return false;
}


/*
&asd\&fuuausud\\gudsa\dag&
 asd&fuuausud\gudsa\dag
&asd\&fuuausud\\gudsa\\dag&
*/


String Command::encodeString(String str) {
    if (isHex(str)) return str;
    int index = 0;
    
    for (int i = 0; i < str.length(); i++) {
        
        if (str.charAt(i) == wrapChar && str.charAt(i - 1) != escChar) {
            str = str.substring(0, i) + escChar + str.substring(i);
        }
        
        if (str.charAt(i) == escChar && str.charAt(i - 1) != escChar && str.charAt(i + 1) != wrapChar) {
            str = str.substring(0, i) + escChar + str.substring(i);
        }
        
    }
    
    return wrapChar + str + wrapChar;
}


String Command::decodeString(String str) {
    if (isHex(str)) return str;
    if (!str.startsWith((String)wrapChar) && !str.endsWith((String)wrapChar)) return str;
    
    int index = 0;
    str.remove(0, 1);
    str.remove(str.length() - 1);
    
    while (index != -1) {
        index = str.indexOf(escChar, index + 1);
        if (str.charAt(index + 1) == wrapChar || str.charAt(index + 1) == escChar) {
            str.remove(index, 1);
        }
    }
    
    return str;
}


bool Command::isUppercaseLetter(char m) {
    return m >= 65 && m <= 90;
}


bool Command::isHex(String str) {
    char c;
    for (int i = 0; i < str.length(); i++) {
        c = str.charAt(i);
        if (!(c >= 48 && c <= 57 || c >= 97 && c <= 102)) {
            return false;
        }
    }
    return true;
}


String Command::toString() {
    return (String)msg;
}
