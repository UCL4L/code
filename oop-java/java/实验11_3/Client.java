package 实验11_3;

import java.net.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;

class ImageCanvas extends Canvas {
    Image image = null;

    public ImageCanvas() {
        setSize(200, 200);
    }

    public void paint(Graphics g) {
        if (image != null) g.drawImage(image, 0, 0, this);
    }

    public void setImage(Image image) {
        this.image = image;
    }
}

class Client extends Frame implements Runnable, ActionListener {
    Button b = new Button("获取图像");
    ImageCanvas canvas;

    Client() {
        super("I am a client");
        setSize(320, 200);
        setVisible(true);
        b.addActionListener(this);
        add(b, BorderLayout.NORTH);
        canvas = new ImageCanvas();
        add(canvas, BorderLayout.CENTER);
        Thread thread = new Thread(this);
        validate();
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
        thread.start();
    }

    public void actionPerformed(ActionEvent event) {
        byte b[] = "请发图像".trim().getBytes();
        try {
            InetAddress address = InetAddress.getByName("127.0.0.1");
            DatagramPacket data = new DatagramPacket(b, b.length, address, 1234);
            //创建数据包，该数据包的目标地址和端口分 别是address 和 1234，其中的数据为数组 b 中的全部 字节。
            DatagramSocket mailSend = new DatagramSocket(); //创建负责发送数据的 DatagramSocket 对象。
            mailSend.send(data);                    // mailSend 发送数据 data。
        } catch (Exception e) {
        }
    }

    public void run() {
        DatagramPacket pack = null;
        DatagramSocket mailReceive = null;
        byte b[] = new byte[8192];
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        try {
            pack = new DatagramPacket(b, b.length);
            mailReceive = new DatagramSocket(5678); //创建在端口 5678 负责收取数据包的 DatagramSocket 对象。
        } catch (Exception e) {
        }
        try {
            while (true) {
                mailReceive.receive(pack);
                String message = new String(pack.getData(), 0, pack.getLength());
                if (message.startsWith("end")) {
                    break;
                }
                out.write(pack.getData(), 0, pack.getLength());
            }
            byte imagebyte[] = out.toByteArray();
            out.close();
            Toolkit tool = getToolkit();
            Image image = tool.createImage(imagebyte);
            canvas.setImage(image);
            canvas.repaint();
            validate();
        } catch (IOException e) {
        }
    }

    public static void main(String args[]) {
        new Client();
    }
}

