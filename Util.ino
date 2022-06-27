/*
// ***************************************** writeString() ************************************
void writeString(String msg)
{
  if (!midiEnabled)
  {
    Serial.println(msg);
  }
}
// ***************************************** writeInt() ************************************
void writeInt(int msg)
{
  if (!midiEnabled)
  {
    Serial.println(msg);
  }
}
bool createBuffer(int size) 
{
  if ( (byteArray = (byte*) malloc (size * sizeof(byte))) == NULL ) 
  {
    free(byteArray);
    return false;
  }
  return true;
}
*/
// ***************************************** outputGmCount() ************************************
void outputGmCount(byte count)
{
  //Serial.print("outputGmCount(). Count=");
  //Serial.println(count, HEX);
  
  if (count & 0x01)
    digitalWrite(D0_pin, HIGH);
  else
    digitalWrite(D0_pin, LOW);
    
  if (count & 0x02)
    digitalWrite(D1_pin, HIGH);
  else
    digitalWrite(D1_pin, LOW);
    
  if (count & 0x04)
    digitalWrite(D2_pin, HIGH);
  else
    digitalWrite(D2_pin, LOW);
    
  if (count & 0x08)
    digitalWrite(D3_pin, HIGH);
  else
    digitalWrite(D3_pin, LOW);
    
  if (count & 0x10)
    digitalWrite(D4_pin, HIGH);
  else
    digitalWrite(D4_pin, LOW);
    
  if (count & 0x20)
    digitalWrite(D5_pin, HIGH);
  else
    digitalWrite(D5_pin, LOW);
}

