package cn.test10._1;

class Consumer implements Runnable{
    private MyStack mystack = null;
    public Consumer(MyStack mystack){
        this.mystack = mystack;
    }
    //����������20����ͷ
    public void run(){
        for(int i=0;i<20;i++){
            mystack.pop();
        }
    }
}
