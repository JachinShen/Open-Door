package com.example.jachinshen.myapplication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    Button btnBUTH;
    TextView tv;
    private final static int REQUEST_ENABLE_BT = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnBUTH = (Button)findViewById(R.id.btnBUTH);
        tv = (TextView) findViewById(R.id.tv);
        btnBUTH.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
                if (mBluetoothAdapter == null) {
                    // Device does not support Bluetooth
                    tv.setText("No BLUETOOTH");
                }else{
                    if (!mBluetoothAdapter.isEnabled()) {
                        Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
                    }else{
                        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
// If there are paired devices
                        if (pairedDevices.size() > 0) {
                            // Loop through paired devices
                            new Thread() {

                                BluetoothDevice bluetoothdevice = mBluetoothAdapter.getRemoteDevice("98:D3:32:30:C1:20");
                                final String SPP_UUID = "00001101-0000-1000-8000-00805F9B34FB";
                                UUID uuid = UUID.fromString(SPP_UUID);
                                BluetoothSocket socket;
                                InputStream inputStream = null;
                                OutputStream outputStream = null;

                                {
                                    try {
                                        socket = bluetoothdevice.createInsecureRfcommSocketToServiceRecord(uuid);
                                        socket.connect();
                                        tv.setText("连接成功");
                                        inputStream = socket.getInputStream();
                                        outputStream = socket.getOutputStream();
                                        outputStream.write('O');
                                        if (inputStream.read() == 'Y') {
                                            tv.setText("开门成功");
                                        }
                                        outputStream.flush();
                                        outputStream.close();
                                        inputStream.close();
                                        socket.close();
                                    } catch (IOException e) {
                                        // tv.setText("连接失败");

                                    }


                                }
                            }.start();
                        }
                    }
                }
            }
        });
    }


}
