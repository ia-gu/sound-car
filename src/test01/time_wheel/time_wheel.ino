#define PIN_IN1  12
#define PIN_IN2  13
#define PIN_VREF 3 // PWM
 
void setup(){
  pinMode(PIN_IN1,OUTPUT); 
  pinMode(PIN_IN2,OUTPUT); 
}
 
void loop(){
      
    // モーターの回転速度を中間にする
    analogWrite(PIN_VREF,127);
 
    // 回転
    digitalWrite(PIN_IN1,HIGH);
    digitalWrite(PIN_IN2,LOW);
    delay(5000);
 
    // ブレーキ
    digitalWrite(PIN_IN1,HIGH);
    digitalWrite(PIN_IN2,HIGH);
    
    // 逆回転
    digitalWrite(PIN_IN1,LOW);
    digitalWrite(PIN_IN2,HIGH);    
    delay(5000);
 
    // ブレーキ
    digitalWrite(PIN_IN1,HIGH);
    digitalWrite(PIN_IN2,HIGH);
    delay(2000);
    
    // モーターの回転速度を最大にする
    analogWrite(PIN_VREF,255); 
    
    // 逆回転　
    digitalWrite(PIN_IN1,LOW);
    digitalWrite(PIN_IN2,HIGH);    
    delay(5000);
                
    // ストップ
    digitalWrite(PIN_IN1,LOW);
    digitalWrite(PIN_IN2,LOW);
    delay(2000);
}
