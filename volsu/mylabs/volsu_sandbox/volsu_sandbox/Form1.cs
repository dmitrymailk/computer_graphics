using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace volsu_sandbox
{
    public partial class Form1 : Form
    {
        private Bitmap bitmap;
        private List<List<int>> dots;
        private Color color;
        public Form1()
        {
            InitializeComponent();
            Console.WriteLine("Start");
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            dots = new List<List<int>>();
            color = Color.FromArgb(0, 0, 0);
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


            }


        }

        private void SetPixel(int x, int y)
        {
            bitmap.SetPixel(x, y, color);
        }
    }
}
