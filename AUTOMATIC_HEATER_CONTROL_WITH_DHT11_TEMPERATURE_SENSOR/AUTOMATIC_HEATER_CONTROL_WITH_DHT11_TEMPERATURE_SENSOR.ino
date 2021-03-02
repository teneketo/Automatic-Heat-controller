#include <dht.h>
#include <LiquidCrystal.h>
#define DHT11_PIN 12 //kyde e DHTto
#define relay_pin 13 // Kyde e releto
#define relayTwo_pin 11 // Wtoro rele ti slagam, za wlajnostta
#define samples 10  //Kolko probi da wzima za temperatura i wlajnost, predi da iz4isli srednata


LiquidCrystal lcd(7,6,5,4,3,2); //Ekrana, towa moje da se mahne ako nqma da pozlwa6 ekran

dht DHT;

int humidity[samples]; //Towa e spisyk s cifri. samples = 10, t.e. tozi spisyk 6te ima 10 zapisa
int temperature[samples];
int sum_temp;
int sum_hum;
int avg_temp;
int avg_hum;


//Tui e nqkakyw izkifili4en simwol za gradus, kojto se polzwa  w ekrana
byte degree[]={B00010,
                  B00101,
                  B00010,
                  B00000,
                  B00000,
                  B00000,
                  B00000,
                  B00000};

void setup(){
  Serial.begin(9600);
  
  lcd.createChar(0,degree);
  lcd.begin(16,2);
  pinMode(relay_pin, OUTPUT);
  pinMode(relayTwo_pin, OUTPUT);
  
}

//Taq funkciq wzima temperaturata i wlajnostta
void get_temp_and_hum(){
  
  DHT.read11(DHT11_PIN);
 //for e kod kojto se powtarq opredelen broi pyti. W slu4aq po-gore kazahme da se powtarq 10 pyti samples = 10 po-gore
  //tozi red se 4ete:
  //zapo4ni ot nulata: i =0
  //prawi sy6tiq kod, zaobraden ot skobite {} dokato i e po-malko ot samples, (t.e. i dokato e po-malko ot 10): i<samples
  //sled wsqko powtorenie dobawqai +1 kym i: i++ (i++ ozna4awa i = i+1)  
  for (int i=0; i<samples; i++){
  //Pri wsqko powtorenie zapiswai w spisyka temperaturata i wlajnostta
  //t.e. pri pyrwoto powtorenie i=0. pyrwiq zapis w spisyka 6te e temperature[0] i 6te zai6e temperaturata kakwato e.
  //sled kato zapi6e i wlajnostta, iz4akwa 1 sekunda predi da naprawi wtorata obikolka ot 10. Togawa 6te zapi6e w temperature[1] i t.n. do temperature[9],
    //sled towa we4e 10 6te e po-golqmo ot 9, t.e. 6te izlezem ot towa powtorenie
  temperature[i]=(DHT.temperature);    
  humidity[i]=(DHT.humidity);
  delay(1000); //AKo iska6 da se obnowqwa wsqka sekunda a ne na wseki 10 sekundi naprawi towa da e 100ms a ne 1000ms (1s)
    //taka kakto e to prawi 10 pyti obikolkata i na wsqka obikolka 4aka po 1000ms predi da naprawi sy6tata obikolka.
    //napraweno e da 4aka, za da moje da se smeni temperaturata, ako 6te se smenq, ako ne 4aka6e 10 pyti 6te6e da e edna i sy6ta temperaturata i nqma6e da ima smisyl wyob6te
    //ot taq slojnotiq
  }

//We4e kato e zapisalo w spisyka 10te temperaturi i wlajnosti, sledwa6tite nqkolko reda iz4islqwat srednata temperatura i wlajnost
  //Pak po sy6tiq na4in, prawim sy6tiq kod 10 pyti, no sega preminawame prez spisyka s temperaturi wlajnosti
  for (int j=0; j<samples; j++){
    //Sabirame wsi4kite temperaturi
    sum_temp += temperature[j];
    }
  //I nakraq gi razdelqme na tehniq broi (na 10)
avg_temp=sum_temp/samples;
//I se polu4awa srednata temperatura - avg. ot avrage = sredna

  //Wajno e da nulirame sbornata promenliwa, ina4e sledwa6tiq pyt kato doidem pak da iz4islqwame, 6te sybirame nowite temperaturi + stariq sbor, i 6te se polu4i byg,
  //kydeto na wseki 10 sekundi temperaturata e dwa pyti po-wisoka
  
sum_temp=0;  
  //(sy6toto ne6to waji za wlajnostta w sledwa6tite redowe)
  for (int j=0; j<samples; j++){
    sum_hum += humidity[j];
    }  
avg_hum=sum_hum/samples;  


sum_hum=0;  
  }

//Funkciq za control na temperaturata. Ako temperaturata e pod 25 gradusa, puska petka4a ( dawa tok na rele-to
void heater_control(){
  if (avg_temp<25)digitalWrite(relay_pin,HIGH);
  else digitalWrite(relay_pin,LOW);//Ako oba4e e NAD 25 gradusa, gasi releto
}

//Nowata funkciq
//Raboti po identi4en na4in kato temperaturata:
//Koda po-gore 10 pyti, 1 pyt w sekunda wzima temperaturata i wlajnostta, sled towa iz4islqwa srednata
void vlajnost_control(){
  if (avg_hum<70)digitalWrite(relayTwo_pin,HIGH); //AKo vjlajnostta e nad 70, swetni sy6toto ne6to kato temperaturata, tuka 6te si sloji6 kakwoto ti trqbwa
  else digitalWrite(relayTwo_pin,LOW);
}


//Tui e we4e glawnata funckiq za ekrana, ne sym polzwal ama mislq 4e ti znae6 kak raboti.
void Display(){

  //Pyrwi iz4islqwame srednite temperatura i wlaga
get_temp_and_hum();

  //sled towa risuwame na ekrana
lcd.setCursor(0,0);
lcd.print("Temperature=");
lcd.print(avg_temp);
lcd.write(byte(0));
lcd.print("C");

lcd.setCursor(0,1);
lcd.print("Humidity = ");
lcd.print(avg_hum);   
lcd.print("%"); 
  
  //Nakraq sled kato sme izpisali na ekrana, puskame ili gasim reletata
heater_control();  
vlajnost_control();
  }

void loop(){
Display();
}
