/*
 * Ben Payne copyright 2015
 * 
 * Code to read servos position from a RC reciever and control a two channel 
 *  H-Bridge for the motors of a robot.  This will also interface to a Raspberry Pi
 *  for controls.
 *
 * Used sparkfun ardomoto reference code as an example for the start of some of this code.
 */ 

int pwm_a = 3;
int pwm_b = 11;
int dir_a = 12;
int dir_b = 13;
int val = 0;
 
int pos = 0;
 
const int ch1_pin = 6;
const int ch2_pin = 7;

int ch1;
int ch2;

void setup()
{
  Serial.begin(9600);
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  
  pinMode(ch1_pin,INPUT);
  pinMode(ch2_pin,INPUT);
  
  analogWrite(pwm_a, 0);  //set both motors to run at (100/255 = 39)% duty cycle (slow)
  analogWrite(pwm_b, 0);
}

void setLeftMotor( char motor_dir, int motor_speed )
{
  if ( motor_dir > 0 )
    digitalWrite(dir_a, HIGH);
  else
    digitalWrite(dir_a, LOW);

  analogWrite(pwm_a, motor_speed);
}

void setRightMotor( char motor_dir, int motor_speed )
{
  if ( motor_dir > 0 )
    digitalWrite(dir_b, HIGH);
  else
    digitalWrite(dir_b, LOW);

  analogWrite(pwm_b, motor_speed);
}

void loop()
{
  int vert_val;
  int hor_val;
  long left_val;
  long right_val;
  int mm_val;
  
  // Get the pulses for the channels.
  // TODO: Add 3rd channel for positive engagement 
  // TODO: Add 4th channel to allow web control
  ch1 = pulseIn(ch1_pin,HIGH);
  ch2 = pulseIn(ch2_pin,HIGH);
  
  Serial.print( "Ch1: " );
  Serial.println( ch1 );
  Serial.print( "Ch2: " );
  Serial.println( ch2 );
  
  // Check if values are within range, otherwise full stop.
  if ( ch1 < 1000 || ch2 < 1000 || ch1 > 2000 || ch2 > 2000 )
  {
    vert_val = 0;
    hor_val = 0;
  }
  else 
  {
    // Turn channel timing data into a value between -255 and +255 for both channels
    //  Vert is forward and back.
    if ( ch1 >= 1500 )
    {
      vert_val=-map( ch1, 1500, 1900, 0, 255 );
      Serial.print( "Out forward ch1: " );
      Serial.println( val );
      if ( vert_val > 255 )
        vert_val = 255;
    }
    else
    {
      vert_val=map( ch1, 1500, 1100, 0, 255 );
      Serial.print( "Out backward ch1: " );
      Serial.println( vert_val );
      if ( vert_val < -255 )
        vert_val = -255;
    }

    // Hor is left/right
    if ( ch2 >= 1500 )
    {
      hor_val=-map( ch2, 1500, 2000, 0, 255 );
      Serial.print( "Out forward ch2: " );
      Serial.println( hor_val );
      if ( hor_val > 255 )
        hor_val = 255;
    }
    else
    {
      hor_val=map( ch2, 1500, 1000, 0, 255 );
      Serial.print( "Out backward ch2: " );
      Serial.println( hor_val );
      if ( hor_val < -255 )
        hor_val = -255;
    }
  }
  
  // Now mix the values together to get the speeds for the left and right motor.
  if ( vert_val > 0 )
  {
    if ( hor_val < 0 )
    {
      right_val = vert_val - hor_val;
      left_val = vert_val;
    }
    else
    {
      right_val = vert_val;
      left_val = vert_val + hor_val;
    }
  }
  else
  {
    if ( hor_val < 0 )
    {
      right_val = vert_val + hor_val;
      left_val = vert_val;
    }
    else
    {
      right_val = vert_val;
      left_val = vert_val - hor_val;
    }
  }
  
  Serial.print( "Left: " );
  Serial.print( left_val );
  Serial.print( " Right: " );
  Serial.println( right_val );
  
  // Now since the values may be larger that 255, we'll scale them back to within range.
  if ( max( right_val, left_val ) > 255 )
  {  
    mm_val = max( right_val, left_val );
    left_val = left_val * 255 / mm_val;
    right_val = right_val * 255 / mm_val;
  }
  else if ( min( right_val, left_val ) < -255 )
  {  
    mm_val =  min( right_val, left_val );
    left_val = left_val * -255 / mm_val;
    right_val = right_val * -255 / mm_val;
  }
  
  Serial.print( "Scaled Left: " );
  Serial.print( left_val );
  Serial.print( " Scaled Right: " );
  Serial.println( right_val );

  // Now tell the motors what to do.
  if ( left_val < 0 )
    setLeftMotor( 0, -left_val );
  else
    setLeftMotor( 1, left_val );

  if ( right_val < 0 )
    setRightMotor( 0, -right_val );
  else
    setRightMotor( 1, right_val );
  
  // TODO: Detect dead stike position and full stop.
  //delay(1000);
}

void stopped()
{ 
  // TODO add control for the motor brakes.  
  digitalWrite(dir_a, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_b, LOW); //Set motor direction, 3 high, 4 low
  analogWrite(pwm_a, 0);    //set both motors to run at 100% duty cycle (fast)
  analogWrite(pwm_b, 0); 
}




