using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Threading;

using OAFKayak;
using Kayak;
using Kayak.Http;

namespace OAFControl
{
    


   


   

    public partial class Form1 : Form
    {
        private static WorkerKayak workerObject;
        private bool allowshowdisplay = false;
        private Thread workerThread;

        public Form1()
        {
            InitializeComponent();

            workerObject = new WorkerKayak();
            workerThread = new Thread(workerObject.Start);
            workerThread.Start();
           
        }

        protected override void SetVisibleCore(bool value)
        {
            base.SetVisibleCore(allowshowdisplay ? value : allowshowdisplay);
        }


       

        private void Form1_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState)
                Hide();
        }

        private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
        {

            this.allowshowdisplay = true;
            this.Visible = !this.Visible; 
            Show();
            WindowState = FormWindowState.Normal;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Hide();
        }

        private void infoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Show();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            workerThread.Abort(); 
            Application.Exit();
        }

    }
	
	 public class WorkerKayak
    {
         

        public void Start()
        {
            var  scheduler = KayakScheduler.Factory.Create(new SchedulerDelegate());
            var  server = KayakServer.Factory.CreateHttp(new RequestDelegate(), scheduler);

            using (server.Listen(new IPEndPoint(IPAddress.Any, 8080)))
            {
                // runs scheduler on calling thread. this method will block until
                // someone calls Stop() on the scheduler.
                scheduler.Start();
            }
        }
       
            

    }
}
