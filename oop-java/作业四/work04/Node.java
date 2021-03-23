package work04;


//链表的节点：数据和下一个节点
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

