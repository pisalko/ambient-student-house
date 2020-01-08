using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Windows.Forms;
using System.Linq;
using System.Threading;

namespace Laptop_application_final
{
    public partial class Form1 : Form
    {
        const String IPSERVER = "http://192.168.43.213:42069/";

        String textInPort = "";
        String responseFromServer = "";
        String dataFromGET = "";
        String oldDataFromGET = "";
        String roomReceived = "";
        String ipReceived = "";
        String selectedItemText = "";
        Dictionary<string, string> RoomsAndIPs = new Dictionary<string, string>();
        bool once = true;
        int indexForComma;

        public Form1()
        {
            InitializeComponent();
            try
            {
                lvAvailableRooms.Items.Add("Master key");
                serialPort1.Open();
            }
            catch (Exception errors)
            {
                MessageBox.Show("Arduino not connected to the port");
            }
        }

        public string GETrequest(string uri)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uri);
            request.AutomaticDecompression = DecompressionMethods.GZip | DecompressionMethods.Deflate;

            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream stream = response.GetResponseStream();
            StreamReader reader = new StreamReader(stream);

            return reader.ReadToEnd();
        }

        private void POSTrequest(string uri, String order)
        {
            try
            {
                //First we create a Webrequest type variable with an absolute uri (htttp://ipaddress:port/)
                WebRequest request = WebRequest.Create(uri);
                //We specify that it will be a POST request, the WebRequest class enables us to do it that easily
                request.Method = "POST";
                //Order is a String which we convert to byte array and later upload to Stream.
                byte[] buffer = Encoding.ASCII.GetBytes(order);
                //We specify what is sending the request, again WebRequest class makes it easy for us
                request.ContentType = "text/plain";
                //Here we tell the server how long our byte array is (string we are sending) (constructing the header)
                request.ContentLength = buffer.Length;
                //We create a Stream type variable and store the requestStream in it
                Stream dataStream = request.GetRequestStream();
                //Uploading the byte array to the requested stream
                dataStream.Write(buffer, 0, buffer.Length);
                //Stream is closed since we dont need it anymore
                dataStream.Close();
                //Here we wait for the server's response (if not received, we get an exception)         
                WebResponse response = request.GetResponse();
                //We print the status in console just for clarification
                Console.WriteLine(((HttpWebResponse)response).StatusDescription);
                //We get the data from the response stream
                dataStream = response.GetResponseStream();
                //We use the StreamReader class to read the incoming Stream at ease
                StreamReader reader = new StreamReader(dataStream);
                //We save the stream's content data in a String
                responseFromServer = reader.ReadToEnd();
                //We display the data in the Console for debugging, this helped me a lot when I was building the server on Java with Sockets (I left that code for clarification, we don't use it)
                Console.WriteLine(responseFromServer);
                //Here we send the POST request  again if the ESP does not parse it correctly
                if (responseFromServer == "ne")
                    POSTrequest(uri, order);
                //Closing the streams
                reader.Close();
                dataStream.Close();
                response.Close();
            }
            catch (Exception errors)
            {
                MessageBox.Show("Client did not receive a response from server, please check connection with server, there is something wrong ! \n Possible errors: wrong IP, wrong Port, server not launched");
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            lbUID.Text = "Current chip ID: " + textInPort;


            if (once)
            {
                dataFromGET = GETrequest(IPSERVER);
                oldDataFromGET = dataFromGET;
                once = false;
            }

            dataFromGET = GETrequest(IPSERVER);
            bool checkIfNewInfo = dataFromGET != oldDataFromGET;

            if (checkIfNewInfo)
            {
                oldDataFromGET = dataFromGET;

                for (int i = 0; i < dataFromGET.Length; i++)
                {
                    if (dataFromGET[i] == ',')
                    {
                        indexForComma = i;
                        break;
                    }
                }

                roomReceived = dataFromGET.Substring(0, indexForComma);
                ipReceived = dataFromGET.Substring(indexForComma + 1);

                lvAvailableRooms.Items.Add(roomReceived);
                try
                {
                    RoomsAndIPs.Add(roomReceived, ipReceived);
                }
                catch(Exception errors)
                {

                }
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            textInPort = serialPort1.ReadLine();
            textInPort.Trim();
            textInPort = textInPort.Replace("\r\n", "").Replace("\r", "").Replace("\n", "");
            textInPort = textInPort.Substring(1);
        }

        private void btnRegister_Click(object sender, EventArgs e)
        {
            //try
            //{
                if (textInPort != "")
                {
                    textInPort.Trim();

                    String ipUsed = "";

                //MessageBox.Show(lvAvailableRooms.SelectedItems[0].Text);

                    if (RoomsAndIPs.ContainsKey(lvAvailableRooms.SelectedItems[0].Text) || lvAvailableRooms.SelectedItems[0].Text == "Master key")
                    {
                        RoomsAndIPs.TryGetValue(lvAvailableRooms.SelectedItems[0].Text, out ipUsed);
                        //MessageBox.Show(ipUsed);
                    }
                    else
                    {
                        MessageBox.Show("Please select a Room !");
                        return;
                    }

                    if (lvAvailableRooms.SelectedItems[0].Text == "Master key" && RoomsAndIPs.Count != 0)
                    {
                        //SEND TO ALL ESPs        
                        for (int i = 0; i < RoomsAndIPs.Values.Count; i++)
                        {
                        //MessageBox.Show(RoomsAndIPs.Values.ToList()[i].ToString());
                            POSTrequest("http://" + RoomsAndIPs.Values.ToList()[i].ToString() + "/add", textInPort);
                            //Thread.Sleep(20);
                        }
                        textInPort = "";
                    }
                    else if (RoomsAndIPs.Count != 0)
                    {
                        POSTrequest("http://" + ipUsed + "/add", textInPort);
                        textInPort = "";
                    }
                    else
                    {
                        MessageBox.Show("There are no rooms in the system yet.");
                    }
                }
                else
                {
                    MessageBox.Show("RFID tag not read properly. Please put the tag on the reader again !");
                }
           /* }
            catch (Exception)
            {
                MessageBox.Show("Please fill all fields before attempting to register new user !");
            }*/
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            serialPort1.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //RoomsAndIPs.Add("Room 1", "312.321.32.21");
            //MessageBox.Show("http://" + RoomsAndIPs.Values.ToList()[0].ToString() + "/add", textInPort);
        }

        private void lvAvailableRooms_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvAvailableRooms.SelectedIndices.Count <= 0)
            {
                return;
            }
            int intselectedindex = lvAvailableRooms.SelectedIndices[0];
            if (intselectedindex >= 0)
            {
                selectedItemText = lvAvailableRooms.Items[intselectedindex].Text;

                //do something
                MessageBox.Show(lvAvailableRooms.Items[intselectedindex].Text); 
            }
        }
    }
}
