#define ENCA 2
#define BLUE 3
#define ENCB 4
#define GREEN 5
#define RED 6

class Vector3Int
{
  public: int X = 0;
  public: int Y = 0;
  public: int Z = 0;

  Vector3Int(int Xin, int Yin, int Zin)
  {
    X = Xin;
    Y = Yin;
    Z = Zin;
  }
};

void setup() {
  // put your setup code here, to run once:
pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
pinMode(ENCA, INPUT_PULLUP);
pinMode(ENCB, INPUT_PULLUP);


digitalWrite(RED, HIGH);
digitalWrite(GREEN, LOW);
digitalWrite(BLUE, LOW);
//Serial.begin(9600);
}

int _multiplier = 16;
int _encDelayTotalTimeExtend = 500000000;
int _encDelayTotalTimeNormal = 1000;
int _encEndTime = _encDelayTotalTimeNormal;
int _encDelayTimer = 0;
int _ongoingValue = 0;
bool _buttonReleased = true;
void loop() 
{
  // put your main code here, to run repeatedly:


  // increment timer
  _encDelayTimer++;
  // check if timer is done
  if (_encDelayTimer >= _encEndTime)
  {
    // reset timer
    _encDelayTimer = 0;
    // get encoder value
    int encOut = GetOutputFromEncoder();

    // no output
    if (encOut == 0)
    {
      // timer will check again after normal time
      _encEndTime = _encDelayTotalTimeNormal;
      // button is in reset position
      _buttonReleased = true;
    }
    // if the encoder is reading a value and has been reset since last read
    else if (_buttonReleased == true)
    {
      // timer will be extended to debounce
      _encEndTime = _encDelayTotalTimeExtend;
      // button needs to be reset now
      _buttonReleased = false;

      _ongoingValue += encOut * _multiplier;
      //Serial.println(abs(_testVolume) % 1023);
      Vector3Int ledValues = GetLedVector(abs(_ongoingValue) % 1023);
      analogWrite(RED, ledValues.X);
      analogWrite(GREEN, ledValues.Y);
      analogWrite(BLUE, ledValues.Z);
    }
  }
}

int GetOutputFromEncoder()
{
  int encAval = digitalRead(ENCA);
  int encBval = digitalRead(ENCB);

  if (encAval == 1 && encBval == 0) return 1;
  else if (encAval == 0 && encBval == 1) return -1;
  else return 0;
}

Vector3Int GetLedVector(int input)
{
  input = constrain(input, 0, 1023);

  return Vector3Int(
    (int)GetRedLEDValue(input),
    GetGreenLEDValue(input),
    GetBlueLEDValue(input)
  );
}

float GetRedLEDValue(int input)
{
  if (input < 342)
  {
    // y = (-255/341)x + 255
    return -255*(float)input/341 +255;
  }
  else if (input < 683)
  {
    return 0;
  }
  else if (input < 1024)
  {
    // y = (255*x)/341 - 510
    return 255*(float)input/341 - 510;
  }
  else return 0;
}

int GetGreenLEDValue(int input)
{
  if (input < 342)
  {
    // y= 255x/341
    return (255 * (float)input)/341;
  }
  else if (input < 683)
  {
    // y = (-255 * x)/ 341 + 510
    return ((-255 * (float)input)/ 341) + 510;
  }
  else return 0;
}

int GetBlueLEDValue(int input)
{
  if (input < 341)
  {
    return 0;
  }
  else if ((float)input < 683)
  {
    // y = 255x/341 -255
    return ((255*(float)input)/341) - 255;
  }
  else if ((float)input < 1024)
  {
    // y = (-255 * x)/341 + 765
    return ((-255 * (float)input)/341) + 765;
  }
  else return 0;
}

