package cn.test10._1;

class MyStack{
    //����ͷ�ŵ�������
    private Bread[] arr = new Bread[10];
    //��ǰ������ͷ��һ��ָ���ʾ��ǰ��λ��
    private int index = 0;

    //������ͷ
    public synchronized void push(Bread bread){
        //����������ͷ������������ʱ���ø��߳��ͷ������ڵȴ���һ��״̬
        if(index == arr.length){
            try{
                this.wait();
            }catch(Exception e){
                System.out.println("����������Χ");
            }
        }
        //�������ڵȴ����߳�
        this.notify();
        //����������ͷ�ŵ�������
        arr[index] = bread;
        //����ǰ��ָ���ƶ�����һ��λ��
        index++;

        try{
            Thread.sleep(1000);
            System.out.println("������"+bread);
        }catch(Exception e){
            System.out.println("�ж��쳣");
        }
    }

    //������ͷ
    public synchronized Bread pop(){
        //��������û�����ʱ���ø��̴߳��ڵȴ�״̬�������ͷ���
        if(index==0){
            try{
                this.wait();
            }catch(Exception e){
                System.out.println("����������Χ");
            }
        }
        //�������ڵȴ����߳�
        this.notify();
        //����ǰָ���λ���ƶ�����һ��
        index--;
        try{
            Thread.sleep(1000);
            System.out.println("������"+arr[index]);
        }catch(Exception e){
            System.out.println("�ж��쳣");
        }
        //��ʾ��ǰ����ͷ
        return arr[index];
    }
}
