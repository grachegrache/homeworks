package main;

import java.io.File;

public interface ISicLoader{
	/**
	 * ���Ϸκ��� �����ڵ带 �о�� �޸𸮿� �ε��Ѵ�.
	 * 
	 * @param objFile
	 */
	public void load(File objFile);
	
	
	/**
	 *  �����ڵ��� �� ���� �а�, �� ���(H, T, M ��)�� �´� ����� �����Ͽ�
	 *  �� �޸� �� ��ɾ� ����Ʈ�� �ʱ�ȭ �Ѵ�.
	 *  
	 * @param line
	 */
	public void readLine(String line);
	
}
