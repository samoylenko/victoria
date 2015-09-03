#define abs(x) (x < 0 ? -x : x)

extern short printx;
extern short printy;
extern char ink;
extern char text[];

extern char escpressed(void);
extern void blur(void *, void *);
extern char length(char *);

void prints(char *);
void printat(short, short, char *);

void prints(char *text)
{
  char a;

  do
  {
    a = *text++;
    printl(a);
  } while (a);
}

void printat(short x, short y, char *text)
{
  printx = x; printy = y;
  prints(text);
}
