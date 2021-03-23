package 实验11_2;

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
        connection = new Button("连接服务器,读取文本区对象");
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
                    //创建端口为 4331、地址为address 的 socketAddress
                    socket.connect(socketAddress);//socket 建立和 socketAddress 的连接呼叫。
                    in = new ObjectInputStream(socket.getInputStream()); //socket 返回输入流
                    out = new ObjectOutputStream(socket.getOutputStream()); //socket 返回输出流
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