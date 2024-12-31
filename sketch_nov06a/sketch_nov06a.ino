int ENA = 11;
int ENB=10;
int IN1 = 2, IN2 = 3;
int IN3=4,IN4=5;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  
}

void loop() {
  // put your main code here, to run repeatedly     
      analogWrite(ENA, 100);
      analogWrite(ENB, 100);
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH); 
      
}
