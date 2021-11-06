
#define STROBE 2

// D0-D7
//int data[8] = { 3,4,5,6,7,8,9,10 };
int data[8] = { 10,9,8,7,6,5,4,3 };
// Keycodes from copper layer
byte table[12][8] = { { 0, 0, 37, 54, 87, 36, 53, 65 }, 
                      { 0, 35, 52, 68, 86, 0, 18, 0 },  
                      { 2, 33, 49, 50, 85, 16, 71, 0 }, 
                      { 17, 34, 51, 67, 70, 19, 1, 66 },  
                      { 7, 29, 45, 61, 81, 12, 77, 55 }, 
                      { 6, 23, 38, 46, 76, 30, 82, 62 },  
                      { 4, 21, 31, 0, 74, 14, 84, 0 }, 
                      { 13, 0, 22, 47, 83, 5, 75, 63 },  
                      { 25, 41, 42, 57, 79, 26, 9, 58 }, 
                      { 11, 24, 44, 39, 78, 28, 8, 60 },  
                      { 27, 40, 59, 56, 80, 43, 10, 69 }, 
                      { 15, 32, 20, 48, 72, 3, 73, 64 }, };  
                      
char *names[88] = { 
"INVALID",
"` |","\\ ~","! 1","@ 2","# 3","$ 4","% 5","^ 6","& 7","* 8","( 9",") 0","- _","+ =","<X]","n7","n8","n9",
"ESC","TAB","Q","W","E","R","T","Y","U","I","O","P","{ [","} ]","n4","n5","n6",
"CMND","CONTROL","A","S","D","F","G","H","J","K","L",": ;","\" '","RETURN","n1","n2","n3",
"ALL CAPS","(L)SHIFT","Z","X","C","V","B","N","M","< ,","> .","? /","(R)SHIFT","n0","n.","ENTER",
"SPACE",
"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","n-","n,","CURSR LOCK"
};
void writeAddress(int addr)
{
  // STROBE GOES LOW
  digitalWrite(STROBE, 0);
  delay(1);
  
  // ASSERT ADDRESS
  for(int i=0;i<8;i++) {
    digitalWrite(data[i], (addr>>i)&1); 
  }
  for(int i=0;i<8;i++) {
    pinMode(data[i], OUTPUT);
  }
  delay(1);
  
  // STROBE GOES HIGH
  digitalWrite(STROBE, 1);
  //delay(10);

  // DEASSERT ADDRESS
  for(int i=0;i<8;i++) {
    pinMode(data[i], INPUT);
  }
}

int readData()
{
  // READ BUS
  int rv = 0;
  for(int i=7;i>=0;i--) {
    rv <<= 1;
    rv = rv | digitalRead(data[i]);
  }
  return rv;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(STROBE, OUTPUT);
  digitalWrite(STROBE, 1);
  for(int i=0;i<8;i++) {
    pinMode(data[i], INPUT);
    digitalWrite(data[i], 0);
  }
  Serial.begin(115200);
}

void blinky(){
  writeAddress(0x20); //cursor on
  delay(200);
  writeAddress(0x10); //cursor off
  delay(200);
  writeAddress(0x80); //caps on
  delay(200);
  writeAddress(0x40); //caps off
  delay(200);
}

void loop() {

  //blinky();
  int rv = 0;
  int d;
  for(int i=0;i<12;i++)
  {
    writeAddress(i);
    delay(2);
    d = readData();
    rv |= printChar(i,d);
  }
  if (rv) Serial.println();
}

int printChar(int addr, int data){
  bool rv = 0;
  for(int i=0; i<8; i++)
  {
    int bitmask = 1<<i;
    if (!(data & bitmask))
    {
      rv = 1;
      Serial.println();
      Serial.print(table[addr][i]);
      Serial.print(", ");
      Serial.print(names[table[addr][i]]);
    }
  }
  return rv;
}
