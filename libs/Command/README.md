Konstruktory
---
- `Command cmd()` vytvoří prázdný objekt.
- `Command cmd(String)` vytvoří objekt s dodaným obsahem (typu String)
- `Command cmd(&char)` vytvoří objekt s dodaným obsahem (typu char array)


Metody
---
- `void set(String)` natvrdo nastaví command na dodaný String 
- `void readSerial()` načte jeden command ze serial bufferu
- `String getIdentifier()` vrátí identifikátor příkazu (první 3 velká písmena)
- `void setIdentifier(String)` nastaví identifikátor příkazu
- `String getParam(const char)` vrátí obsah parametru jako string. Po přidání .toInt() na konec bude vracet hodnotu jako int.
- `void setParam(const char, int)` nastaví hodnotu parametru. Pokud tam parametr s požadovaným názvem není, vytvoří nový.
- `void setParam(const char, String)` to samé, jen se Stringem.
- `void delParam(const char)` smaže zvolený parametr
- `bool hasParam(const char)` zjistí, zda se daný parametr nachází v příkazu
- `String toString();` vrátí parametr jako String
