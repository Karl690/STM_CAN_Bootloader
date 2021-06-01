using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CanREL
{
    public partial class ProgressForm : Form
    {
        public ProgressForm()
        {
            InitializeComponent();
            progressBar1.Maximum = 100;
            timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (Main.main.ProgressFirmwareDataSize > 0)
            {
                if (Main.main.ProgressFirmwareCounter < Main.main.ProgressFirmwareDataSize)
                    progressBar1.Value = (int)((Main.main.ProgressFirmwareCounter / (float)Main.main.ProgressFirmwareDataSize) * 100);
                else
                    this.Close();
            }
            else
                this.Close();
            
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Main.main.IsCanFirmwareUpdating = false;
            this.Close();
        }
    }
}
