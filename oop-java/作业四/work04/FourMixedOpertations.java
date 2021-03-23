package work04;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;


public class FourMixedOpertations {
    //����һ�����У�����ջʵ����������
    //һ�����д���������㹫ʽԪ��
    //һ��ջ�������
    //һ��ջ��������
    static Map<String, Integer> map = new HashMap<String, Integer>();
    //����ջ
    static MyStack numStack = new MyStack();
    //�����ջ
    static MyStack symbolStack = new MyStack();

    public static void main(String[] args) {
        //��ʼ������������ȼ�
        map.put("(", 0);
        map.put(")", 0);
        map.put("+", 1);
        map.put("-", 1);
        map.put("*", 2);
        map.put("/", 2);

        Scanner scanner = new Scanner(System.in);
        //��ȡ�û�����Ĺ�ʽ
        String value = scanner.nextLine();
        //�����㹫ʽת�����ַ����������
        MyQueue queue = enQueue(value);
        //����ÿ��Ԫ�س�����
        while (queue.getSize() > 0) {
            String con = queue.dequeue().toString();
            //��������־�������ջ
            if (!map.containsKey(con)) {
                numStack.push(Integer.parseInt(con));
                continue;
            }
            //����Ƿ����ҷ���ջ��û�����ݣ�����ֱ�ӽ�����ջ
            if(symbolStack.getSize()==0){
                symbolStack.push(con);
                continue;
            }
            //��������ţ�ֱ����ջ
            if(con.equals("(")){
                symbolStack.push(con);
                continue;
            }
            //��������ţ�ѭ��ȡ������ջ�еķ��ţ�������ջ���������ֲ��������������ջ��ֱ�����������š�
            if(con.equals(")")){
                while(!symbolStack.peer().equals("(")){
                    ops(numStack, symbolStack);
                }
                symbolStack.pop();
                continue;
            }
            //������������ջ��
            // �������������ȼ�С������ջջ��Ԫ�أ�ջ��Ԫ�س�ջ��������ջ������Ԫ�����㣬�����������ջ
            while (symbolStack.getSize() > 0 && getLevel((String) symbolStack.peer(), con)) {
                ops(numStack, symbolStack);
            }
            //��ǰ���������ջ��������������Է������ջ
            symbolStack.push(con);
        }

        //�������Ϊ�պ�������������ݣ������γ�ջ��������ջ��������
        while (symbolStack.getSize() > 0) {
            ops(numStack, symbolStack);
        }

        //�����
        System.out.println(numStack.pop());
    }

    private static MyQueue enQueue(String value) {
        MyQueue queue = new MyQueue();
        for(char c :value.toCharArray()){
            queue.enqueue(c);
        }
        return queue;
    }

    private static boolean getLevel(String con1, String con2) {
        return map.get(con1) > map.get(con2);

    }

    private static void ops(MyStack numStack, MyStack symbolStack) {
        int num1 = (Integer) numStack.pop();
        int num2 = (Integer) numStack.pop();
        String symbol = (String) symbolStack.pop();
        int r = operator(symbol, num1, num2);
        numStack.push(r);
    }

    private static int operator(String symbol, int num1, int num2) {
        switch (symbol.charAt(0)) {
            case '+':
                return num2 + num1;
            case '-':
                return num2 - num1;
            case '*':
                return num2 * num1;
            case '/':
                return num2 / num1;
            default:
                return 0;
        }
    }
}

