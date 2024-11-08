#pragma once
#include<iostream>
class Var {
private:
	int index;       // ����������
	int dimension = 0; // ������ά�ȣ�Ĭ��ֵΪ0
	int dim1;       // ��һά�Ĵ�С
	int dim2;       // �ڶ�ά�Ĵ�С

public:
	// ���캯��
	Var() : index(0), dim1(0), dim2(0) {}
	Var(int idx) : index(idx), dim1(0), dim2(0) {}

	// ��ȡ��Ա�����ķ���
	int getIndex() const;
	int getDim1() const;
	int getDim2() const;
	int getDimension() const;

	// ���ó�Ա�����ķ���
	void setDim1(int dim1);
	void setDim2(int dim2);
	void setDimension(int dimension);
};