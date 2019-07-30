namespace echoClient_csharp
{
    partial class client
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnConnect = new System.Windows.Forms.Button();
            this.checkBoxLocalhost = new System.Windows.Forms.CheckBox();
            this.serverInfoBox = new System.Windows.Forms.GroupBox();
            this.serverPort = new System.Windows.Forms.TextBox();
            this.labelServerPort = new System.Windows.Forms.Label();
            this.labelServerIP = new System.Windows.Forms.Label();
            this.serverIP = new System.Windows.Forms.TextBox();
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.echoMsg = new System.Windows.Forms.TextBox();
            this.labelEchoMsg = new System.Windows.Forms.Label();
            this.btnEchoSend = new System.Windows.Forms.Button();
            this.currentStateText = new System.Windows.Forms.Label();
            this.labelCurrentState = new System.Windows.Forms.Label();
            this.logList = new System.Windows.Forms.ListBox();
            this.logMsgBox = new System.Windows.Forms.GroupBox();
            this.serverInfoBox.SuspendLayout();
            this.logMsgBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnConnect
            // 
            this.btnConnect.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnConnect.Location = new System.Drawing.Point(516, 41);
            this.btnConnect.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(123, 33);
            this.btnConnect.TabIndex = 0;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.BtnConnect_Click);
            // 
            // checkBoxLocalhost
            // 
            this.checkBoxLocalhost.AutoSize = true;
            this.checkBoxLocalhost.Checked = true;
            this.checkBoxLocalhost.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLocalhost.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.checkBoxLocalhost.Location = new System.Drawing.Point(351, 32);
            this.checkBoxLocalhost.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.checkBoxLocalhost.Name = "checkBoxLocalhost";
            this.checkBoxLocalhost.Size = new System.Drawing.Size(119, 26);
            this.checkBoxLocalhost.TabIndex = 1;
            this.checkBoxLocalhost.Text = "Localhost";
            this.checkBoxLocalhost.UseVisualStyleBackColor = true;
            // 
            // serverInfoBox
            // 
            this.serverInfoBox.Controls.Add(this.serverPort);
            this.serverInfoBox.Controls.Add(this.labelServerPort);
            this.serverInfoBox.Controls.Add(this.labelServerIP);
            this.serverInfoBox.Controls.Add(this.serverIP);
            this.serverInfoBox.Controls.Add(this.checkBoxLocalhost);
            this.serverInfoBox.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.serverInfoBox.Location = new System.Drawing.Point(12, 12);
            this.serverInfoBox.Name = "serverInfoBox";
            this.serverInfoBox.Size = new System.Drawing.Size(480, 122);
            this.serverInfoBox.TabIndex = 2;
            this.serverInfoBox.TabStop = false;
            this.serverInfoBox.Text = "Server Info";
            // 
            // serverPort
            // 
            this.serverPort.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.serverPort.Location = new System.Drawing.Point(169, 71);
            this.serverPort.Name = "serverPort";
            this.serverPort.Size = new System.Drawing.Size(159, 29);
            this.serverPort.TabIndex = 5;
            this.serverPort.Text = "23452";
            // 
            // labelServerPort
            // 
            this.labelServerPort.AutoSize = true;
            this.labelServerPort.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelServerPort.Location = new System.Drawing.Point(18, 74);
            this.labelServerPort.Name = "labelServerPort";
            this.labelServerPort.Size = new System.Drawing.Size(120, 22);
            this.labelServerPort.TabIndex = 4;
            this.labelServerPort.Text = "Server Port";
            // 
            // labelServerIP
            // 
            this.labelServerIP.AutoSize = true;
            this.labelServerIP.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelServerIP.Location = new System.Drawing.Point(18, 32);
            this.labelServerIP.Name = "labelServerIP";
            this.labelServerIP.Size = new System.Drawing.Size(150, 22);
            this.labelServerIP.TabIndex = 3;
            this.labelServerIP.Text = "Server Address";
            // 
            // serverIP
            // 
            this.serverIP.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.serverIP.Location = new System.Drawing.Point(169, 29);
            this.serverIP.Name = "serverIP";
            this.serverIP.Size = new System.Drawing.Size(159, 29);
            this.serverIP.TabIndex = 2;
            this.serverIP.Text = "0.0.0.0";
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDisconnect.Location = new System.Drawing.Point(516, 83);
            this.btnDisconnect.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(123, 33);
            this.btnDisconnect.TabIndex = 3;
            this.btnDisconnect.Text = "Disconnect";
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.BtnDisconnect_Click);
            // 
            // echoMsg
            // 
            this.echoMsg.Location = new System.Drawing.Point(148, 152);
            this.echoMsg.Name = "echoMsg";
            this.echoMsg.Size = new System.Drawing.Size(343, 29);
            this.echoMsg.TabIndex = 4;
            this.echoMsg.Text = "echo";
            // 
            // labelEchoMsg
            // 
            this.labelEchoMsg.AutoSize = true;
            this.labelEchoMsg.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelEchoMsg.Location = new System.Drawing.Point(12, 155);
            this.labelEchoMsg.Name = "labelEchoMsg";
            this.labelEchoMsg.Size = new System.Drawing.Size(130, 22);
            this.labelEchoMsg.TabIndex = 7;
            this.labelEchoMsg.Text = "Echo Message";
            // 
            // btnEchoSend
            // 
            this.btnEchoSend.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnEchoSend.Location = new System.Drawing.Point(516, 150);
            this.btnEchoSend.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnEchoSend.Name = "btnEchoSend";
            this.btnEchoSend.Size = new System.Drawing.Size(123, 33);
            this.btnEchoSend.TabIndex = 8;
            this.btnEchoSend.Text = "Send Echo";
            this.btnEchoSend.UseVisualStyleBackColor = true;
            this.btnEchoSend.Click += new System.EventHandler(this.BtnEchoSend_Click);
            // 
            // currentStateText
            // 
            this.currentStateText.AutoSize = true;
            this.currentStateText.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.currentStateText.Location = new System.Drawing.Point(169, 447);
            this.currentStateText.Name = "currentStateText";
            this.currentStateText.Size = new System.Drawing.Size(130, 22);
            this.currentStateText.TabIndex = 6;
            this.currentStateText.Text = "Disconnected";
            // 
            // labelCurrentState
            // 
            this.labelCurrentState.AutoSize = true;
            this.labelCurrentState.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCurrentState.Location = new System.Drawing.Point(12, 447);
            this.labelCurrentState.Name = "labelCurrentState";
            this.labelCurrentState.Size = new System.Drawing.Size(160, 22);
            this.labelCurrentState.TabIndex = 9;
            this.labelCurrentState.Text = "Current State :";
            // 
            // logList
            // 
            this.logList.FormattingEnabled = true;
            this.logList.ItemHeight = 22;
            this.logList.Location = new System.Drawing.Point(20, 28);
            this.logList.Name = "logList";
            this.logList.Size = new System.Drawing.Size(580, 180);
            this.logList.TabIndex = 0;
            // 
            // logMsgBox
            // 
            this.logMsgBox.Controls.Add(this.logList);
            this.logMsgBox.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.logMsgBox.Location = new System.Drawing.Point(16, 204);
            this.logMsgBox.Name = "logMsgBox";
            this.logMsgBox.Size = new System.Drawing.Size(623, 226);
            this.logMsgBox.TabIndex = 6;
            this.logMsgBox.TabStop = false;
            this.logMsgBox.Text = "Log Message";
            // 
            // client
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 22F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(658, 478);
            this.Controls.Add(this.logMsgBox);
            this.Controls.Add(this.labelCurrentState);
            this.Controls.Add(this.currentStateText);
            this.Controls.Add(this.btnEchoSend);
            this.Controls.Add(this.labelEchoMsg);
            this.Controls.Add(this.echoMsg);
            this.Controls.Add(this.btnDisconnect);
            this.Controls.Add(this.serverInfoBox);
            this.Controls.Add(this.btnConnect);
            this.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.Name = "client";
            this.Text = "Test Echo Client";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Client_FormClosed);
            this.Load += new System.EventHandler(this.Client_Load);
            this.serverInfoBox.ResumeLayout(false);
            this.serverInfoBox.PerformLayout();
            this.logMsgBox.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.CheckBox checkBoxLocalhost;
        private System.Windows.Forms.GroupBox serverInfoBox;
        private System.Windows.Forms.TextBox serverPort;
        private System.Windows.Forms.Label labelServerPort;
        private System.Windows.Forms.Label labelServerIP;
        private System.Windows.Forms.TextBox serverIP;
        private System.Windows.Forms.TextBox echoMsg;
        private System.Windows.Forms.Label labelEchoMsg;
        private System.Windows.Forms.Button btnEchoSend;
        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Label currentStateText;
        private System.Windows.Forms.Label labelCurrentState;
        private System.Windows.Forms.ListBox logList;
        private System.Windows.Forms.GroupBox logMsgBox;
    }
}

