using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab_1
{
    public partial class Form1 : Form
    {
        private Bitmap bitmap;
        private List<List<int>> dots;
        private Color color;
        public Form1()
        {
            InitializeComponent();
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            dots = new List<List<int>>();
            color = Color.FromArgb(246, 45, 71);
        }

        private void MouseClickEvent(object sender, MouseEventArgs e)
        {

            if (e.Button == MouseButtons.Left)
            {
                List<int> coords = new List<int>();
                coords.Add(e.X);
                coords.Add(e.Y);

                color = Color.FromArgb(0, 0, 0);
                bitmap.SetPixel(e.X, e.Y, color);
                dots.Add(coords);
                pictureBox1.Image = bitmap;
            }
            else if (e.Button == MouseButtons.Right)
            {
                Console.WriteLine("Draw line");
                int amount = dots.Count;
                if (amount < 2) return;

                bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
                color = Color.FromArgb(246, 45, 71);

                for (int i = 0; i < amount; i++)
                {
                    int start_x = dots[i % amount][0];
                    int start_y = dots[i % amount][1];
                    int end_x = dots[(i + 1) % amount][0];
                    int end_y = dots[(i + 1) % amount][1];

                    drawLine(start_x, start_y, end_x, end_y);
                }

                pictureBox1.Image = bitmap;
                dots.Clear();
            }
        }

        // реализация алгоритма Брезенхейма
        private void drawLine(int xa, int ya, int xb, int yb)
        {

            int dy = Math.Abs(yb - ya);
            int dx = Math.Abs(xb - xa);
            int d;
            int sx = xb >= xa ? 1 : -1; // направление приращения x
            int sy = yb >= ya ? 1 : -1; // направление приращения y

            // если наклон не слишком сильный, то
            // идет приращение по x, иначе по y
            if (dy <= dx)
            {
                d = (2 * dy) - dx;
                // умножение на 2 появляется в результате попытки ухода от float
                // и чтобы не сравнивать значение ошибки с 0.5, а с 0 мы умножаем на 2
                int d1 = dy * 2;
                int d2 = (dy - dx) * 2;
                int x = xa + sx;
                int y = ya;


                for (int i = 1; i <= dx; i++, x += sx)
                {
                    // считаем ошибку округления
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
                    // считаем ошибку округления
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



    }


}
