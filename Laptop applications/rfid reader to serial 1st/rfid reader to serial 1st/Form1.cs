using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace rfid_reader_to_serial_1st
{
    public partial class Form1 : Form
    {
        String textInPort = "";
        String dataSendToServer = "";
        String responseFromServer = "";
        public Form1()
        {
            try
            {
                InitializeComponent();
                serialPort1.Open();
            }
            catch (Exception errors)
            {
                MessageBox.Show("Arduino not connected to the port");
            }
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

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            serialPort1.Close();
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            textInPort = serialPort1.ReadLine();
            textInPort.Trim();
            textInPort = textInPort.Replace("\r\n", "").Replace("\r", "").Replace("\n", "");
            textInPort = textInPort.Substring(1);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            lbUID.Text = "Current chip ID: " + textInPort;
        }

        private void btnRegister_Click(object sender, EventArgs e)
        {
            try
            {
                if (textInPort != "")
                {
                    if (cbRoom.SelectedItem.ToString() == "Master")
                    {
                        textInPort.Trim();
                                                                    //SEND TO ALL ESPs
                        dataSendToServer = "*" + tbName.Text + " " + tbAge.Text + " " + textInPort;
                        //MessageBox.Show("*" + textInPort + "*");
                                                                    //depending on which room is selected, send to different ESPs
                        
                        POSTrequest("http://192.168.43.114/add", textInPort);                      
                        dataSendToServer = "";
                        textInPort = "";
                    }
                    else if (cbRoom.SelectedItem.ToString() == "Room 1")
                    {
                        textInPort.Trim();
                                                                    //SEND TO ESP with room 1
                        dataSendToServer = "*" + tbName.Text + " " + tbAge.Text + " " + textInPort;

                                                                    //depending on which room is selected, send to different ESPs

                        POSTrequest("http://192.168.43.114/add", textInPort); //ESP 1                       
                        dataSendToServer = "";
                        textInPort = "";
                    }
                }
                else
                {
                    MessageBox.Show("RFID tag not read properly. Please put the tag on the reader again !");
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Please fill all fields before attempting to register new user !");
            }
        }
    }
}
