using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace lab_5
{
    public partial class Form1 : Form
    {
        Graphics g;
        Pen pen;
        List<Matrix> cubeCoors;
        double delta = 0.001;
        int scale = 100;
        double angle = 0.0174533;
        public Form1()
        {
            InitializeComponent();
            g = pictureBox1.CreateGraphics();
            pen = new Pen(Brushes.Black);

            var model = Matrix.IdentityMatrix(3, 3);

            cubeCoors = new List<Matrix> {
                new Matrix(new double[,] { { 0, 0, 0 } }),
                new Matrix(new double[,] { {0, 0, 1 } }),
                new Matrix(new double[,] { {0, 1, 1 } }),
                new Matrix(new double[,] { {0, 1, 0 } })
            };

            Scale();
            Translations();
        }



        private int toInt(double x)
        {
            var some = (int)Math.Floor(x) + 100;
            Console.WriteLine(some);
            return some;
        }

        private void DrawCube()
        {

            //Clear();
            g.Clear(Color.White);

            for (int i = 0; i < 5; i++)
            {
                g.DrawLine(pen,
                    toInt((cubeCoors[i % 4][0, 0] + delta)),
                    toInt((cubeCoors[i % 4][0, 1] + delta)),
                    toInt((cubeCoors[(i + 1) % 4][0, 0] + delta)),
                    toInt((cubeCoors[(i + 1) % 4][0, 1] + delta))
                   );
            }

        }

        private void Rotations()
        {

            for (int i = 0; i < 4; i++)
            {
                cubeCoors[i] = rotate(cubeCoors[i], angle, false, false, true);
            }
            //angle += 0.0174533;

        }

        private void Scale()
        {
            for (int i = 0; i < 4; i++)
            {
                cubeCoors[i] = scale * cubeCoors[i];
            }
        }

        private void Translations()
        {
            for (int i = 0; i < 4; i++)
            {
                cubeCoors[i] = translate(cubeCoors[i], new double[] { 400, 100, 0 });
            }
        }

        private Matrix translate(Matrix mat, double[] vec)
        {
            for (int i = 0; i < mat.rows; i++)
            {
                for (int j = 0; j < mat.cols; j++)
                {
                    mat[i, j] += vec[j];
                }
                DrawCube();
            }

            return mat;
        }

        // https://en.wikipedia.org/wiki/Rotation_matrix
        private Matrix rotate(
            Matrix mat,
            double angle,
            bool x = false,
            bool y = false,
            bool z = false
            )
        {
            var cos = Math.Cos(angle);
            var sin = Math.Sin(angle);

            var Rx = new Matrix(new double[,] {
                {1, 0, 0 },
                {0, cos,  -sin},
                {0, sin, cos }
            });

            var Ry = new Matrix(new double[,] {
                {cos, 0, sin },
                {0, 1, 0 },
                {-sin, 0, cos }
            });

            var Rz = new Matrix(new double[,] {
                {cos, -sin, 0 },
                {sin, cos, 0 },
                {0, 0, 1 }
            });

            if (x) mat = mat * Rx;
            if (y) mat = mat * Ry;
            if (z) mat = mat * Rz;



            return mat;
        }
        private void pictureBox1_Click(object sender, EventArgs e)
        {


            DrawCube();
            Rotations();
        }
    }
}
