extern char* screen;

extern void setvmode(short);
extern void waitretrace(void);
extern void putpixel(short, short, char);
extern void horline(short, short, short, char);
extern void vertline(short, short, short, char);
extern void setpalette(void *);
extern void clearscreen(void);
extern void printl(char);

void line(float, float, float, float, char);
void triangle(float, float, float, float, float, float, char);

void line(float ax, float ay, float bx, float by, char color)
{
  float dx;

  if (ax == bx)
  {
    vertline(ax, ay, by, color);
    return;
  }
  if (ay == by)
  {
    horline(ax, bx, ay, color);
    return;
  }

  if (ay > by)
  {
    dx = ax; ax = bx; bx = dx;
    dx = ay; ay = by; by = dx;
  }

  dx = (bx - ax) / (by - ay + 1);

  for (; ay <= by; ay ++)
  {
    horline(ax, ax + dx, ay, color);
    ax += dx;
  }
}

void triangle(float ax, float ay, float bx, float by, float cx, float cy, char color)
{
  float dx1, dx2, x;

  if (ay > by)
  {
    dx1 = ax; ax = bx; bx = dx1;
    dx1 = ay; ay = by; by = dx1;
  }
  if (by > cy)
  {
    dx1 = bx; bx = cx; cx = dx1;
    dx1 = by; by = cy; cy = dx1;
  }
  if (ay > by)
  {
    dx1 = ax; ax = bx; bx = dx1;
    dx1 = ay; ay = by; by = dx1;
  }

  if (ay == cy)
  {
    horline(ax, bx, ay, color);
    horline(ax, cx, ay, color);
    return;
  }
  if ((ax == bx) && (ax == cx))
  {
    vertline(ax, ay, cy, color);
    return;
  }

  x = ax;
  dx1 = (cx - ax) / (cy - ay + 1);
  dx2 = (bx - ax) / (by - ay + 1);

  for (; ay <= by; ay++)
  {
    horline(ax, x + dx2, ay, color);
    ax += dx1; x += dx2;
  }

  dx2 = (cx - bx) / (cy - by + 1);

  for (; ay <= cy; ay++)
  {
    horline(ax, x + dx2, ay, color);
    ax += dx1; x += dx2;
  }
}
