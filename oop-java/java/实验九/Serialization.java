package ʵ���;

import java.io.*;
public class Serialization {
		public static void main(String[] args) throws IOException, ClassNotFoundException{		
//		Serialization t=new Serialization();//����һ���ⲿʵ��
//		Student stu1=t.new Student(1,"����",18);
//		Student stu2=t.new Student(2,"����",20);

			Student stu1=new Student(1,"����",18);
			Student stu2=new Student(2,"����",20);
		/**
		 * FileOutputStream
		 * ObjectOutputStream
		 * ������д���ļ�
		 *writeObject д���ļ�
		 * */		
		/**
		 * FileInputStream
		 * ObjectInputStream
		 * ��ȡ�������ļ����������
		 * */
		
		//�ļ�����
		FileOutputStream file=new FileOutputStream("student.txt");
		ObjectOutputStream files=new ObjectOutputStream(file);
		files.writeObject(stu1);
		files.writeObject(stu2);
		files.close();
		
		//�ļ���ȡ
		FileInputStream f=new FileInputStream("student.txt");
		ObjectInputStream fi=new ObjectInputStream(f);
		stu1=(Student)fi.readObject();
		stu2=(Student)fi.readObject();
		fi.close();
		
		System.out.println("id:"+stu1.id);
		System.out.println("name:"+stu1.name);
		System.out.println("age:"+stu1.age);
		System.out.println();
		System.out.println("id:"+stu2.id);
		System.out.println("name:"+stu2.name);
		System.out.println("age:"+stu2.age);

	}
}
