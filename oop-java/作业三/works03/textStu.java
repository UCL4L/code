package works03;

import java.util.ArrayList;

public class textStu {
		public static void main(String[] args) {
			ArrayList<Student> list = new ArrayList<Student>();
			Student stu1=new Student(1,"赵钱","男",19,"信工");
			Student stu2=new Student(2,"孙李","女",18,"电工");
			Student stu3=new Student(3,"周吴","男",22,"网工");
			Student stu4=new Student(4,"郑王","女",21,"建工");
			Student stu5=new Student(5,"未知","女",20,"食工");
			list.add(stu1);
			list.add(stu2);
			list.add(stu3);
			list.add(stu4);
			list.add(stu5);
			//根据年龄排序
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