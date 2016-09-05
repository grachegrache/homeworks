package main;

import java.io.File;

public interface ISicSimulator{
	/**
	 * ������ ����� �α׿� �߰��ϴ� �޼ҵ�
	 */
	public void addLog();
	
	/**
	 * ���� ��ɾ ��� �����ϴ� �޼ҵ�.
	 * ���� �ڵ带 ��� �����ϰ� �� ���� ��ȭ�� �����ش�.
	 * �������� ������ �����ϴ� �޼ҵ�.
	 */
	public void allStep();
	
	/**
	 * �ùķ����͸� ���۽�Ű�� ���� ������ �����Ѵ�.
	 * �޸� �۾� �� �������� �ʱ�ȭ �۾��� �����Ѵ�.
	 * �� ��ɾ ����Ǿ� �ִ� inst.data������ �а� �����Ѵ�.
	 * 
	 * @param instFile
	 */
	public void initialize(File instFile);
	
	/**
	 * �ϳ��� ��ɾ �����Ѵ�. �ش� ��ɾ ����ǰ� �� ���� ��ȭ��
	 * �����ְ�, ���� ��ɾ �������Ѵ�.
	 * �������� ������ �����ϴ� �޼ҵ�
	 */
	public void oneStep();
}
