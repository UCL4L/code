package cn.test10._1;

class Bread{
    //当前面包的一个标号
    private int Id;
    //使用构造方法赋值
    public Bread(int Id){
        this.Id = Id;
    }
    //当前面包的详细信息
    public String toString(){
        return Id+"个馒头";
    }
}
