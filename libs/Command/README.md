Konstruktory
---
`Command cmd()` vytvoří prázdný objekt.

`Command cmd(String m)` vytvoří objekt s dodaným obsahem (typu String)

Metody
---
`void set(String s)` natvrdo nastaví command na dodaný String 

`void readSerial()` načte jeden command ze serial bufferu

`String getIdentifier()` vrátí identifikátor příkazu (první 3 velká písmena)

`void setIdentifier(String identifier)` nastaví identifikátor příkazu

`String getParam(const char parameterIdentifier)` vrátí obsah parametru jako string. Po přidání .toInt() na konec bude vracet hodnotu jako int.

`void setParam(const char parameterIdentifier, int value)` nastaví hodnotu parametru. Pokud tam parametr s požadovaným názvem není, vytvoří nový.

`void setParam(const char parameterIdentifier, String value)` to samé, jen se Stringem.

`void delParam(const char parameterIdentifier)` smaže zvolený parametr

`bool hasParam(const char parameterIdentifier)` zjistí, zda se daný parametr nachází v příkazu

`String toString();` vrátí parametr jako String
