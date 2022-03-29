## Versione casalinga

In questa cartella è raccolto il codice per far funzionare il progetto casalingo, che sfrutta un modulo WiFi ESP32 per comunicare con il wifi di casa.

### Perchè usiamo la scheda ESP32 invece di Arduino?

Perchè la scheda ESP32 consente di fare le stesse cose, ma ha un più un modulo WiFi e un modulo Bluetooth integrati. A dire il vero, è più potente di Arduino, in quanto è dual core e possiede una memoria flash. In pratica ha tutto quello che ci serve in una soluzione compatta.

## Componenti da acquistare (DRAFT)

Di seguito vi proponiamo le componenti minime da acquistare per realizzare il progetto. I link sono solo dei riferimenti su cosa acquistare, qualunque componente equivalente andrà bene:

- ESP 32 Devkit V1 (attenzione a comprare il Devkit V1 e non altre versioni)
    - [Opzione 1](https://amzn.to/3I1UWo7)
    - [Opzione 2](https://bit.ly/3rLBN3c) (può impiegare qualche settimana ad arrivare)
- [Cavi, breadboard etc.](https://amzn.to/3I0vpMa)
    - Se oltre agli accessori volete anche un Arduino Uno (usato al posto della ESP 32 nel progetto dell'ufficio, ed è anche molto più user friendly), vi suggeriamo di prendere invece [questo kit](https://amzn.to/3qS6Tag)
- [Sensore di umidità](https://amzn.to/33fi18c) (attenzione a quale sensore prendete, questo che proponiamo è resistente alla corrosione)
- [Pompe per l'acqua](https://amzn.to/33sKqHE)
- Qualunque set di relay va bene, ad esempio [Relay](https://amzn.to/3tpK53x) ma se pensate che ve ne servano di meno si trovano molti prodotto equivalenti
- Power bank/batteria

# Creare i certificati SSL per MQTT

Nella cartella `homeassistant/mosquitto/certs` c'è lo script `certificates.sh` che serve a creare dei certificati SSL usando OpenSSL in modo da crittografare la comunicazione tra il broker MQTT e i client. Una volta girato lo script, scegliete le password e i parametri dei certificati come preferite, facendo attenzione a:

- Organizational Unit Name: verrà chiesto 2 volte, questi due nomi devono essere diversi
- Common Name: dev'essere il DNS name del pc dove fate girare mosquitto

Una volta creati i certificati, copiate `ca.crt` nella cartella `homeassistant`.

Fate ripartire tutte le docker instance e andate su homeassistant. Andate nelle impostazioni, aggiungete una nuova integrazione, cercate "mqtt" e scegliete la con figurazione già presente (la prima), poi inserite i dati così come scritti in `secrets.yaml`.