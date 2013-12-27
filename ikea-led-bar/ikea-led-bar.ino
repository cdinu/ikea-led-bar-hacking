#define R 9
#define G 10
#define B 11

#define READING A0
#define N 100
#define INTERVAL 1000

int currentReading;
int readings[N];

int cursor = 0;

void updateReading() {
  unsigned long avgs = 0;
  int i = 0;

  while (i < N) {
    avgs += analogRead(READING);
    i++;
  }

  avgs = 1.0 * avgs / i;
  currentReading = (int) avgs;


  
  Serial.print("Tick. The readings are: ");
  int k;
  for (k = 0; k <N; k++) {
    Serial.print(readings[k]);
    Serial.print(" ");
  }
  
  Serial.println("");
  
  readings[cursor] = currentReading;
  cursor++;
  if (cursor >= N) {
    cursor = 0;
  }
}

int getTrend() {
  unsigned long average = 0;
  int i = 0;
  int count = 0;

  while (i < N) {
    int v = readings[i];
    if (v != 0) {
      average += v;
      count ++;
    }
    i++;
  }
  
  if (count == 0) return 0;
  average = average / count;


  Serial.print("Read: ");
  Serial.print(currentReading);

  Serial.print(" average: ");
  Serial.println(average);
 
  int delta = 100 * (currentReading - average) / average;
   
  if (delta < 0) return -1;
  if (delta > 0) return  1;
  return 0;

}

void setColor(int red, int green, int blue) {
  analogWrite(R, 255 - red);
  analogWrite(G, (255 - green) / 1.05);
  analogWrite(B, (255 - blue) / 1.2);
}

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  updateReading();

  switch (getTrend())  {
    case -1:
      setColor(0, 0, 255);
      break;
    case 1:
      setColor(255, 0, 0);
      break;
    default:
      setColor(255, 0, 255);
      break;
  }

  delay(INTERVAL);
}
