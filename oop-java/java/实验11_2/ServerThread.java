package ʵ��11_2;
import java.awt.*;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

class ServerThread extends Thread {
    Socket socket;
    ObjectInputStream in = null;
    ObjectOutputStream out = null;
    String s = null;

    ServerThread(Socket t) {
        socket = t;
        try {
            out = new ObjectOutputStream(socket.getOutputStream()); //socket �����������
            in = new ObjectInputStream(socket.getInputStream()); //socket ������������
        } catch (IOException e) {
        }
    }

    public void run() {
        TextArea text = new TextArea("���,���Ƿ�����", 12, 12);
        try {
            out.writeObject(text);
        } catch (IOException e) {
            System.out.println("�ͻ��뿪");
        }
    }
}

