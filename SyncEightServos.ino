void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

int sN;
int angleVal;
int holderVec[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int count;

void loop() {
  // put your main code here, to run repeatedly:
   char input[] = "2,90,3,40,4,50,1,20,6,30,5,80,7,100,8,100";
   char separator[] = ",";
   char *token;
   
   
   /* get the first token */
   token = strtok(input, separator);
   
  // Find any more?
    count = 0;
   while(token != NULL) 
   {
       
      token = strtok(NULL, separator);
//      Serial.println(token);
      holderVec[count] = *token;
      
      
      count = count + 1;
      
   }
       
      //desiredpositions[servoNum] = AngleFind(angleVal);

 for(int b = 0; b < 15; b++){
  Serial.println(holderVec[b]);
 }
       
      //desiredpositions[servoNum] = AngleFind(angleVal);
}
