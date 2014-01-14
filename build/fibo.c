int main ()
{
  int a, b;
  int c = 0;
  a = 1;
  b = 1;

  while (c < 20)
    {
      c = c + 1;
      printi (a);
      int t = a;
      t = b;
      b = a + b;
      a = t;
      prints (",");
    }
}
}
