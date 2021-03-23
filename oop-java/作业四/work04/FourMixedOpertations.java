package work04;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;


public class FourMixedOpertations {
    //利用一个队列，二个栈实现四则运算
    //一个队列存放所有运算公式元素
    //一个栈存放数字
    //一个栈存放运算符
    static Map<String, Integer> map = new HashMap<String, Integer>();
    //数字栈
    static MyStack numStack = new MyStack();
    //运算符栈
    static MyStack symbolStack = new MyStack();

    public static void main(String[] args) {
        //初始化运算符和优先级
        map.put("(", 0);
        map.put(")", 0);
        map.put("+", 1);
        map.put("-", 1);
        map.put("*", 2);
        map.put("/", 2);

        Scanner scanner = new Scanner(System.in);
        //获取用户输入的公式
        String value = scanner.nextLine();
        //将运算公式转换成字符数组入队列
        MyQueue queue = enQueue(value);
        //队列每个元素出队列
        while (queue.getSize() > 0) {
            String con = queue.dequeue().toString();
            //如果是数字就入数字栈
            if (!map.containsKey(con)) {
                numStack.push(Integer.parseInt(con));
                continue;
            }
            //如果是符号且符号栈中没有内容，符号直接进符号栈
            if(symbolStack.getSize()==0){
                symbolStack.push(con);
                continue;
            }
            //如果左括号，直接入栈
            if(con.equals("(")){
                symbolStack.push(con);
                continue;
            }
            //如果右括号，循环取出符号栈中的符号，跟数字栈中两个数字参与运算后入数字栈，直到碰到左括号。
            if(con.equals(")")){
                while(!symbolStack.peer().equals("(")){
                    ops(numStack, symbolStack);
                }
                symbolStack.pop();
                continue;
            }
            //否则进入运算符栈，
            // 如果运算符的优先级小于运算栈栈顶元素，栈顶元素出栈，跟数字栈的两个元素运算，否则运算符入栈
            while (symbolStack.getSize() > 0 && getLevel((String) symbolStack.peer(), con)) {
                ops(numStack, symbolStack);
            }
            //当前运算符大于栈顶的运算符，可以放入符号栈
            symbolStack.push(con);
        }

        //如果队列为空后，运算符还有内容，就依次出栈，跟数字栈参与运算
        while (symbolStack.getSize() > 0) {
            ops(numStack, symbolStack);
        }

        //最后结果
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

