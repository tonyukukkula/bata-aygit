// Yazar: Alperen Kula
// Yazım Tarihi 23.05.2019
#include <LiquidCrystal.h> 
#include <Adafruit_Fingerprint.h>
int ldrPin = A0; //ışık duyarlılığını kullanacağımız için analog pinden bağladık
int Contrast=100;
int ldrValue =0;
uint8_t id;
int i;
void(*resetFunc)(void)=0; //o zaman bir sıkıntı çıkmıştı sistemi resetlememiz gerekiyordu ondan yazmıştık ama ne sıkıntısı çıktığını hatırlamıyorum.
SoftwareSerial mySerial(10, 11); //projenin ana uygulaması (windows form app) ile haberleştirmek için
LiquidCrystal lcd(9, 8, 5, 4, 3, 2); //lcd için 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup()  
{
  analogWrite(6,Contrast);
  lcd.begin(16, 2);
  pinMode(A13, OUTPUT);//parmak izi için yeşil Led
  pinMode(A15, OUTPUT);//parmak izi için kırmızı led
  pinMode(A8, OUTPUT);//Oda_1 beyaz ledi
  pinMode(A5, OUTPUT);//Oda_2 beyaz 2.ledi
  pinMode(A1, OUTPUT);//Oda_2 beyaz 3.ledi
  lcd.setCursor(0,0);lcd.print("Merhabalar!");

  Serial.begin(9600);
  while (!Serial);
  delay(100);
  finger.begin(57600);
  if (finger.verifyPassword()) { //sensörün çalışıp çalışmadığını başta kontrol etmek için
    Serial.println("Sensor bulundu!");
    analogWrite(A8, 1023);
  } else {
    Serial.println("Sensor bulunamadı:(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
}
void loop()
{
    char b=(char)Serial.read(); //uygulamadan gelecek komutlar için arduino dinlemeye geçiyor
    if(b=='t'){lcd.clear();lcd.setCursor(0,0);lcd.print("parmak tarat");getFingerprintIDez();while(getFingerprintIDez()==(-1)){getFingerprintIDez();}}
    else if(b=='k'){lcd.clear();lcd.setCursor(0,0);lcd.print("parmak kaydet");enrolling();}
    else if(b=='s'){lcd.clear();lcd.setCursor(0,0);lcd.print("parmak sil");delet();}
    //Oda_1();
    //Oda_2();
    //Oda_3();
    if(b=='t'){resetFunc();} //sanırım hata parmak taratma fonksiyonundan kaynaklanıyordu buradan anladığım üzere
    if(b=='r'){resetFunc();} //alette sorun çıkarsa önlem olarak koyduk
}
int getFingerprintIDez() {//parmak taratma fonksiyonu
  //Serial.println("Waiting for valid finger...");
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {return -1;}

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){return -1;}

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){return -1;}
  Serial.print(finger.fingerID);Serial.println(";");
  //Serial.print(" ;Şu doğruluk oranıyla: "); Serial.println(finger.confidence);
  if(finger.fingerID==10){lcd.clear();lcd.setCursor(0,0);lcd.print("Alperen Baba");lcd.setCursor(0,1);lcd.print("Giris Yapti");} //id sayım 10 olduğu için espri olsun diye kendime özel yazı yazdırmıştım :)
  if(finger.confidence>=90){OnayBildirim();}
  else{RedBildirim();}
  return finger.fingerID; 
}
uint8_t getFingerprintEnroll() {//parmak kaydetme fonksiyonu

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      analogWrite(A5,800);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      break;
    default:
      Serial.println("Unknown error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
    default:
      Serial.println("Unknown error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
  }
  
  Serial.println("Remove finger");
  analogWrite(A5,0);
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");analogWrite(A5,800);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      break;
    default:
      Serial.println("Unknown error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
    default:
      Serial.println("Unknown error");
      for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else {
    Serial.println("Unknown error");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");analogWrite(A5,0);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else {
    Serial.println("Unknown error");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  }   
}
uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
uint8_t deleteFingerprint(uint8_t id) {//parmak silme fonksiyonu
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");analogWrite(A15, 600);delay(1800);analogWrite(A15,0);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}
void enrolling() //parmak kaydetme fonksiyonunun kullandıldığı kısım
{
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
    
}
void delet() //parmak silme fonksiyonunun kullandıldığı kısım
{
    Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  uint8_t id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }

  Serial.print("Deleting ID #");
  Serial.println(id);
  
  deleteFingerprint(id);
}

void OnayBildirim() //eğer eylem başarılı ise başarılı olduğunu göstermek için
{
  int i=0;
  for(i=0;i<10;i++){
    analogWrite(A13, 255);
    delay(100);
    analogWrite(A13, 0);
    delay(100);
  }
  
}
void RedBildirim() //eğer eylem başarısız ise başarısız olduğunu göstermek için
{
  int i=0;
  analogWrite(A13, 255);
  for(i=0;i<20;i++){analogWrite(A15, 255);delay(100);analogWrite(A15, 0);delay(100);}
  analogWrite(A13, 0);
}

void LDR() //güvenlik sisteminin kapsadığı odaların içinde lambalanın yanıp yanmadığını algılaması için
{
   ldrValue = analogRead(ldrPin);
   Serial.print(ldrValue);Serial.println("I");
}
