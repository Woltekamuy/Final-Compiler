#pragma once
#include<iostream>
class Var {
private:
	int index;       // 变量的索引
	int dimension = 0; // 变量的维度，默认值为0
	int dim1;       // 第一维的大小
	int dim2;       // 第二维的大小

public:
	// 构造函数
	Var() : index(0), dim1(0), dim2(0) {}
	Var(int idx) : index(idx), dim1(0), dim2(0) {}

	// 获取成员变量的方法
	int getIndex() const;
	int getDim1() const;
	int getDim2() const;
	int getDimension() const;

	// 设置成员变量的方法
	void setDim1(int dim1);
	void setDim2(int dim2);
	void setDimension(int dimension);
};