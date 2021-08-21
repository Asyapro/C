#pragma once
class Vector {
public:
	Vector();
	Vector(int size); //������ �������� size, ��������� �� �����
	Vector(int size, double value); //������ �������� size, ��� �������� �������� ����� value
	Vector(int size, double * array); //������, �������� �������� �������������� �� �������
	Vector(Vector& Vector); //����������� �����������
	~Vector();

	//�������� �������������� �������� � ���������
	void operator+=(Vector& Vector);
	void operator-=(Vector& Vector);
	void operator*=(double& number);

	Vector operator=(Vector& Vector);
	Vector operator+(Vector& Vector);
	Vector operator-(Vector& Vector);
	Vector operator*(double& number); 
	
	//��������� ������������ � ���������� �����
	double scalar_multiplication(Vector& vector);
	double Lenght();

	int getRows() { return rows; }
	double getValue(int column);
	void setValue(int column, double value);

	//��� ������ �� �����
	void toString();
private:
	double* values; //���������� ������, � ������� ����� ��������� ���������� �������
	int rows; //������ �������������� ����������� nx1, ��������� � ������ Matrix ������ �������������� ��������� Ax
};

