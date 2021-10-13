using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab_2
{
    public partial class Form1 : Form
    {


        private Bitmap bitmap;

        // reactangle
        int _height;
        int _width;
        int start_x;
        int start_y;

        private List<int> down_loc = new List<int>();
        private List<int> up_loc = new List<int>();
        private Color color;

        public Form1()
        {
            InitializeComponent();
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            color = Color.FromArgb(246, 45, 71);
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            Console.WriteLine("Down");

            down_loc.Add(e.X);
            down_loc.Add(e.Y);
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            Console.WriteLine("UP");
            up_loc.Add(e.X);
            up_loc.Add(e.Y);

            DrawRectangle(down_loc[0], down_loc[1], up_loc[0], up_loc[1]);


            // draw ellipse
            int A = _width / 2;
            int B = _height / 2;

            // center ellipse
            int x = start_x + A;
            int y = start_y + B;

            DrawEllipse(x, y, A, B);
            ClearLoc();
        }

        // попиксельная отрисовка прямоугольника
        private void DrawRectangle(int s_x, int s_y, int e_x, int e_y)
        {
            int width = Math.Abs(s_x - e_x);
            int height = Math.Abs(s_y - e_y);

            _height = height;
            _width = width;

            Console.WriteLine($"x1={s_x}, y1={s_y}, x2={e_x}, y2={e_y}");

            int x = s_x <= e_x ? s_x : e_x;
            int y = s_y <= e_y ? s_y : e_y;
            start_x = x;
            start_y = y;

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

        // симметричная отрисовка пикселя
        void simmetrical_pixel(int x0, int y0, int x1, int y1)
        {
            SetPixel(x0 + x1, y0 + y1);
            SetPixel(x0 - x1, y0 - y1);
            SetPixel(x0 + x1, y0 - y1);
            SetPixel(x0 - x1, y0 + y1);
        }

        private void DrawEllipse(int x, int y, int A, int B)
        {
            int x1 = 0;
            int y1 = B;
            // большая полуось
            int A_pow = Pow(A, 2);
            // малая полуось
            int B_pow = Pow(B, 2);

            // (x, y / 2)
            int d = 4 * B_pow * Pow(x1, 2) + A_pow * Pow(2 * y1, 2) - 4 * A_pow * B_pow;

            // Первая часть дуги
            while (A_pow * 2 * y1 > 2 * B_pow * x1)
            {
                simmetrical_pixel(x, y, x1, y1);
                // по горизонтали
                if (d < 0)
                {
                    x1 += 1;
                    d += 4 * B_pow * 2 * x1;
                }
                // по диагонали
                else
                {
                    x1++;
                    d = d - 8 * A_pow * y1 + 4 * B_pow * 2 * x1;
                    y1--;
                }
            }

            //(x / 2, y )
            d = B_pow * Pow(2 * x1, 2) + 4 * A_pow * Pow(y1, 2) - 4 * A_pow * B_pow;

            // вторая часть дуги
            while (y1 + 1 != 0)
            {
                simmetrical_pixel(x, y, x1, y1);
                // по вертикали
                if (d < 0)
                {
                    y1 -= 1;
                    d += 4 * A_pow * 2 * y1;
                }
                // по диагонали
                else
                {
                    y1 -= 1;
                    d = d - 8 * B_pow * x1 + 4 * A_pow * 2 * y1;
                    x1++;
                }
            }
        }

        int Pow(int x, int pow)
        {
            int ret = 1;
            while (pow != 0)
            {
                if ((pow & 1) == 1)
                    ret *= x;
                x *= x;
                pow >>= 1;
            }
            return ret;
        }

        private void SetPixel(int x, int y)
        {
            Console.WriteLine($"x={x} y={y}");
            bitmap.SetPixel(x, y, color);
        }

        private void ClearLoc()
        {
            down_loc.Clear();
            up_loc.Clear();
        }
    }
}
