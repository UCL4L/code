package cn.test10._1;

class MyStack{
    //将馒头放到数组中
    private Bread[] arr = new Bread[10];
    //当前所放馒头的一个指针表示当前的位置
    private int index = 0;

    //生产馒头
    public synchronized void push(Bread bread){
        //当生产的馒头超出生产工具时，让该线程释放锁处于等待的一个状态
        if(index == arr.length){
            try{
                this.wait();
            }catch(Exception e){
                System.out.println("超出生产范围");
            }
        }
        //唤醒正在等待的线程
        this.notify();
        //将生产的馒头放到数组中
        arr[index] = bread;
        //将当前的指针移动到下一个位置
        index++;

        try{
            Thread.sleep(1000);
            System.out.println("生产了"+bread);
        }catch(Exception e){
            System.out.println("中断异常");
        }
    }

    //消费馒头
    public synchronized Bread pop(){
        //当数组中没有面包时，让该线程处于等待状态，并且释放锁
        if(index==0){
            try{
                this.wait();
            }catch(Exception e){
                System.out.println("超出生产范围");
            }
        }
        //唤醒真在等待的线程
        this.notify();
        //将当前指针的位置移动到上一个
        index--;
        try{
            Thread.sleep(1000);
            System.out.println("消费了"+arr[index]);
        }catch(Exception e){
            System.out.println("中断异常");
        }
        //显示当前的馒头
        return arr[index];
    }
}
