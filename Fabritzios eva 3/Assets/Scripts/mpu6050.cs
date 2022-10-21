using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;
using System;

public class mpu6050 : MonoBehaviour
{
    
    SerialPort serialPort = new SerialPort("/dev/ttyUSB0", 115200);
    byte[] buffer = new byte[16];
    private float timer;
    private float intervalo = 0.06f;
    //public string[] strData = new string[4];
    //public string[] strData_received = new string[4];
    public float qw, qx, qy, qz;
    void Start()
    {
        serialPort.NewLine = "\n";
        serialPort.DtrEnable = true;
        serialPort.Open(); //Open the Serial Stream.
    }

    // Update is called once per frame
    void Update()
    {
    
        if (serialPort.BytesToRead >= 16)
        {
            serialPort.Read(buffer,0,16);
            qw = BitConverter.ToSingle(buffer, 0);
            qx = BitConverter.ToSingle(buffer, 4);
            qy = BitConverter.ToSingle(buffer, 8);
            qz = BitConverter.ToSingle(buffer, 12);
            
            transform.rotation = new Quaternion(-qy, -qz, qx, qw);
            
            
        }
        
        timer += Time.deltaTime;

        if (timer > intervalo)
        {
            timer = timer - intervalo;
            serialPort.Write("s\n");
        }  

        /*strReceived = stream.ReadLine(); //Read the information  

        strData = strReceived.Split(',');*/

        /*if (strData[0] != "" && strData[1] != "" && strData[2] != "" && strData[3] != "")//make sure data are reday
        {
             
            strData_received[0] = strData[0];
            strData_received[1] = strData[1];
            strData_received[2] = strData[2];
            strData_received[3] = strData[3];
         
            qw = float.Parse(strData_received[0]);
            qx = float.Parse(strData_received[1]);
            qy = float.Parse(strData_received[2]);
            qz = float.Parse(strData_received[3]);
      
            transform.rotation = new Quaternion(-qy, -qz, qx, qw);
    
        }      */
    }
}
