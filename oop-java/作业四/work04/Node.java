package work04;


//����Ľڵ㣺���ݺ���һ���ڵ�
public class Node {
Object value;
Node next;

public Node(Object o) {
    this.value = o;
}

public Object getValue() {
    return value;
}

public void setValue(Object value) {
    this.value = value;
}

public Node getNext() {
    return next;
}

public void setNext(Node next) {
    this.next = next;
}
}

