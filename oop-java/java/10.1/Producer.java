package cn.test10._1;

class Producer implements Runnable{
    private MyStack mystack = null;
    public Producer(MyStack mystack){
        this.mystack = mystack;
    }
    //����������20����ͷ
    public void run(){
        for(int i=1;i<20;i++){
            Bread bread = new Bread(i);
            mystack.push(bread);
        }
    }
}
