package works03;

import java.util.ArrayList;

public class textStu {
		public static void main(String[] args) {
			ArrayList<Student> list = new ArrayList<Student>();
			Student stu1=new Student(1,"��Ǯ","��",19,"�Ź�");
			Student stu2=new Student(2,"����","Ů",18,"�繤");
			Student stu3=new Student(3,"����","��",22,"����");
			Student stu4=new Student(4,"֣��","Ů",21,"����");
			Student stu5=new Student(5,"δ֪","Ů",20,"ʳ��");
			list.add(stu1);
			list.add(stu2);
			list.add(stu3);
			list.add(stu4);
			list.add(stu5);
			//������������
			for (int i = 0; i < list.size() - 1; i++) 
			{
				for (int j = 0; j < list.size() - 1 - i; j++) 
				{
					if (list.get(j).getAge() > list.get(j + 1).getAge()) 
					{
						Student student = list.get(j);
						list.set(j, list.get(j + 1));
						list.set(j + 1, student);
					} 
					else if (list.get(j).getAge() == (list.get(j + 1).getAge())) 
					{
						while (list.get(j).getId() > list.get(j + 1).getId()) 
						{
							Student student = list.get(j);
							list.set(j, list.get(j + 1));
							list.set(j + 1, student);
						}
					}
				}
			}
			for (Student stu : list) {
				System.out.println(stu);
			}
		}
	}