package 实验11_3;
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
                mailReceive = new DatagramSocket(1234);//创建在端口 1234 负责收取数据包的DatagramSocket 对象。
            } catch (IOException e1) {
                System.out.println("正在等待");
            }
            try {
                mailReceive.receive(pack);
                address = pack.getAddress(); //pack 返回 InetAddress 对象。
                System.out.println("客户的地址:" + address);
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
                DatagramPacket data = new DatagramPacket(b, n, address, 5678); //创建数据包，目标地址和端口分别是address 和 5678，其中的数据为数组 b 中的前 n 个字节
                DatagramSocket mailSend = new DatagramSocket();//创建发送数据的 DatagramSocket 对象
                mailSend.send(data);                           // mailSend 发送数据 data
            }
            in.close();
            byte end[] = "end".getBytes();
            DatagramPacket data = new DatagramPacket(end, end.length, address, 5678); //创建数据包，目标地址和端口分别是address 和 5678，其中的数据为数组 end 中的全部字节
            DatagramSocket mailSend = new DatagramSocket();//创建负责发送数据的DatagramSocket 对 象
            mailSend.send(data);                        // mailSend 发送数据 data
        } catch (Exception e) {
        }
    }
}

