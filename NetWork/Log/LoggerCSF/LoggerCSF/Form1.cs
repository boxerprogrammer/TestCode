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
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;

namespace LoggerCSF
{
    public partial class Form1 : Form
    {
        List<string> messageList = new List<string>();
        // ソケット・リスナー
        private TcpListener myListener;
        // クライアント送受信
        private ClientTcpIp[] myClient = new ClientTcpIp[4];


        public Form1()
        {
            InitializeComponent();
        }
        // C#.NETのソケット通信サンプル



        // フォームロード時のソケット接続処理
        private void Form1_Load(object sender, EventArgs e)
        {
            // IPアドレス＆ポート番号設定
            int myPort = 9850;
            IPAddress myIp = Dns.Resolve("localhost").AddressList[0]; // 旧バージョン
            
            //IPAddress myIp = Dns.GetHostEntry("localhost").AddressList[0];
            myIp=myIp.MapToIPv4();
            
            IPEndPoint myEndPoint = new IPEndPoint(myIp, myPort);

            // リスナー開始
            myListener = new TcpListener(myEndPoint);
            myListener.Start();
            Console.WriteLine("Listenを開始({0}:{1})。",
           ((System.Net.IPEndPoint)myListener.LocalEndpoint).Address,
           ((System.Net.IPEndPoint)myListener.LocalEndpoint).Port);

            // クライアント接続待ち開始
            Thread myServerThread = new Thread(new ThreadStart(ServerThread));
            myServerThread.Start();
        }

        // フォームクローズ時のソケット切断処理
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            // リスナー終了
            myListener.Stop();
            // クライアント切断
            for (int i = 0; i <= myClient.GetLength(0) - 1; i++)
            {
                if (myClient[i] != null)
                {
                    if (myClient[i].objSck.Connected == true)
                    {
                        // ソケットクローズ
                        myClient[i].objStm.Close();
                        myClient[i].objSck.Close();
                    }
                }
            }
            timer1.Enabled = true;
        }

        // クライアント接続待ちスレッド
        private void ServerThread()
        {
            Debug.WriteLine("Try ServerThread");
            try
            {
                int intNo;
                while (true)
                {
                    // ソケット接続待ち
                    TcpClient myTcpClient = myListener.AcceptTcpClient();
                    Debug.WriteLine("Accepted");
                    // クライアントから接続有り
                    for (intNo = 0; intNo <= myClient.GetLength(0) - 1; intNo++)
                    {
                        if (myClient[intNo] == null)
                        {
                            break;
                        }
                        else if (myClient[intNo].objSck.Connected == false)
                        {
                            break;
                        }
                    }
                    Debug.WriteLine("Wait");
                    if (intNo < myClient.GetLength(0))
                    {
                        Debug.WriteLine("Connected");
                        // クライアント送受信オブジェクト生成
                        myClient[intNo] = new ClientTcpIp(ref messageList);
                        myClient[intNo].intNo = intNo + 1;
                        myClient[intNo].objSck = myTcpClient;
                        myClient[intNo].objStm = myTcpClient.GetStream();
                        // クライアントとの送受信開始
                        Thread myClientThread = new Thread(
                            new ThreadStart(myClient[intNo].ReadWrite));
                        myClientThread.Start();
                    }
                    else
                    {
                        Debug.WriteLine("Rejected");
                        // 接続拒否
                        myTcpClient.Close();
                    }
                }
            }
            catch { }
        }

        // クライアント送受信クラス
        public class ClientTcpIp
        {
            public int intNo;
            public TcpClient objSck;
            public NetworkStream objStm;
            List<string> msgList;
            public ClientTcpIp(ref List<string> list)
            {
                msgList = list;
            }
            // クライアント送受信スレッド
            public void ReadWrite()
            {
                try
                {
                    while (true)
                    {
                        // ソケット受信
                        Byte[] rdat = new Byte[1024];
                        int ldat = objStm.Read(rdat, 0, rdat.GetLength(0));
                        if (ldat > 0)
                        {
                            // クライアントからの受信データ有り
                            // 送信データ作成
                            Byte[] sdat = new Byte[ldat];
                            Array.Copy(rdat, sdat, ldat);
                            //sdat=System.Text.Encoding.Convert(Encoding.GetEncoding("Shift-JIS"),Encoding.Unicode,sdat);
                            String msg = Encoding.GetEncoding("Shift-JIS").GetString(sdat);
                            //sdat = System.Text.Encoding.GetEncoding(
                            //    "SHIFT-JIS").GetBytes(msg);
                            // ソケット送信
                            //objStm.Write(sdat, 0, sdat.GetLength(0));

                            if (msg.Substring(0, 3) == "fps")
                            {
                                lock (msgList)
                                {
                                    msgList.Add(msg);
                                }
                            }
                            Debug.WriteLine(msg);
                        }
                        else
                        {
                            // ソケット切断有り
                            // ソケットクローズ
                            objStm.Close();
                            objSck.Close();
                            return;
                        }
                    }
                }
                catch { }
            }
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            lock (messageList)
            {
               
                foreach (var m in messageList)
                {
                    listLog.Items.Add(m);
                }
                messageList.Clear();
            }
            ////Readが0を返した時はクライアントが切断したと判断
            //if (resSize == 0)
            //{
            //    disconnected = true;
            //    Console.WriteLine("クライアントが切断しました。");
            //    return;
            //}
            ////受信したデータを蓄積する
            //ms.Write(resBytes, 0, resSize);
            ////まだ読み取れるデータがあるか、データの最後が\nでない時は、
            //// 受信を続ける
        }

        private void button1_Click(object sender, EventArgs e)
        {
            listLog.Items.Clear();
            foreach (var m in messageList)
            {
                listLog.Items.Add(m);
            }
        }
    }
}
