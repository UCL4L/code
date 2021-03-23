package cn.test10._1;
public class Tester {
    public static void main(String[] args){
        MyStack stack = new MyStack();
        Producer producer = new Producer(stack);
        Consumer consumer = new Consumer(stack);
        new Thread(producer).start();
        new Thread(consumer).start();
    }
}
