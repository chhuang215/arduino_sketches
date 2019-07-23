#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 8
void setup(void)
{
  DHT11.read(DHT11PIN);
  Serial.begin(9600);
}
void loop(void)
{
  Serial.print("TMEP=");
  Serial.print(DHT11.temperature);
  Serial.print(" C");
  Serial.print('\t');
  Serial.print("HUM=%");
  Serial.println(DHT11.humidity);
  delay(500);
  exit(0);
}
