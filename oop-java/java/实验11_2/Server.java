package 实验11_2;

import java.io.*;
import java.net.*;

public class Server {
    public static void main(String args[]) {
        ServerSocket server = null;
        ServerThread thread;
        Socket you = null;
        while (true) {
            try {
                server = new ServerSocket(4331);//创建在端口 4331 上负责监听的 ServerSocket 对象
            } catch (IOException e1) {
                System.out.println("正在监听");
            }
            try {
                you = server.accept(); // server 返回和客户端相连接的 Socket 对象
                System.out.println("客户的地址:" + you.getInetAddress());
            } catch (IOException e) {
                System.out.println("正在等待客户");
            }
            if (you != null) {
                new ServerThread(you).start();
            } else {
                continue;
            }
        }
    }
}