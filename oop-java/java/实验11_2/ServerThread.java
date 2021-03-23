package 实验11_2;
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
            out = new ObjectOutputStream(socket.getOutputStream()); //socket 返回输出流。
            in = new ObjectInputStream(socket.getInputStream()); //socket 返回输入流。
        } catch (IOException e) {
        }
    }

    public void run() {
        TextArea text = new TextArea("你好,我是服务器", 12, 12);
        try {
            out.writeObject(text);
        } catch (IOException e) {
            System.out.println("客户离开");
        }
    }
}

