package main;

public interface IResourceManager{
	/**
	 * �ٲ� ������ GUI�� �����Ű�� �޼ҵ�
	 */
	public void affectVisualSimulator();
	
	/**
	 * �޸� �������� ���� �о���� �޼ҵ�
	 * 
	 * @param locate
	 * @param size
	 * @return
	 */
	public byte[] getMemory(int locate, int size);
	
	/**
	 * �������Ϳ��� ���� �������� �޼ҵ�
	 * 
	 * @param regNum
	 * @return
	 */
	public int getRegister(int regNum);
	
	 
	/**
	 * ����̽� ���ٿ� ���� �޼ҵ�
	 * ����̽��� �� �̸��� ��Ī�Ǵ� ���Ϸ� �����Ѵ�
	 * (F1 �̶�� ����̽��� ������ F1 �̶�� ���Ͽ��� ���� �д´�.)
	 * �ش� ����̽�(����)�� ��� ������ ���·� ����� �޼ҵ�
	 * 
	 * @param devName
	 */
	public void initialDevice(String devName);
	
	/**
	 * �޸� ������ �ʱ�ȭ �ϴ� �޼ҵ�
	 */
	public void initializeMemory();
	
	/**
	 * �� �������� ���� �ʱ�ȭ �ϴ� �޼ҵ�
	 */
	public void initializeRegister();
	
	/**
	 * ������ ����̽�(����)���� ���� �д� �޼ҵ�, �Ķ���ʹ� ���� �����ϴ�.
	 * 
	 * @return
	 */
	public byte readDevice();
	
	/**
	 * �޸� ������ ���� ���� �޼ҵ�
	 * 
	 * @param locate
	 * @param data
	 * @param size
	 */
	public void setMemory(int locate, byte[] data, int size);
	
	/**
	 * �������Ϳ� ���� �����ϴ� �޼ҵ�, regNum�� �������� ������ ��Ÿ����.
	 * 
	 * @param regNum
	 * @param value
	 */
	public void setRegister(int regNum, int value);
	
	/**
	 * ������ ����̽�(����)�� ���� ���� �޼ҵ�, �Ķ���ʹ� ���� �����ϴ�.
	 * 
	 * @param data
	 */
	public void writeDevice(byte[] data);
}
