using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab_4
{
    public partial class Form1 : Form
    {
        private Bitmap bitmap;
        private List<Point> dots;
        private List<List<Point>> dotsLines;
        private Color color;

        private int max_x = 0;
        private int min_x = 0;
        private int max_y = 0;
        private int min_y = 0;

        public Form1()
        {
            InitializeComponent();

            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            dots = new List<Point>();
            dotsLines = new List<List<Point>>();
            color = Color.FromArgb(0, 0, 0);
            Console.WriteLine("Start");

            min_x = pictureBox1.Width;
            min_y = pictureBox1.Height;

            dotsLines = new List<List<Point>>(pictureBox1.Height);
            for (int i = 0; i < pictureBox1.Height; i++)
            {
                dotsLines.Add(new List<Point>(pictureBox1.Width));
            }
        }

        void DetermineBounds(int x, int y)
        {
            min_x = Math.Min(min_x, x);
            min_y = Math.Min(min_y, y);
            max_x = Math.Max(max_x, x);
            max_y = Math.Max(max_y, y);
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Console.WriteLine($"DOWN {e.X} {e.Y}");
                Point coords = new Point(e.X, e.Y);

                DetermineBounds(e.X, e.Y);

                color = Color.FromArgb(0, 0, 0);
                bitmap.SetPixel(e.X, e.Y, color);
                dots.Add(coords);
                pictureBox1.Image = bitmap;

                ConnectDots();
            }

            if (e.Button == MouseButtons.Right)
            {
                if (dots.Count < 2) return;

                int start_x = dots[0].X;
                int start_y = dots[0].Y;
                int end_x = dots[dots.Count - 1].X;
                int end_y = dots[dots.Count - 1].Y;
                color = Color.FromArgb(77, 5, 232);
                drawLine(start_x, start_y, end_x, end_y);
                pictureBox1.Image = bitmap;

                FillFigure();
                ConnectDots();
            }
        }

        void FillFigure()
        {
            Console.WriteLine("Fill rectangle");

            int len = dots.Count;
            // запускаем цикл от самой маленькой до самой большой точки У
            for (int y = min_y; y <= max_y; y++)
            {
                int yMin, yMax;
                bool yModule;
                for (int pt = 0; pt < len; pt++)
                {
                    int ptNext = (pt + 1) % len;

                    // если координата текущей точки совпадает со следующей, то пропускаем
                    if (dots[pt].Y == dots[ptNext].Y)
                    {
                        continue;
                    }
                    // нахождение крайних координат для точек
                    yMax = Math.Max(dots[pt].Y, dots[ptNext].Y);
                    yMin = Math.Min(dots[pt].Y, dots[ptNext].Y);
                    yModule = dots[pt].Y > dots[ptNext].Y;

                    // если точк
                    if (y >= yMin && y <= yMax)
                    {
                        // если одна прямая, то добавляем первую  
                        if (dots[pt].X - dots[ptNext].X == 0)
                        {
                            dotsLines[y].Add(new Point(dots[pt].X, y));
                        }
                        else if (y != yMin)
                        {
                            // вычисление точки пересечения первой
                            double k = (double)(yMin - yMax) / (yModule ? (dots[ptNext].X - dots[pt].X) : (dots[pt].X - dots[ptNext].X));
                            double b = dots[pt].Y - k * dots[pt].X;
                            // пересечение 2 координат
                            double dotCross = ((b - y) / (0 - k));
                            int xCross = (int)Math.Round(dotCross);
                            dotsLines[y].Add(new Point(xCross, y));
                        }
                    }
                }
            }


            for (int y = min_y; y <= max_y; y++)
            {
                Point prevPoint = new Point();
                dotsLines[y].Sort((a, b) => a.X.CompareTo(b.X));
                foreach (Point point in dotsLines[y])
                {
                    // построчно заливаем фигуру
                    if (!prevPoint.IsEmpty)
                    {
                        drawLine(prevPoint.X, prevPoint.Y, point.X, point.Y);
                        prevPoint = new Point();
                    }
                    else
                    {
                        prevPoint = point;
                    }
                }
            }
        }

        private void ConnectDots()
        {
            Console.WriteLine("Draw line");
            int amount = dots.Count;
            if (amount < 2) return;
            Console.WriteLine($"Amount {amount}");

            //bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);

            for (int i = 0; i < amount - 1; i++)
            {
                int start_x = dots[i].X;
                int start_y = dots[i].Y;
                int end_x = dots[(i + 1)].X;
                int end_y = dots[(i + 1)].Y;

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


    }
}
