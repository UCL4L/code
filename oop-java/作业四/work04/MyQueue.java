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
    //��������ʵ�ֶ���
    //���д�С
    int size = 0;
    public int getSize() {
        return size;
    }
    //ͷ�ڵ�
    Node head = null;
    //β�ڵ�
    Node tail = null;
    //�����
    public void enqueue(Object o){
//        ��β���
        //�����½ڵ�
        Node newNode = new Node(o);
        if(getSize() == 0){
        //�������Ϊ�գ�ͷβ�ڵ㶼ָ���½ڵ�
            head = tail = newNode;
        }else{
        //����β�ڵ���һ�ڵ�ָ���½ڵ㣬β�ڵ�ָ���½ڵ�
            tail.setNext(newNode);
            tail = newNode;
        }
        //������һ
        size++;
    }
    //������
    public Object dequeue(){
        //��ͷ����
        Object value = head.getValue();
        head = head.getNext();
        //������һ
        size--;
        return value;
    }
    //�鿴����ͷԪ��
    public Object peek(){
        return head.getValue();
    }
}


