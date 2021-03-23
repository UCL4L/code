package 实验11_1;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;

class NetWin extends Frame implements ActionListener, Runnable {
    Button button;
    URL url;
    TextField text;
    TextArea area;
    byte b[] = new byte[118];
    Thread thread;

    NetWin() {
        text = new TextField(20);
        area = new TextArea(12, 12);
        button = new Button("Enter");
        button.addActionListener(this);
        thread = new Thread(this);
        Panel p = new Panel();
        p.add(new Label("put URL:"));
        p.add(text);
        p.add(button);
        add(area, BorderLayout.CENTER);
        add(p, BorderLayout.NORTH);
        setBounds(60, 60, 360, 300);
        setVisible(true);
        validate();
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
    }

    public void actionPerformed(ActionEvent e) {
        if (!(thread.isAlive())) thread = new Thread(this);
        try {
            thread.start();
        } catch (Exception ee) {
            text.setText("Reading..." + url);
        }
    }

    public void run() {
        try {
            int n = -1;
            area.setText(null);
            String name = text.getText().trim();
            url = new URL(name); //使用字符串 name 创建 url 对象
            String hostName = url.getHost();//url 调用 getHost()
            int urlPortNumber = url.getPort();//url 调用 getPort()
            String fileName = url.getFile();//url 调用 getFile()
            InputStream in = url.openStream();//url 调用方法返回一个输入流
            area.append("\n 主机:" + hostName + "端口:" + urlPortNumber + "包含的文件名字:" + fileName);
            area.append("\n 文件的内容如下:");
            while ((n = in.read(b)) != -1) {
                String s = new String(b, 0, n);
                area.append(s);
            }
        } catch (MalformedURLException e1) {
            text.setText("" + e1);
            return;
        } catch (IOException e1) {
            text.setText("" + e1);
            return;
        }
    }
}
