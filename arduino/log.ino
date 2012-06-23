

#include <Dns.h>

char  serverName[] = "philippe.lang2.free.fr";
IPAddress  freeperso ; 
EthernetClient  client;
IPAddress  freeperso2 =(212,27,63,128) ; 

void InitLog(void){
  DNSClient dns;
  dns.begin(Ethernet.dnsServerIP());
  dns.getHostByName(serverName, freeperso);

}

void LogOnInternet(int level,char *text, char *user){
    if (client.connect(freeperso, 80)) {
      client.print("GET /cloudSensor/php/logToit.php?level=");
      client.print(level);
      client.print("&text=");
      client.print(text);
      client.print("&user=");
      client.print(user);
      client.println(" HTTP/1.1");
      client.println("HOST: philippe.lang2.free.fr");
      client.println("\n");
      client.println("\n");
      client.flush();
      
       delay (50);
    client.stop();
    client.flush();
 }
}

  

