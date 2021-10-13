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
        bool is_down = false;
        bool is_up = false;
        bool is_move = false;

        private Bitmap bitmap;

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
            ClearLoc();
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            //Console.WriteLine("MOVe");

        }

        private void DrawRectangle(int s_x, int s_y, int e_x, int e_y)
        {
            int width = Math.Abs(s_x - e_x);
            int height = Math.Abs(s_y - e_y);

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

        private void SetPixel(int x, int y)
        {
            bitmap.SetPixel(x, y, color);
        }

        private void ClearLoc()
        {
            down_loc.Clear();
            up_loc.Clear();
        }
    }
}
