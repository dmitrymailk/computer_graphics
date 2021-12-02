using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab_7
{
    public partial class Form1 : Form
    {
        int h;
        int w;
        public Form1()
        {
            InitializeComponent();
            //Console.WriteLine("");
            w = 800;
            h = 600;
            Size = new Size(w, h);
            pictureBox1.Size = new System.Drawing.Size(w, h);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

            const float zoom = 0.5f;
            const int maxiter = 255;

            // двигаем наш центр отрисовки если нужно
            const int moveX = 0;
            const int moveY = 0;

            // устанавливаем параметры нашей функции z + c = z + Cr + CIm 
            const double realC = -0.1194;
            const double imaginaryC = 0.6289;
            double zReal, zIm, tmp;
            const int Radius = 2;
            int i;

            var colors = (from c in Enumerable.Range(0, 256)
                          select Color.FromArgb(
                              //(c * 4 + 110) % 255,
                              //(c * 3 + 150) % 255,
                              //(c * 7 + 100) % 255)
                              c, c, c)
                          ).ToArray();

            var bitmap = new Bitmap(w, h);
            for (int x = 0; x < w; x++)
            {
                for (int y = 0; y < h; y++)
                {
                    // центруем координаты
                    zReal = (x - w / 2) / (0.5 * zoom * w) + moveX;
                    zIm = (y - h / 2) / (0.5 * zoom * h) + moveY;
                    i = maxiter;
                    // выполняем наше преобразование до тех пока не 
                    // достигнем максимума по абсолютному значению воображаемых чисел и реальных 
                    // либо пока не выполним максимальное количество итераций
                    while (zReal * zReal + zIm * zIm < Radius * Radius && i > 1)
                    {
                        tmp = zReal * zReal - zIm * zIm + realC;
                        zIm = 2 * zReal * zIm + imaginaryC;
                        zReal = tmp;
                        i -= 1;
                    }
                    bitmap.SetPixel(x, y, colors[i]);
                }
            }

            pictureBox1.Image = bitmap;
        }
    }
}
