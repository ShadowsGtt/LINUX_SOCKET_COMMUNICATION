int DJBHash(string str)
{
   int hash = 5381;
   for(int i = 0; i < str.size(); i++)
  {
       hash = ((hash << 5) + hash) + str[i]; //hash = hash * 32 + c
  }
   return (hash & 0x7FFFFFFF);
}
