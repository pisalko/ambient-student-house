namespace Laptop_application_final
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.labUID = new System.Windows.Forms.Label();
            this.lbName = new System.Windows.Forms.Label();
            this.lbAge = new System.Windows.Forms.Label();
            this.tbName = new System.Windows.Forms.TextBox();
            this.tbAge = new System.Windows.Forms.TextBox();
            this.btnRegister = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.lvAvailableRooms = new System.Windows.Forms.ListView();
            this.lbRooms = new System.Windows.Forms.Label();
            this.btnRemove = new System.Windows.Forms.Button();
            this.labRegistered = new System.Windows.Forms.Label();
            this.lbRegistered = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // labUID
            // 
            this.labUID.AutoSize = true;
            this.labUID.Location = new System.Drawing.Point(12, 25);
            this.labUID.Name = "labUID";
            this.labUID.Size = new System.Drawing.Size(35, 13);
            this.labUID.TabIndex = 0;
            this.labUID.Text = "label1";
            // 
            // lbName
            // 
            this.lbName.AutoSize = true;
            this.lbName.Location = new System.Drawing.Point(12, 58);
            this.lbName.Name = "lbName";
            this.lbName.Size = new System.Drawing.Size(38, 13);
            this.lbName.TabIndex = 1;
            this.lbName.Text = "Name:";
            // 
            // lbAge
            // 
            this.lbAge.AutoSize = true;
            this.lbAge.Location = new System.Drawing.Point(160, 61);
            this.lbAge.Name = "lbAge";
            this.lbAge.Size = new System.Drawing.Size(29, 13);
            this.lbAge.TabIndex = 2;
            this.lbAge.Text = "Age:";
            // 
            // tbName
            // 
            this.tbName.Location = new System.Drawing.Point(55, 58);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(100, 20);
            this.tbName.TabIndex = 3;
            // 
            // tbAge
            // 
            this.tbAge.Location = new System.Drawing.Point(194, 58);
            this.tbAge.Name = "tbAge";
            this.tbAge.Size = new System.Drawing.Size(100, 20);
            this.tbAge.TabIndex = 4;
            // 
            // btnRegister
            // 
            this.btnRegister.Location = new System.Drawing.Point(14, 242);
            this.btnRegister.Name = "btnRegister";
            this.btnRegister.Size = new System.Drawing.Size(278, 23);
            this.btnRegister.TabIndex = 10;
            this.btnRegister.Text = "Register new user";
            this.btnRegister.UseVisualStyleBackColor = true;
            this.btnRegister.Click += new System.EventHandler(this.btnRegister_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.PortName = "COM13";
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 10;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // lvAvailableRooms
            // 
            this.lvAvailableRooms.BackColor = System.Drawing.Color.White;
            this.lvAvailableRooms.GridLines = true;
            this.lvAvailableRooms.HideSelection = false;
            this.lvAvailableRooms.Location = new System.Drawing.Point(14, 111);
            this.lvAvailableRooms.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.lvAvailableRooms.Name = "lvAvailableRooms";
            this.lvAvailableRooms.Size = new System.Drawing.Size(279, 118);
            this.lvAvailableRooms.TabIndex = 2;
            this.lvAvailableRooms.TileSize = new System.Drawing.Size(35, 45);
            this.lvAvailableRooms.UseCompatibleStateImageBehavior = false;
            // 
            // lbRooms
            // 
            this.lbRooms.AutoSize = true;
            this.lbRooms.Location = new System.Drawing.Point(12, 96);
            this.lbRooms.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.lbRooms.Name = "lbRooms";
            this.lbRooms.Size = new System.Drawing.Size(89, 13);
            this.lbRooms.TabIndex = 30;
            this.lbRooms.Text = "Available Rooms:";
            // 
            // btnRemove
            // 
            this.btnRemove.Location = new System.Drawing.Point(326, 242);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(278, 23);
            this.btnRemove.TabIndex = 31;
            this.btnRemove.Text = "Remove Tag ";
            this.btnRemove.UseVisualStyleBackColor = true;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // labRegistered
            // 
            this.labRegistered.AutoSize = true;
            this.labRegistered.Location = new System.Drawing.Point(323, 25);
            this.labRegistered.Name = "labRegistered";
            this.labRegistered.Size = new System.Drawing.Size(121, 13);
            this.labRegistered.TabIndex = 32;
            this.labRegistered.Text = "Registered guests && info";
            // 
            // lbRegistered
            // 
            this.lbRegistered.FormattingEnabled = true;
            this.lbRegistered.Location = new System.Drawing.Point(326, 58);
            this.lbRegistered.Name = "lbRegistered";
            this.lbRegistered.Size = new System.Drawing.Size(278, 173);
            this.lbRegistered.TabIndex = 33;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(619, 276);
            this.Controls.Add(this.lbRegistered);
            this.Controls.Add(this.labRegistered);
            this.Controls.Add(this.btnRemove);
            this.Controls.Add(this.lbRooms);
            this.Controls.Add(this.lvAvailableRooms);
            this.Controls.Add(this.btnRegister);
            this.Controls.Add(this.tbAge);
            this.Controls.Add(this.tbName);
            this.Controls.Add(this.lbAge);
            this.Controls.Add(this.lbName);
            this.Controls.Add(this.labUID);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labUID;
        private System.Windows.Forms.Label lbName;
        private System.Windows.Forms.Label lbAge;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.TextBox tbAge;
        private System.Windows.Forms.Button btnRegister;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ListView lvAvailableRooms;
        private System.Windows.Forms.Label lbRooms;
        private System.Windows.Forms.Button btnRemove;
        private System.Windows.Forms.Label labRegistered;
        private System.Windows.Forms.ListBox lbRegistered;
    }
}

