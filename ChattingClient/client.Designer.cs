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
            this.currentStateText = new System.Windows.Forms.Label();
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.echoMsg = new System.Windows.Forms.TextBox();
            this.labelEchoMsg = new System.Windows.Forms.Label();
            this.btnEchoSend = new System.Windows.Forms.Button();
            this.labelCurrentState = new System.Windows.Forms.Label();
            this.logMsgBox = new System.Windows.Forms.GroupBox();
            this.logList = new System.Windows.Forms.ListBox();
            this.LoginInfo = new System.Windows.Forms.GroupBox();
            this.btnLogout = new System.Windows.Forms.Button();
            this.btnLogin = new System.Windows.Forms.Button();
            this.pwText = new System.Windows.Forms.TextBox();
            this.pwLabel = new System.Windows.Forms.Label();
            this.idLabel = new System.Windows.Forms.Label();
            this.idText = new System.Windows.Forms.TextBox();
            this.RoomInfo = new System.Windows.Forms.GroupBox();
            this.btnChat = new System.Windows.Forms.Button();
            this.chat = new System.Windows.Forms.TextBox();
            this.curRoomNumberLabel = new System.Windows.Forms.Label();
            this.roomLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.roomTitle = new System.Windows.Forms.TextBox();
            this.title = new System.Windows.Forms.Label();
            this.btnCreate = new System.Windows.Forms.Button();
            this.titleLabel = new System.Windows.Forms.Label();
            this.chatLog = new System.Windows.Forms.ListBox();
            this.userList = new System.Windows.Forms.ListBox();
            this.btnLeave = new System.Windows.Forms.Button();
            this.btnEnter = new System.Windows.Forms.Button();
            this.userListLabel = new System.Windows.Forms.Label();
            this.roomNumberLabel = new System.Windows.Forms.Label();
            this.roomNumber = new System.Windows.Forms.TextBox();
            this.serverInfoBox.SuspendLayout();
            this.logMsgBox.SuspendLayout();
            this.LoginInfo.SuspendLayout();
            this.RoomInfo.SuspendLayout();
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
            // currentStateText
            // 
            this.currentStateText.AutoSize = true;
            this.currentStateText.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.currentStateText.Location = new System.Drawing.Point(169, 953);
            this.currentStateText.Name = "currentStateText";
            this.currentStateText.Size = new System.Drawing.Size(130, 22);
            this.currentStateText.TabIndex = 6;
            this.currentStateText.Text = "Disconnected";
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
            // labelCurrentState
            // 
            this.labelCurrentState.AutoSize = true;
            this.labelCurrentState.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCurrentState.Location = new System.Drawing.Point(12, 953);
            this.labelCurrentState.Name = "labelCurrentState";
            this.labelCurrentState.Size = new System.Drawing.Size(160, 22);
            this.labelCurrentState.TabIndex = 9;
            this.labelCurrentState.Text = "Current State :";
            // 
            // logMsgBox
            // 
            this.logMsgBox.Controls.Add(this.logList);
            this.logMsgBox.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.logMsgBox.Location = new System.Drawing.Point(16, 724);
            this.logMsgBox.Name = "logMsgBox";
            this.logMsgBox.Size = new System.Drawing.Size(623, 226);
            this.logMsgBox.TabIndex = 6;
            this.logMsgBox.TabStop = false;
            this.logMsgBox.Text = "Log Message";
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
            // LoginInfo
            // 
            this.LoginInfo.Controls.Add(this.btnLogout);
            this.LoginInfo.Controls.Add(this.btnLogin);
            this.LoginInfo.Controls.Add(this.pwText);
            this.LoginInfo.Controls.Add(this.pwLabel);
            this.LoginInfo.Controls.Add(this.idLabel);
            this.LoginInfo.Controls.Add(this.idText);
            this.LoginInfo.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LoginInfo.Location = new System.Drawing.Point(16, 192);
            this.LoginInfo.Name = "LoginInfo";
            this.LoginInfo.Size = new System.Drawing.Size(623, 114);
            this.LoginInfo.TabIndex = 6;
            this.LoginInfo.TabStop = false;
            this.LoginInfo.Text = "Login Info";
            // 
            // btnLogout
            // 
            this.btnLogout.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLogout.Location = new System.Drawing.Point(458, 66);
            this.btnLogout.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnLogout.Name = "btnLogout";
            this.btnLogout.Size = new System.Drawing.Size(123, 33);
            this.btnLogout.TabIndex = 11;
            this.btnLogout.Text = "Logout";
            this.btnLogout.UseVisualStyleBackColor = true;
            this.btnLogout.Click += new System.EventHandler(this.BtnLogout_Click);
            // 
            // btnLogin
            // 
            this.btnLogin.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLogin.Location = new System.Drawing.Point(458, 26);
            this.btnLogin.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(123, 33);
            this.btnLogin.TabIndex = 10;
            this.btnLogin.Text = "Login";
            this.btnLogin.UseVisualStyleBackColor = true;
            this.btnLogin.Click += new System.EventHandler(this.BtnLogin_Click);
            // 
            // pwText
            // 
            this.pwText.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.pwText.Location = new System.Drawing.Point(165, 70);
            this.pwText.Name = "pwText";
            this.pwText.Size = new System.Drawing.Size(236, 29);
            this.pwText.TabIndex = 5;
            // 
            // pwLabel
            // 
            this.pwLabel.AutoSize = true;
            this.pwLabel.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.pwLabel.Location = new System.Drawing.Point(46, 73);
            this.pwLabel.Name = "pwLabel";
            this.pwLabel.Size = new System.Drawing.Size(90, 22);
            this.pwLabel.TabIndex = 4;
            this.pwLabel.Text = "Password";
            // 
            // idLabel
            // 
            this.idLabel.AutoSize = true;
            this.idLabel.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.idLabel.Location = new System.Drawing.Point(46, 31);
            this.idLabel.Name = "idLabel";
            this.idLabel.Size = new System.Drawing.Size(80, 22);
            this.idLabel.TabIndex = 3;
            this.idLabel.Text = "User ID";
            // 
            // idText
            // 
            this.idText.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.idText.Location = new System.Drawing.Point(165, 28);
            this.idText.Name = "idText";
            this.idText.Size = new System.Drawing.Size(236, 29);
            this.idText.TabIndex = 2;
            this.idText.Text = "gnues";
            // 
            // RoomInfo
            // 
            this.RoomInfo.Controls.Add(this.btnChat);
            this.RoomInfo.Controls.Add(this.chat);
            this.RoomInfo.Controls.Add(this.curRoomNumberLabel);
            this.RoomInfo.Controls.Add(this.roomLabel);
            this.RoomInfo.Controls.Add(this.label1);
            this.RoomInfo.Controls.Add(this.roomTitle);
            this.RoomInfo.Controls.Add(this.title);
            this.RoomInfo.Controls.Add(this.btnCreate);
            this.RoomInfo.Controls.Add(this.titleLabel);
            this.RoomInfo.Controls.Add(this.chatLog);
            this.RoomInfo.Controls.Add(this.userList);
            this.RoomInfo.Controls.Add(this.btnLeave);
            this.RoomInfo.Controls.Add(this.btnEnter);
            this.RoomInfo.Controls.Add(this.userListLabel);
            this.RoomInfo.Controls.Add(this.roomNumberLabel);
            this.RoomInfo.Controls.Add(this.roomNumber);
            this.RoomInfo.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RoomInfo.Location = new System.Drawing.Point(16, 312);
            this.RoomInfo.Name = "RoomInfo";
            this.RoomInfo.Size = new System.Drawing.Size(623, 406);
            this.RoomInfo.TabIndex = 11;
            this.RoomInfo.TabStop = false;
            this.RoomInfo.Text = "Room";
            // 
            // btnChat
            // 
            this.btnChat.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnChat.Location = new System.Drawing.Point(542, 353);
            this.btnChat.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnChat.Name = "btnChat";
            this.btnChat.Size = new System.Drawing.Size(58, 33);
            this.btnChat.TabIndex = 22;
            this.btnChat.Text = "Chat";
            this.btnChat.UseVisualStyleBackColor = true;
            this.btnChat.Click += new System.EventHandler(this.BtnChat_Click);
            // 
            // chat
            // 
            this.chat.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chat.Location = new System.Drawing.Point(192, 355);
            this.chat.Name = "chat";
            this.chat.Size = new System.Drawing.Size(343, 29);
            this.chat.TabIndex = 21;
            this.chat.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Chat_KeyDown);
            // 
            // curRoomNumberLabel
            // 
            this.curRoomNumberLabel.AutoSize = true;
            this.curRoomNumberLabel.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.curRoomNumberLabel.Location = new System.Drawing.Point(581, 135);
            this.curRoomNumberLabel.Name = "curRoomNumberLabel";
            this.curRoomNumberLabel.Size = new System.Drawing.Size(0, 22);
            this.curRoomNumberLabel.TabIndex = 20;
            // 
            // roomLabel
            // 
            this.roomLabel.AutoSize = true;
            this.roomLabel.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.roomLabel.Location = new System.Drawing.Point(274, 135);
            this.roomLabel.Name = "roomLabel";
            this.roomLabel.Size = new System.Drawing.Size(0, 22);
            this.roomLabel.TabIndex = 19;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(14, 82);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 22);
            this.label1.TabIndex = 17;
            this.label1.Text = "Title";
            // 
            // roomTitle
            // 
            this.roomTitle.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.roomTitle.Location = new System.Drawing.Point(80, 79);
            this.roomTitle.Name = "roomTitle";
            this.roomTitle.Size = new System.Drawing.Size(343, 29);
            this.roomTitle.TabIndex = 16;
            this.roomTitle.Text = "Room Title";
            // 
            // title
            // 
            this.title.AutoSize = true;
            this.title.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.title.Location = new System.Drawing.Point(274, 147);
            this.title.Name = "title";
            this.title.Size = new System.Drawing.Size(0, 22);
            this.title.TabIndex = 15;
            // 
            // btnCreate
            // 
            this.btnCreate.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCreate.Location = new System.Drawing.Point(443, 77);
            this.btnCreate.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(99, 33);
            this.btnCreate.TabIndex = 14;
            this.btnCreate.Text = "Create";
            this.btnCreate.UseVisualStyleBackColor = true;
            this.btnCreate.Click += new System.EventHandler(this.BtnCreate_Click);
            // 
            // titleLabel
            // 
            this.titleLabel.AutoSize = true;
            this.titleLabel.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.titleLabel.Location = new System.Drawing.Point(188, 135);
            this.titleLabel.Name = "titleLabel";
            this.titleLabel.Size = new System.Drawing.Size(80, 22);
            this.titleLabel.TabIndex = 13;
            this.titleLabel.Text = "Title :";
            // 
            // chatLog
            // 
            this.chatLog.FormattingEnabled = true;
            this.chatLog.ItemHeight = 22;
            this.chatLog.Location = new System.Drawing.Point(192, 160);
            this.chatLog.Name = "chatLog";
            this.chatLog.Size = new System.Drawing.Size(406, 180);
            this.chatLog.TabIndex = 12;
            // 
            // userList
            // 
            this.userList.FormattingEnabled = true;
            this.userList.ItemHeight = 22;
            this.userList.Location = new System.Drawing.Point(18, 160);
            this.userList.Name = "userList";
            this.userList.Size = new System.Drawing.Size(158, 224);
            this.userList.TabIndex = 1;
            // 
            // btnLeave
            // 
            this.btnLeave.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLeave.Location = new System.Drawing.Point(324, 30);
            this.btnLeave.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnLeave.Name = "btnLeave";
            this.btnLeave.Size = new System.Drawing.Size(99, 33);
            this.btnLeave.TabIndex = 11;
            this.btnLeave.Text = "Leave";
            this.btnLeave.UseVisualStyleBackColor = true;
            this.btnLeave.Click += new System.EventHandler(this.BtnLeave_Click);
            // 
            // btnEnter
            // 
            this.btnEnter.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnEnter.Location = new System.Drawing.Point(211, 30);
            this.btnEnter.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.btnEnter.Name = "btnEnter";
            this.btnEnter.Size = new System.Drawing.Size(99, 33);
            this.btnEnter.TabIndex = 10;
            this.btnEnter.Text = "Enter";
            this.btnEnter.UseVisualStyleBackColor = true;
            this.btnEnter.Click += new System.EventHandler(this.BtnEnter_Click);
            // 
            // userListLabel
            // 
            this.userListLabel.AutoSize = true;
            this.userListLabel.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.userListLabel.Location = new System.Drawing.Point(14, 135);
            this.userListLabel.Name = "userListLabel";
            this.userListLabel.Size = new System.Drawing.Size(100, 22);
            this.userListLabel.TabIndex = 4;
            this.userListLabel.Text = "User List";
            // 
            // roomNumberLabel
            // 
            this.roomNumberLabel.AutoSize = true;
            this.roomNumberLabel.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.roomNumberLabel.Location = new System.Drawing.Point(14, 35);
            this.roomNumberLabel.Name = "roomNumberLabel";
            this.roomNumberLabel.Size = new System.Drawing.Size(120, 22);
            this.roomNumberLabel.TabIndex = 3;
            this.roomNumberLabel.Text = "Room Number";
            // 
            // roomNumber
            // 
            this.roomNumber.Font = new System.Drawing.Font("Consolas", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.roomNumber.Location = new System.Drawing.Point(140, 32);
            this.roomNumber.Name = "roomNumber";
            this.roomNumber.Size = new System.Drawing.Size(55, 29);
            this.roomNumber.TabIndex = 2;
            this.roomNumber.Text = "0";
            // 
            // client
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 22F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(658, 986);
            this.Controls.Add(this.RoomInfo);
            this.Controls.Add(this.LoginInfo);
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
            this.LoginInfo.ResumeLayout(false);
            this.LoginInfo.PerformLayout();
            this.RoomInfo.ResumeLayout(false);
            this.RoomInfo.PerformLayout();
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
        private System.Windows.Forms.Label currentStateText;
        private System.Windows.Forms.TextBox echoMsg;
        private System.Windows.Forms.Label labelEchoMsg;
        private System.Windows.Forms.Button btnEchoSend;
        private System.Windows.Forms.Label labelCurrentState;
        private System.Windows.Forms.GroupBox logMsgBox;
        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.ListBox logList;
        private System.Windows.Forms.GroupBox LoginInfo;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.TextBox pwText;
        private System.Windows.Forms.Label idLabel;
        private System.Windows.Forms.TextBox idText;
        private System.Windows.Forms.GroupBox RoomInfo;
        private System.Windows.Forms.Label titleLabel;
        private System.Windows.Forms.ListBox chatLog;
        private System.Windows.Forms.ListBox userList;
        private System.Windows.Forms.Button btnLeave;
        private System.Windows.Forms.Button btnEnter;
        private System.Windows.Forms.Label userListLabel;
        private System.Windows.Forms.Label roomNumberLabel;
        private System.Windows.Forms.TextBox roomNumber;
        private System.Windows.Forms.Button btnLogout;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Label title;
        private System.Windows.Forms.Label pwLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox roomTitle;
        private System.Windows.Forms.Label roomLabel;
        private System.Windows.Forms.Label curRoomNumberLabel;
        private System.Windows.Forms.Button btnChat;
        private System.Windows.Forms.TextBox chat;
    }
}

