package work04;


public class MyStack {

    public static void main(String[] args) {
        MyStack stack = new MyStack();
        stack.push(2);
        stack.push(5);
        stack.push(7);
        stack.push(3);
        while (stack.getSize() > 0) {
            System.out.println(stack.pop());
        }
    }

    //��������ʵ��ջ
    //ջ������
    int size = 0;
    //ջ���ڵ�
    Node head = null;

    public int getSize() {
        return size;
    }

    //��ջ
    public void push(Object o) {
        Node newNode = new Node(o);
//        ���ջ��û��Ԫ�أ�ֱ����ջ
        if (head == null) {
            head = newNode;
        } else {
            //        ���ջ����Ԫ�أ���Ҫ���½ڵ�ָ��ͷ�ڵ㣬����ͷ�ڵ�ָ���½ڵ�
            newNode.setNext(head);
            head = newNode;
        }

        size++;
    }

    //��ջ
    public Object pop() {
        Node tmp = head;
        head = head.getNext();
        size--;
        return tmp.getValue();
    }

    //�鿴��ջ��Ԫ��
    public Object peer() {
        return head.getValue();
    }
}
