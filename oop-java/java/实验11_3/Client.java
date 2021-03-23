package ʵ��11_3;

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
    Button b = new Button("��ȡͼ��");
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
        byte b[] = "�뷢ͼ��".trim().getBytes();
        try {
            InetAddress address = InetAddress.getByName("127.0.0.1");
            DatagramPacket data = new DatagramPacket(b, b.length, address, 1234);
            //�������ݰ��������ݰ���Ŀ���ַ�Ͷ˿ڷ� ����address �� 1234�����е�����Ϊ���� b �е�ȫ�� �ֽڡ�
            DatagramSocket mailSend = new DatagramSocket(); //�������������ݵ� DatagramSocket ����
            mailSend.send(data);                    // mailSend �������� data��
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
            mailReceive = new DatagramSocket(5678); //�����ڶ˿� 5678 ������ȡ���ݰ��� DatagramSocket ����
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

