class Command {
  public:
    // Constructors
    Command(char starting, char ending);
    Command(String m);
    Command(char &m);
    Command();
    
    void readSerial();
    String getIdentifier();
    String getParameter(int nth);
    String getParameterValue(const char parameterIdentifier);
    
    String toString();
    
    //bool verify();
    
    
    String msg = "";
    
  private:
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



// Get n-th parameter of command
String Command::getParameter(int nth) {
    int length = msg.length() + 1;
    int start, round = 0;
    //bool cmdAsParameter = false;
    
    // Find beginning of parameter
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


