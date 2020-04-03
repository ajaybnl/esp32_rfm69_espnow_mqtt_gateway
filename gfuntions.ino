
//Blink Led (Times)
void blink(byte PIN, int times)
{
  for (int i = 1; i <= times; i++) {
    digitalWrite(PIN, HIGH);
    delay(50);
    digitalWrite(PIN, LOW);
    delay(50);
  }
}
