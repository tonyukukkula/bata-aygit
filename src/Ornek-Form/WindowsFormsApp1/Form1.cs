//mevzubahis arduino kodları için örnek windows form app
//Yazar: Alperen Kula
//Yazım Tarihi: 20.05.2019
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {

        public static SerialPort mp;

        public Form1()
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            InitializeComponent();
            //
            //ldr();
            //Console.WriteLine(ldr());
            //Console.ReadLine();
            mp = new SerialPort();
            mp.PortName = "COM3";
            mp.BaudRate = 9600;
        }
        private void Label1_Click(object sender, EventArgs e)
        {
            label2.Text = ldr();
        }
        static public string ldr()
        {
          //  int deger;
          //  mp.WriteLine("ldr");
            string b = mp.ReadTo(";");
            //deger = Convert.ToInt32();
            return b;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            mp.Open();
        }
        private void tarat() {
            //if (!mp.IsOpen) { mp.Open(); }
            Thread.Sleep(400);
            mp.WriteLine("t");
            string b = mp.ReadTo(";");
            label2.Text = b;
            //mp.Dispose();
           // mp.Close();
        }
        private void Button1_Click(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();
        }

        private void Button2_Click(object sender, EventArgs e)
        {
            //if (!mp.IsOpen) { mp.Open(); }
            Thread.Sleep(400);
            mp.WriteLine("k");
            //Convert.ToInt32(ta.Text);
            Thread.Sleep(400);
            mp.WriteLine(ta.Text);
            Thread.Sleep(400);

            //if(mp.ReadTo("#")== "Waiting for valid finger to enroll as ")
            //mp.Close();
            //Please type in the ID # (from 1 to 127) you want to save this finger as $
        }

        private void Timer1_Tick(object sender, EventArgs e)
        {        }
        private void Button3_Click(object sender, EventArgs e)
        {
            //if (!mp.IsOpen) { mp.Open(); }
            Thread.Sleep(400);
            mp.WriteLine("s");
            //Convert.ToInt32(ta.Text);
            Thread.Sleep(400);
            mp.WriteLine(ta1.Text);
            //if(mp.ReadTo("#")== "Waiting for valid finger to enroll as ")
           // mp.Dispose();
           // mp.Close();
            //Please type in the ID # (from 1 to 127) you want to save this finger as $
        }

        private void Ta1_TextChanged(object sender, EventArgs e)
        {

        }

        private void BackgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            tarat();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            //mp.Dispose();
            //mp.Close();
        }
    }
}
