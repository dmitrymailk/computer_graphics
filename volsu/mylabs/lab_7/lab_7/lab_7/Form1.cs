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
            Console.WriteLine("");
            w = 800;
            h = 600;
            Size = new Size(w, h);
            pictureBox1.Size = new System.Drawing.Size(w, h);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

            const int zoom = 1;
            const int maxiter = 255;
            const int moveX = 0;
            const int moveY = 0;
            const double cX = -0.7;
            const double cY = 0.27015;
            double zx, zy, tmp;
            const int R = 2;
            int i;

            var colors = (from c in Enumerable.Range(0, 256) select Color.FromArgb(c, c, c)).ToArray();
            //var color = Color.FromArgb(0, 0, 0);
            var bitmap = new Bitmap(w, h);
            for (int x = 0; x < w; x++)
            {
                for (int y = 0; y < h; y++)
                {
                    zx = 1.0 * (x - w / 2) / (0.5 * zoom * w) + moveX;
                    zy = 1.0 * (y - h / 2) / (0.5 * zoom * h) + moveY;
                    i = maxiter;
                    while (zx * zx + zy * zy < R * R && i > 1)
                    {
                        tmp = zx * zx - zy * zy + cX;
                        zy = 2 * zx * zy + cY;
                        zx = tmp;
                        i -= 1;
                    }
                    bitmap.SetPixel(x, y, colors[i]);
                }
            }

            pictureBox1.Image = bitmap;
        }
    }
}
