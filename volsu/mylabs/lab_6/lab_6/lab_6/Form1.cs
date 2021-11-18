using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace lab_6
{
    public partial class Form1 : Form
    {
        private Bitmap bitmap;
        private List<Vector3> coords;
        private Color color;
        private double angle = 1.0;

        private List<Vector3> projection;
        private List<Vector3> rotation;

        public Form1()
        {
            InitializeComponent();
            color = Color.FromArgb(0, 0, 0);
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            coords = new List<Vector3>();
            //pictureBox1.KeyDown += new KeyEventHandler(KeyDown);

            projection = new List<Vector3>();
            projection.Add(new Vector3(1.0, 0.0, 0.0));
            projection.Add(new Vector3(0.0, 1.0, 0.0));

            rotation = new List<Vector3>();
            rotation.Add(new Vector3(Math.Cos(angle), -Math.Sin(angle), 0));
            rotation.Add(new Vector3(Math.Sin(angle), Math.Cos(angle), 0));

            coords.Add(new Vector3(-50, -50, 0));
            coords.Add(new Vector3(50, -50, 0));
            coords.Add(new Vector3(50, 50, 0));
            coords.Add(new Vector3(-50, 50, 0));

            foreach (Vector3 item in coords)
            {
                var proj2D = matMul(projection, item);
                proj2D = matMul(rotation, proj2D);

                point(proj2D.x, proj2D.y);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void ClearScreen()
        {
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            pictureBox1.Image = bitmap;
        }

        private void CustomUpdate()
        {
            ClearScreen();

            rotation = new List<Vector3>();
            rotation.Add(new Vector3(Math.Cos(angle), -Math.Sin(angle), 0));
            rotation.Add(new Vector3(Math.Sin(angle), Math.Cos(angle), 0));

            foreach (Vector3 item in coords)
            {
                var proj2D = matMul(projection, item);
                proj2D = matMul(rotation, proj2D);

                point(proj2D.x, proj2D.y);
            }
        }

        private Vector3 matMul(List<Vector3> matrix, Vector3 toProjVec)
        {
            var proj2D = new Vector3();
            double x = 0;
            double y = 0;


            for (int j = 0; j < 3; j++)
            {
                x += matrix[0][j] * toProjVec[j];
                y += matrix[1][j] * toProjVec[j];
            }
            proj2D.x = x;
            proj2D.y = y;

            return proj2D;
        }

        private void point(double _x, double _y)
        {
            int x = (int)Math.Floor(_x);
            int y = (int)Math.Floor(_y);

            x += pictureBox1.Width / 2;
            y += pictureBox1.Height / 2;

            bitmap.SetPixel(x, y, color);
            pictureBox1.Image = bitmap;
        }

        private void KeyDownHandler(object sender, KeyEventArgs e)
        {

            if (e.KeyCode == Keys.Space)
            {
                Console.WriteLine("A pressed");
                angle += 1;
            }

            CustomUpdate();
        }


    }
}

class Vector3
{
    public Vector3(double _x = 0, double _y = 0, double _z = 0)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    public double x { get; set; }
    public double y { get; set; }
    public double z { get; set; }

    public double this[int key]
    {
        get
        {
            if (key == 0)
            {
                return x;
            }
            else if (key == 1)
            {
                return y;
            }
            else
            {
                return z;
            }
        }
    }

    public override string ToString() => $"x={x} y={y} z={z}";
};

class Vector2
{
    public Vector2(double _x = 0, double _y = 0)
    {
        x = _x;
        y = _y;
    }

    public double x { get; set; }
    public double y { get; set; }

    public override string ToString() => $"x={x} y={y}";
}

