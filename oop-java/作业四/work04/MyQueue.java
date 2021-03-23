package work04;


public class MyQueue {

    public static void main(String[] args) {
        MyQueue queue = new MyQueue();
        queue.enqueue(2);
        queue.enqueue(3);
        queue.enqueue(5);
        queue.enqueue(6);
        queue.enqueue(7);
        while(queue.getSize() > 0 ){
            System.out.println(queue.dequeue());
        }
    }
    //利用链表实现队列
    //队列大小
    int size = 0;
    public int getSize() {
        return size;
    }
    //头节点
    Node head = null;
    //尾节点
    Node tail = null;
    //入队列
    public void enqueue(Object o){
//        队尾入队
        //创建新节点
        Node newNode = new Node(o);
        if(getSize() == 0){
        //如果队列为空，头尾节点都指向新节点
            head = tail = newNode;
        }else{
        //否则尾节点下一节点指向新节点，尾节点指向新节点
            tail.setNext(newNode);
            tail = newNode;
        }
        //数量加一
        size++;
    }
    //出队列
    public Object dequeue(){
        //队头出队
        Object value = head.getValue();
        head = head.getNext();
        //数量减一
        size--;
        return value;
    }
    //查看队列头元素
    public Object peek(){
        return head.getValue();
    }
}


