#include &lt;TinyGPS++.h&gt;
#include &lt;SoftwareSerial.h&gt;
SoftwareSerial ss(3,4);
//SoftwareSerial mySerial(12,13);
#include&lt;dht.h&gt;
#define dht11 A0
#define gas A1
#define buzzer 2
TinyGPSPlus gps;
float lat = 18.5206,lon = 78.6310; // create variable for latitude and longitude object
dht DHT;
String latitude=&quot;17.438249&quot;,longitude=&quot;78.445030&quot;;
int temp,humi,gas_value;
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
 // mySerial.begin(9600);
   pinMode(buzzer,OUTPUT);
}
void loop()
{
  //sendMessage();
  while (ss.available() &gt; 0)
    if (gps.encode(ss.read()))
    {
      displayInfo();
     
    }
}
void displayInfo()
{
  Serial.print(F(&quot;Location: &quot;));
  if (gps.location.isValid())
  {
    latitude=String(gps.location.lat(), 6);
    longitude=String(gps.location.lng(), 6);
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(&quot;,&quot;));
    Serial.print(gps.location.lng(), 6);
     data();
  }
  else
  {
    Serial.print(F(&quot;INVALID&quot;));

  }
  Serial.print(F(&quot;  Date/Time: &quot;));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F(&quot;/&quot;));
    Serial.print(gps.date.day());
    Serial.print(F(&quot;/&quot;));
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println(F(&quot;INVALID&quot;));
  }
}
void data()
{
    DHT.read11(dht11);
  temp=DHT.temperature;
  humi=DHT.humidity;
  gas_value=analogRead(gas);
  Serial.print(&quot;Temperature : &quot;);
  Serial.print(temp);
  Serial.println(&quot;C&quot;);
  Serial.print(&quot;Humidity :&quot;);
  Serial.println(humi);
  Serial.print(&quot;Gas Level : &quot;);
  Serial.println(gas_value);
  if(temp&gt;35)
  {
    Serial.println(F(&quot;Temperature HIGH&quot;));
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    sendMessage(1);
  }
  if(gas_value&gt;110)
  {
    Serial.println(F(&quot;Gas level HIGH&quot;));
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    sendMessage(2);
  }
  delay(2000);
  Send2thing();
}
void sendMessage(int i)

{
  String msg;//=&quot;Alert! \n &quot;;
  msg+=&quot;https://www.google.co.in/maps/place/&quot;;
   msg+=latitude+&quot;,&quot;+longitude;
  switch(i)
  {
    case 1:msg+=&quot;\nTemperature HIGH&quot;;break;
    case 2:msg+=&quot;\nGas level HIGH&quot;;break;
  }
  Serial.println(&quot;AT+CMGF=1&quot;);//set the GSM Module in Text mode
  delay(1000);
  Serial.println(&quot;AT+CMGS=\&quot;+917013447604\&quot;\r&quot;);
  delay(1000);
  Serial.println(msg);//the SMS text you want to send
  delay(100);
  Serial.println((char)26);
  delay(100);
}
void Send2thing()
{
  Serial.println(&quot;AT&quot;);
  delay(1000);
  Serial.println(&quot;AT+CPIN?&quot;);
  delay(1000);
  Serial.println(&quot;AT+CREG?&quot;);
  delay(1000);
  Serial.println(&quot;AT+CGATT?&quot;);
  delay(1000);
  Serial.println(&quot;AT+CIPSHUT&quot;);
  delay(1000);
  Serial.println(&quot;AT+CIPSTATUS&quot;);
  delay(2000);
  Serial.println(&quot;AT+CIPMUX=0&quot;);
  delay(2000);
  Serial.println(&quot;AT+CSTT=\&quot;ideagprs.com\&quot;&quot;);//start task and setting the APN
  delay(1000);
   Serial.println(&quot;AT+CIICR&quot;);//bring up wireless connection
  delay(3000);
   Serial.println(&quot;AT+CIFSR&quot;);//get local IP adress
  delay(2000);
   Serial.println(&quot;AT+CIPSPRT=0&quot;);
  delay(3000);
   Serial.println(&quot;AT+CIPSTART=\&quot;TCP\&quot;,\&quot;api.thingspeak.com\&quot;,\&quot;80\&quot;&quot;);//start up the connection

  delay(5000);
 
  Serial.println(&quot;AT+CIPSEND&quot;);//begin send data to remote server
 
  delay(500);
  String str=&quot;GET http://api.thingspeak.com/update?api_key=PWRRSCYYTJGGY5T0&amp;field1=&quot; + String(temp);
  str+=&quot;&amp;field2=&quot;;
  str+=String(humi);
  str+=&quot;&amp;field3=&quot;;
  str+=String(gas_value);  
  Serial.println(str);//begin send data to remote server
  delay(500);
  Serial.println((char)26);//sending
  delay(2000);//waiting for reply, important! the time is base on the condition of internet
  Serial.println();
 
  Serial.println(&quot;AT+CIPSHUT&quot;);//close the connection
  delay(100);
}
