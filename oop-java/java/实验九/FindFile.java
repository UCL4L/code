package ʵ���;

import java.io.File;
public class FindFile{
	public static void main(String[] args) {
		String path="C:\\Windows";
		File file = new File(path);
		judge(file);
	}
	//�ж�
	public static void judge(File file) {
		if(file==null)
		{
			return;
		}
		if(file.isDirectory())
		{
			System.out.println(file.getAbsolutePath()+"  "+"is Directory");
		}
		File[] p=file.listFiles();
		for(File q:p)
		{
			if(!q.isDirectory())
			{
			System.out.println(q+"\t"+"is file");
			}
			
		}
		
		//list()�����Ƿ���ĳ��Ŀ¼�µ������ļ���Ŀ¼���ļ��������ص���String����
		//listFiles()�����Ƿ���ĳ��Ŀ¼�������ļ���Ŀ¼�ľ���·�������ص���File����
	}
}
