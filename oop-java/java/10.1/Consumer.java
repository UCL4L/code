package cn.test10._1;

class Consumer implements Runnable{
    private MyStack mystack = null;
    public Consumer(MyStack mystack){
        this.mystack = mystack;
    }
    //消费者消费20个馒头
    public void run(){
        for(int i=0;i<20;i++){
            mystack.pop();
        }
    }
}
