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
        private Color color;
        private Bitmap bitmap;

        private List<Vector3> coords;
        private List<Vector3> projectedCoords;

        // параметры поворота для всех осей
        private double angleZ = 0;
        private double angleX = 0;
        private double angleY = 0;

        // параметры скейла для всех осей
        private double scaleX = 150;
        private double scaleY = 150;
        private double scaleZ = 150;

        // матрица проекции
        private List<Vector3> projection;

        // матрицы поворота 
        private List<Vector3> rotationZ;
        private List<Vector3> rotationX;
        private List<Vector3> rotationY;

        // вектор переноса
        private Vector3 translation;

        // параметр переноса
        private double translationX = 1;
        private double translationY = 1;
        private double translationZ = 1;

        public Form1()
        {
            InitializeComponent();
            color = Color.FromArgb(0, 0, 0);
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            coords = new List<Vector3>();
            projectedCoords = new List<Vector3>();

            // задаем координаты куба в трехмерном пространстве
            coords.Add(new Vector3(-0.5, -0.5, -0.5));
            coords.Add(new Vector3(0.5, -0.5, -0.5));
            coords.Add(new Vector3(0.5, 0.5, -0.5));
            coords.Add(new Vector3(-0.5, 0.5, -0.5));

            coords.Add(new Vector3(-0.5, -0.5, 0.5));
            coords.Add(new Vector3(0.5, -0.5, 0.5));
            coords.Add(new Vector3(0.5, 0.5, 0.5));
            coords.Add(new Vector3(-0.5, 0.5, 0.5));

            // апдейт
            CustomUpdate();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        // очистить экран чтобы нарисовать что-то новое
        private void ClearScreen()
        {
            bitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            pictureBox1.Image = bitmap;
        }

        // соединяем точки на экране
        private void ConnectProjectedDots(int startNum, int endNum)
        {
            Vector3 start = projectedCoords[startNum];
            Vector3 end = projectedCoords[endNum];
            drawLine(start, end);
        }

        // поворачиваем наш куб по всем трем направлениям 
        private void UpdateRotation()
        {
            rotationZ = new List<Vector3>();
            rotationZ.Add(new Vector3(Math.Cos(angleZ), -Math.Sin(angleZ), 0));
            rotationZ.Add(new Vector3(Math.Sin(angleZ), Math.Cos(angleZ), 0));
            rotationZ.Add(new Vector3(0, 0, 1));

            rotationX = new List<Vector3>();
            rotationX.Add(new Vector3(1, 0, 0));
            rotationX.Add(new Vector3(0, Math.Cos(angleX), -Math.Sin(angleX)));
            rotationX.Add(new Vector3(0, Math.Sin(angleX), Math.Cos(angleX)));

            rotationY = new List<Vector3>();
            rotationY.Add(new Vector3(Math.Cos(angleY), 0, Math.Sin(angleY)));
            rotationY.Add(new Vector3(0, 1, 0));
            rotationY.Add(new Vector3(-Math.Sin(angleY), 0, Math.Cos(angleY)));

        }

        private void UpdateProjection()
        {
            // обновляем матрицу проекции
            projection = new List<Vector3>();

            projection.Add(new Vector3(1.0, 0.0, 0.0) * scaleX);
            projection.Add(new Vector3(0.0, 1.0, 0.0) * scaleY);
            projection.Add(new Vector3(0.0, 0.0, 1.0) * scaleZ);
        }


        // обновление всего экрана
        private void CustomUpdate()
        {
            ClearScreen();
            UpdateRotation();
            UpdateProjection();

            projectedCoords.Clear();

            // трансформация всех координат куба
            foreach (Vector3 item in coords)
            {
                Vector3 proj2D = item;
                var trans = new Vector3(translationX, translationY, translationZ);

                proj2D = matMul(projection, proj2D);
                proj2D = matMul(rotationX, proj2D);
                proj2D = matMul(rotationZ, proj2D);
                proj2D = matMul(rotationY, proj2D);
                proj2D += trans;

                projectedCoords.Add(proj2D);

                point(proj2D);
            }

            for (int i = 0; i < 4; i++)
            {
                ConnectProjectedDots(i, (i + 1) % 4);
                ConnectProjectedDots(i + 4, ((i + 1) % 4) + 4);
                ConnectProjectedDots(i, i + 4);
            }
        }


        // перемножение матриц из трехмерного пространства
        private Vector3 matMul(List<Vector3> matrix, Vector3 toProjVec)
        {
            var proj2D = new Vector3();
            double x = 0;
            double y = 0;
            double z = 0;


            for (int j = 0; j < 3; j++)
            {
                x += matrix[0][j] * toProjVec[j];
                y += matrix[1][j] * toProjVec[j];
                z += matrix[2][j] * toProjVec[j];
            }
            proj2D.x = x;
            proj2D.y = y;
            proj2D.z = z;

            return proj2D;
        }

        // поставить точку на экране перегрузка
        private void point(Vector3 vec3)
        {
            point(vec3.x, vec3.y);
        }


        private void point(double _x, double _y)
        {
            int x = (int)Math.Floor(_x);
            int y = (int)Math.Floor(_y);

            x += pictureBox1.Width / 2;
            y += pictureBox1.Height / 2;

            //if (x > 0 && x < pictureBox1.Width && y > 0 && y < pictureBox1.Height)
            //{
            //    bitmap.SetPixel(x, y, color);
            //    pictureBox1.Image = bitmap;
            //}
            x = Math.Abs(x % pictureBox1.Width);
            y = Math.Abs(y % pictureBox1.Height);

            bitmap.SetPixel(Math.Max(x, 0), Math.Max(0, y), color);
            pictureBox1.Image = bitmap;


        }

        // обрабатываем нажатия
        private void KeyDownHandler(object sender, KeyEventArgs e)
        {
            Keys key = e.KeyCode;


            switch (key)
            {
                case Keys.Z:
                    {
                        angleZ += 0.1;
                        break;
                    }
                case Keys.X:
                    {
                        angleX += 0.1;
                        break;
                    }
                case Keys.Y:
                    {
                        angleY += 0.1;
                        break;
                    }
                case Keys.Q:
                    {
                        translationX += 10;
                        break;
                    }
                case Keys.W:
                    {
                        translationY += 10;
                        break;
                    }
                case Keys.E:
                    {
                        translationZ += 10;
                        break;
                    }
                case Keys.A:
                    {
                        scaleX += 1;
                        break;
                    }
                case Keys.S:
                    {
                        scaleY += 1;
                        break;
                    }
                case Keys.D:
                    {
                        scaleZ += 1;
                        break;
                    }
            }

            CustomUpdate();
        }


        // рисуем линию
        private void drawLine(Vector3 start, Vector3 end)
        {
            int xa = (int)Math.Floor(start.x);
            int ya = (int)Math.Floor(start.y);
            int xb = (int)Math.Floor(end.x);
            int yb = (int)Math.Floor(end.y);

            drawLine(xa, ya, xb, yb);
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

                    point(x, y);
                }
            }
            else
            {
                d = (2 * dx) - dy;
                int d1 = 2 * dx;
                int d2 = (dx - dy) * 2;
                int x = xa;
                int y = ya + sy;
                point(x, y);

                for (int i = 1; i <= dy; i++, y += sy)
                {
                    if (d > 0)
                    {
                        d += d2;
                        x += sx;
                    }
                    else
                        d += d1;
                    point(x, y);
                }
            }
        }

    }
}


// собственный тип данных
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

    // норма матрицы
    public double norm()
    {
        return Math.Sqrt(x * x + y * y + z * z);
    }

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
    public static Vector3 operator *(Vector3 vec3, double scale)
    {
        return new Vector3(vec3.x * scale, vec3.y * scale, vec3.z * scale);
    }

    public static Vector3 operator +(Vector3 vec3_1, Vector3 vec3_2)
    {
        return new Vector3(vec3_1.x + vec3_2.x, vec3_1.y + vec3_2.y, vec3_1.z + vec3_2.z);
    }
    public override string ToString() => $"x={x} y={y} z={z}";
};

