using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CanREL
{
    public class InvokeControl
    {
        public static void InvokeControlVisible(Control ctrl, bool visible)
        {
            if (ctrl.InvokeRequired)
            {
                ctrl.Invoke((MethodInvoker)delegate ()
                {
                    ctrl.Visible = visible;

                });
            }
            else
                ctrl.Visible = visible;
        }
    }
}
