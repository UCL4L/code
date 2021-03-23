package ʵ��11_2;

import java.net.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;

class Client extends Frame implements Runnable, ActionListener {
    Button connection;
    Socket socket = null;
    ObjectInputStream in = null;
    ObjectOutputStream out = null;
    Thread thread;

    public Client() {
        socket = new Socket();
        connection = new Button("���ӷ�����,��ȡ�ı�������");
        add(connection, BorderLayout.NORTH);
        connection.addActionListener(this);
        thread = new Thread(this);
        setBounds(100, 100, 360, 310);
        setVisible(true);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
    }

    public void run() {
        while (true) {
            try {
                TextArea text = (TextArea) in.readObject();
                add(text, BorderLayout.CENTER);
                validate();
            } catch (Exception e) {
                break;
            }
        }
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == connection) {
            try {
                if (socket.isConnected()) {
                } else {
                    InetAddress address = InetAddress.getByName("127.0.0.1");
                    InetSocketAddress socketAddress = new InetSocketAddress(address, 4331);
                    //�����˿�Ϊ 4331����ַΪaddress �� socketAddress
                    socket.connect(socketAddress);//socket ������ socketAddress �����Ӻ��С�
                    in = new ObjectInputStream(socket.getInputStream()); //socket ����������
                    out = new ObjectOutputStream(socket.getOutputStream()); //socket ���������
                    thread.start();
                }
            } catch (Exception ee) {
            }
        }
    }

    public static void main(String args[]) {
        Client win = new Client();
    }
}