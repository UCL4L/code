package ʵ���;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
public class FileInputstream {
	
	public static void main(String[] args) {
		byte[] bytes=new byte[2056];
		try{
			@SuppressWarnings("resource")
			FileInputStream file=new FileInputStream("D:/java������Ŀ/hello.txt");
			try {
				@SuppressWarnings("unused")
				int data=file.read(bytes,0,2056);
				String str=new String(bytes,0,2056);
				System.out.println(str);
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}		
			}
		catch(FileNotFoundException e)
		{
			e.printStackTrace();
		}
	}
}

