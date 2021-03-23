package ʵ��11_3;
import java.net.*;
import java.io.*;

public class Server {
    public static void main(String args[]) {
        DatagramPacket pack = null;
        DatagramSocket mailReceive = null;
        ServerThread thread;
        byte b[] = new byte[8192];
        InetAddress address = null;
        pack = new DatagramPacket(b, b.length);
        while (true) {
            try {
                mailReceive = new DatagramSocket(1234);//�����ڶ˿� 1234 ������ȡ���ݰ���DatagramSocket ����
            } catch (IOException e1) {
                System.out.println("���ڵȴ�");
            }
            try {
                mailReceive.receive(pack);
                address = pack.getAddress(); //pack ���� InetAddress ����
                System.out.println("�ͻ��ĵ�ַ:" + address);
            } catch (IOException e) {
            }
            if (address != null) {
                new ServerThread(address).start();
            } else {
                continue;
            }
        }
    }
}

class ServerThread extends Thread {
    InetAddress address;
    DataOutputStream out = null;
    DataInputStream in = null;
    String s = null;

    ServerThread(InetAddress address) {
        this.address = address;
    }

    public void run() {
        FileInputStream in;
        byte b[] = new byte[8192];
        try {
            in = new FileInputStream("a.jpg");
            int n = -1;
            while ((n = in.read(b)) != -1) {
                DatagramPacket data = new DatagramPacket(b, n, address, 5678); //�������ݰ���Ŀ���ַ�Ͷ˿ڷֱ���address �� 5678�����е�����Ϊ���� b �е�ǰ n ���ֽ�
                DatagramSocket mailSend = new DatagramSocket();//�����������ݵ� DatagramSocket ����
                mailSend.send(data);                           // mailSend �������� data
            }
            in.close();
            byte end[] = "end".getBytes();
            DatagramPacket data = new DatagramPacket(end, end.length, address, 5678); //�������ݰ���Ŀ���ַ�Ͷ˿ڷֱ���address �� 5678�����е�����Ϊ���� end �е�ȫ���ֽ�
            DatagramSocket mailSend = new DatagramSocket();//�������������ݵ�DatagramSocket �� ��
            mailSend.send(data);                        // mailSend �������� data
        } catch (Exception e) {
        }
    }
}

