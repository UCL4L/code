package ʵ��11_2;

import java.io.*;
import java.net.*;

public class Server {
    public static void main(String args[]) {
        ServerSocket server = null;
        ServerThread thread;
        Socket you = null;
        while (true) {
            try {
                server = new ServerSocket(4331);//�����ڶ˿� 4331 �ϸ�������� ServerSocket ����
            } catch (IOException e1) {
                System.out.println("���ڼ���");
            }
            try {
                you = server.accept(); // server ���غͿͻ��������ӵ� Socket ����
                System.out.println("�ͻ��ĵ�ַ:" + you.getInetAddress());
            } catch (IOException e) {
                System.out.println("���ڵȴ��ͻ�");
            }
            if (you != null) {
                new ServerThread(you).start();
            } else {
                continue;
            }
        }
    }
}