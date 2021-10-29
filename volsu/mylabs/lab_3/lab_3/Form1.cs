using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab_3
{
    public partial class Form1 : Form
    {
        private Bitmap bitmap;
        private List<List<int>> dots;
        private Color color;
        private Color clearColor;

        private List<int> down_loc = new List<int>();
        private List<int> up_loc = new List<int>();

        const int INSIDE = 0; // 0000
        const int LEFT = 1;   // 0001
        const int RIGHT = 2;  // 0010
        const int BOTTOM = 4; // 0100
        const int TOP = 8;    // 1000

        int xmin;
        int xmax;
        int ymin;
        int ymax;

        public Form1()
        {
            InitializeComponent();
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            dots = new List<List<int>>();
            color = Color.FromArgb(246, 45, 71);
            Console.WriteLine("Start");
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Console.WriteLine($"DOWN {e.X} {e.Y}");
                List<int> coords = new List<int>();
                coords.Add(e.X);
                coords.Add(e.Y);

                color = Color.FromArgb(0, 0, 0);
                bitmap.SetPixel(e.X, e.Y, color);
                dots.Add(coords);
                pictureBox1.Image = bitmap;

                ConnectDots();
            }
            else if (e.Button == MouseButtons.Right)
            {
                Console.WriteLine("Down Rectangle");
                down_loc.Add(e.X);
                down_loc.Add(e.Y);
            }
        }

        private void ConnectDots()
        {
            Console.WriteLine("Draw line");
            int amount = dots.Count;
            if (amount < 2) return;

            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            color = Color.FromArgb(246, 45, 71);

            for (int i = 0; i < amount - 1; i++)
            {
                int start_x = dots[i][0];
                int start_y = dots[i][1];
                int end_x = dots[(i + 1)][0];
                int end_y = dots[(i + 1)][1];

                drawLine(start_x, start_y, end_x, end_y);
            }

            pictureBox1.Image = bitmap;
        }

        // реализация алгоритма Брезенхейма
        private void drawLine(int xa, int ya, int xb, int yb)
        {

            int dy = Math.Abs(yb - ya);
            int dx = Math.Abs(xb - xa);
            int d;
            int sx = xb >= xa ? 1 : -1; // направление приращения x
            int sy = yb >= ya ? 1 : -1; // направление приращения y

            if (dy <= dx)
            {
                d = (2 * dy) - dx;
                int d1 = dy * 2;
                int d2 = (dy - dx) * 2;
                int x = xa + sx;
                int y = ya;


                for (int i = 1; i <= dx; i++, x += sx)
                {
                    if (d > 0)
                    {
                        d += d2;
                        y += sy;
                    }
                    else
                        d += d1;

                    SetPixel(x, y);
                }
            }
            else
            {
                d = (2 * dx) - dy;
                int d1 = 2 * dx;
                int d2 = (dx - dy) * 2;
                int x = xa;
                int y = ya + sy;
                SetPixel(x, y);

                for (int i = 1; i <= dy; i++, y += sy)
                {
                    if (d > 0)
                    {
                        d += d2;
                        x += sx;
                    }
                    else
                        d += d1;
                    SetPixel(x, y);
                }
            }
        }

        private void SetPixel(int x, int y)
        {
            bitmap.SetPixel(x, y, color);
        }
        private void ClearLoc()
        {
            down_loc.Clear();
            up_loc.Clear();
        }

        private void ClearScreen(int s_x, int s_y, int e_x, int e_y)
        {

        }

        // попиксельная отрисовка прямоугольника
        private void DrawRectangle(int s_x, int s_y, int e_x, int e_y)
        {
            color = Color.FromArgb(255, 0, 255);
            int width = Math.Abs(s_x - e_x);
            int height = Math.Abs(s_y - e_y);

            //_height = height;
            //_width = width;

            Console.WriteLine($"x1={s_x}, y1={s_y}, x2={e_x}, y2={e_y}");

            int x = s_x <= e_x ? s_x : e_x;
            int y = s_y <= e_y ? s_y : e_y;


            int d_x = s_x - e_x >= 0 ? -1 : 1;
            int d_y = s_y - e_y >= 0 ? -1 : 1;

            for (int i = x; i < x + width; i += 1)
            {
                SetPixel(i, s_y);
                SetPixel(i, s_y + d_y * height);
            }

            for (int i = y; i < y + height; i += 1)
            {
                SetPixel(s_x, i);
                SetPixel(s_x + d_x * width, i);
            }

            pictureBox1.Image = bitmap;
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Console.WriteLine($"UP {e.X} {e.Y}");
                up_loc.Add(e.X);
                up_loc.Add(e.Y);



                xmin = Math.Min(down_loc[0], up_loc[0]);
                xmax = Math.Max(down_loc[0], up_loc[0]);
                ymin = Math.Min(down_loc[1], up_loc[1]);
                ymax = Math.Max(down_loc[1], up_loc[1]);
                Console.WriteLine();
                DrawRectangle(down_loc[0], down_loc[1], up_loc[0], up_loc[1]);

                color = Color.FromArgb(0, 0, 255);

                for (int i = 0; i < dots.Count - 1; i++)
                {
                    int start_x = dots[i][0];
                    int start_y = dots[i][1];
                    int end_x = dots[(i + 1)][0];
                    int end_y = dots[(i + 1)][1];

                    CohenSutherlandLineClipAndDraw(start_x, start_y, end_x, end_y);
                    Console.WriteLine(i);
                }
                pictureBox1.Image = bitmap;
                Console.WriteLine("End");
                ClearLoc();
            }
        }

        int ComputeOutCode(int x, int y)
        {
            int code;
            code = INSIDE;          // полагаем что точка лежит внутри

            if (x < xmin)
                code |= LEFT;
            else if (x > xmax)
                code |= RIGHT;
            if (y < ymin)
                code |= BOTTOM;
            else if (y > ymax)
                code |= TOP;

            return code;
        }

        void CohenSutherlandLineClipAndDraw(int x0, int y0, int x1, int y1)
        {
            // узнаем где лежат точки
            int outcode0 = ComputeOutCode(x0, y0);
            int outcode1 = ComputeOutCode(x1, y1);
            Console.WriteLine($"outcode0={outcode0}, outcode1={outcode1}");

            // пока 2 точки не окажутся внутри прямоугольника
            while ((outcode0 | outcode1) != 0)
            {
                // обе точки за пределами прямоугольника, выходим их функции
                if ((outcode0 & outcode1) != 0)
                {
                    return;
                }
                else
                {
                    Console.WriteLine("CALC new points");
                    int x = x0;
                    int y = y0;

                    // берем ту точку которая лежит внутри прямоугольника
                    int outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

                    // находим точки пересечения по следующим формулам
                    //   k = (y1 - y0) / (x1 - x0)
                    //   x = x0 + (1 / k) * (ym - y0),
                    //   y = y0 + k * (xm - x0)
                    if ((outcodeOut & LEFT) != 0)
                    {
                        y += (y0 - y1) * (down_loc[0] - x) / (x0 - x1);
                        x = down_loc[0];
                    }
                    else if ((outcodeOut & RIGHT) != 0)
                    {
                        y += (y0 - y1) * ((up_loc[0]) - x) / (x0 - x1);
                        x = up_loc[0];
                    }
                    else if ((outcodeOut & BOTTOM) != 0)
                    {
                        x += (x0 - x1) * (down_loc[1] - y) / (y0 - y1);
                        y = down_loc[1];
                    }
                    else if ((outcodeOut & TOP) != 0)
                    {
                        x += (x0 - x1) * ((up_loc[1]) - y) / (y0 - y1);
                        y = up_loc[1];
                    }

                    // заменяем точку снаружи на точку пересечения
                    if (outcodeOut == outcode0)
                    {
                        x0 = x;
                        y0 = y;
                        outcode0 = ComputeOutCode(x0, y0);
                    }
                    else
                    {
                        x1 = x;
                        y1 = y;
                        outcode1 = ComputeOutCode(x1, y1);
                    }
                }
            }
            Console.WriteLine($"x0={x0}, y0={y0}, x1={x1}, y1={y1} ");
            // рисуем линию с новыми координатами
            drawLine(x0, y0, x1, y1);

        }


    }
}
