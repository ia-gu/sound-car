const int VOL_PIN = A0;
#define PIN_IN1  2
#define PIN_IN2  4
#define PIN_VREF 3 // PWM
int keep;
void setup(){
    Serial.begin( 9600 );
    pinMode(PIN_IN1,OUTPUT);
    pinMode(PIN_IN2,OUTPUT);
}

void loop(){
    int value;
    float volt;
    value = analogRead( VOL_PIN );
    if(value==0){
      value=keep;
    }
    volt = value * 5.0 / 1023.0;
    Serial.print( "Value: " );
    Serial.print( value );
    Serial.print( "  Volt: " );
    Serial.println( volt );
    if(700<value && value<770){
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, LOW);
      delay(100);
    }
    else if(value>300){
      analogWrite(PIN_VREF,255);
      digitalWrite(PIN_IN1,HIGH);
      digitalWrite(PIN_IN2,LOW);
      delay( 50 );
    }
    else if(value>200){
      analogWrite(PIN_VREF,127);
      digitalWrite(PIN_IN1,HIGH);
      digitalWrite(PIN_IN2,LOW);
      delay( 50 );
    }
    else if(value>100){
      analogWrite(PIN_VREF,127);
      digitalWrite(PIN_IN1,LOW);
      digitalWrite(PIN_IN2,HIGH);
      delay( 50 );
    }
    else {
      digitalWrite(PIN_IN1,LOW);
      digitalWrite(PIN_IN2,LOW);
      delay(50);
    }
    keep=value;
}
