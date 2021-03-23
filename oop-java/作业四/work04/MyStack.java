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

    //利用链表实现栈
    //栈的数量
    int size = 0;
    //栈顶节点
    Node head = null;

    public int getSize() {
        return size;
    }

    //入栈
    public void push(Object o) {
        Node newNode = new Node(o);
//        如果栈中没有元素，直接入栈
        if (head == null) {
            head = newNode;
        } else {
            //        如果栈中有元素，需要将新节点指向头节点，并将头节点指向新节点
            newNode.setNext(head);
            head = newNode;
        }

        size++;
    }

    //出栈
    public Object pop() {
        Node tmp = head;
        head = head.getNext();
        size--;
        return tmp.getValue();
    }

    //查看，栈顶元素
    public Object peer() {
        return head.getValue();
    }
}
