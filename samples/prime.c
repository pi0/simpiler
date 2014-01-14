void main ()
{
  int a;
  int i;
  int a = 2;

  while (a < 50)
    {

      i = 2;
      int is_prime = 1;
      while (i < a)
	{
	  int mod = a % i;
	  if (mod == 0)
	    is_prime = 0;
	  i = i + 1;
	}
      if (is_prime == 1)
	{
	  printi (a);
	  prints ("\n");
	}

      a = a + 1;
    }
}
