package 实验九;

import java.io.File;
public class Content {
	public static void main(String[] args) {
		File file = new File("D:\\java导出项目");
		File[] files=file.listFiles();
		findjava(files);
	}
	//判断
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
				findjava(f.listFiles());//递归调用
				if(f.getName().endsWith(".java"))
				{
					System.out.println(f.getName());
				}
			}
		}
	}
}