package ʵ���;

import java.io.File;
public class Content {
	public static void main(String[] args) {
		File file = new File("D:\\java������Ŀ");
		File[] files=file.listFiles();
		findjava(files);
	}
	//�ж�
	public static void findjava(File[] files) {
		if(files==null)
		{
			return;
		}
//		if(file.isDirectory())
//		{
//			System.out.println(file.getAbsolutePath()+"  "+"is Directory");
//		}
		for(File f:files)
		{
			if(f.isFile())
			{
				System.out.println(f.getName());
			}
			else if(f.isDirectory())
			{
				findjava(f.listFiles());//�ݹ����
				if(f.getName().endsWith(".java"))
				{
					System.out.println(f.getName());
				}
			}
		}
	}
}