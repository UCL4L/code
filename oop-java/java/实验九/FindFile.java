package 实验九;

import java.io.File;
public class FindFile{
	public static void main(String[] args) {
		String path="C:\\Windows";
		File file = new File(path);
		judge(file);
	}
	//判断
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
		
		//list()方法是返回某个目录下的所有文件和目录的文件名，返回的是String数组
		//listFiles()方法是返回某个目录下所有文件和目录的绝对路径，返回的是File数组
	}
}
