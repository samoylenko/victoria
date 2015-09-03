#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include "graphics.h"
#include "misc.h"
#include "memory.h"
#include "judas.h"
#include "timer.h"

#define rscr (char *)0xa0000
#define scrl 0x3e80
#define pi 3.14159265358979324

void main(void);
void maincycle(void);
void maintext(void);

char pal[0x300], mtext[] = "ВИКТОРИЯ";
char *scrbuff1, *scrbuff2, *ptext = &text;
short ax = 50, ay = 50, bx = 90, by = 150, cx = 150, cy = 100;
short colcount = 0, raindata[0x300], i;
signed char txtcol = 3;
signed short a1 = 2, a2 = 3, b1 = -3, b2 = 2, c1 = -3, c2 = 1;
signed short txtcolor = 0;
float angle = 0, an1, an2, an3, vudata[0x20];

void main(void)
{
  scrbuff1 = lockedmalloc(0xfa00);
  scrbuff2 = lockedmalloc(0xfa00);

  if (scrbuff1 == NULL)
  {
    printf("Memory leak.");
    return;
  }
  if (scrbuff2 == NULL)
  {
    printf("Memory leak.");
    lockedfree(scrbuff1);
    return;
  }
  setvmode(0x13);
  clearscreen();
  screen = scrbuff1;
  clearscreen();
  screen = scrbuff2;
  clearscreen();
  srand(0);

  for (i=0; i < 0x40; i++)
  {
    pal[i*3] = 0;
    pal[i*3+1] = 0;
    pal[i*3+2] = i;
  }
  for (; i < 0x80; i++)
  {
    pal[i*3] = 0;
    pal[i*3+1] = i - 0x40;
    pal[i*3+2] = 0x3f;
  }
  for (; i < 0xc0; i++)
  {
    pal[i*3] = 0;
    pal[i*3+1] = 0x3f;
    pal[i*3+2] = 0xbf - i;
  }
  for (; i < 0x100; i++)
  {
    pal[i*3] = i - 0xc0;
    pal[i*3+1] = 0xff - i;
    pal[i*3+2] = 0;
  }
  setpalette(pal);

  for (i=0; i < 256; i++)
  {
    raindata[i*3] = 5 + rand() % 310;
    raindata[i*3+1] = 5 + rand() % 190;
    raindata[i*3+2] = 1 + rand() % 5;
  }

  judas_config();
  judas_init(44100, QUALITYMIXER, SIXTEENBIT | STEREO, 1);
  judas_loads3m("unexpect.s3m");
  judas_plays3m(0);
  timer_init(0x4300, judas_update);

  an1 = (rand() % 11) * pi/10;
  an2 = (rand() % 11) * pi/10;
  an2 = (rand() % 11) * pi/10;

  while (escpressed()) maincycle();

  timer_uninit();
  judas_uninit();

  setvmode(0x03);

  lockedfree(scrbuff1);
  lockedfree(scrbuff2);

  printf(" code: powermike\nmusic: djdus");
}

void maincycle(void)
{
  short stary, aaa;

  screen = scrbuff1;

  for (i=0; i < 256; i++)
  {
    stary = raindata[i*3+1];
    putpixel(raindata[i*3], stary, 0xff);

    stary += raindata[i*3+2]+(stary>>6);
    if (stary > 198) stary = 2;

    raindata[i*3+1] = stary;
  }

  line(320 - ax, 200 - ay, 320 - bx, 200 - by, 255);
  line(320 - ax, 200 - ay, 320 - cx, 200 - cy, 255);
  line(320 - cx, 200 - cy, 320 - bx, 200 - by, 255);

  line(ax, ay, bx, by, 255);
  line(ax, ay, cx, cy, 255);
  line(cx, cy, bx, by, 255);


  line(320 - ax, 200 - ay, ax, ay, 255);
  line(320 - cx, 200 - cy, cx, cy, 255);
  line(320 - bx, 200 - by, bx, by, 255);

  printx = 80; ink = 0x8f;
  for (i=0; i < 10; i++)
  {
    printy = 185 - abs(15 * sin(i * pi / 5 + angle));
    printl(mtext[i]);
  }
  angle += pi/15;

  maintext();

  blur(scrbuff1, scrbuff2);

  screen = scrbuff2;

  for (i = 0; i < 32; i++)
  {
    aaa = 45 * judas_getvumeter(i);

    vertline(10 + i, 195 - aaa, 195, 0x3f + (aaa * 3));
    vertline(310 - i, 195 - aaa, 195, 0x3f + (aaa * 3));

    if (vudata[i] < aaa) vudata[i] = aaa;
  }

  ax += a1; ay += a2;
  bx += b1; by += b2;
  cx += c1; cy += c2;

  if ((ax < 10) || (ax > 310)) a1 = -a1;
  if ((bx < 10) || (bx > 310)) b1 = -b1;
  if ((cx < 10) || (cx > 310)) c1 = -c1;

  if ((ay < 10) || (ay > 190)) a2 = -a2;
  if ((by < 10) || (by > 190)) b2 = -b2;
  if ((cy < 10) || (cy > 190)) c2 = -c2;

  copymem(scrbuff2, scrbuff1, scrl);

  screen = scrbuff2;
  for (i = 0; i < 32; i++)
  {
    putpixel(10 + i, 193 - vudata[i], 0xff);
    putpixel(310 - i, 193 - vudata[i], 0xff);
    if (vudata[i] > 0) vudata[i] -= 0.5;
  }

  copymem(scrbuff2, rscr, scrl);
}

void maintext(void)
{
  short x1, x2, x3, l1, l2, l3;
  char *txt;

  txt = ptext;

  l1 = length(txt);
  l2 = length(txt + l1 + 2);
  l3 = length(txt + l1 + l2 + 4);

  x1 = 160 - (9 * l1);
  x2 = 160 - (9 * l2);
  x3 = 160 - (9 * l3);

  ink = txtcolor;

  printx = x1;
  for (i = 0; i < l1; i++)
  {
    printy = 40 + 10 * sin(i * pi / 4 + an1);
    printl(*txt++);
  }
  txt += 2;

  printx = x2;
  for (i = 0; i < l2; i++)
  {
    printy = 80 + 10 * sin(i * pi / 4 + an2);
    printl(*txt++);
  }
  txt += 2;

  printx = x3;
  for (i = 0; i < l3; i++)
  {
    printy = 120 + 10 * sin(i * pi / 4 + an3);
    printl(*txt++);
  }
  txt += 2;

  if (*txt == 0) txt = &text;

  if (txtcolor > 0x8f) colcount++;
  if (colcount > 0x50)
  {
    colcount = 0;
    txtcol = -txtcol;
  }
  if (!colcount) txtcolor += txtcol;
  if (txtcolor < 1)
  {
    txtcol = -txtcol;
    ptext = txt;
    an1 = (rand() % 11) * pi/10;
    an2 = (rand() % 11) * pi/10;
    an2 = (rand() % 11) * pi/10;
  }
}
