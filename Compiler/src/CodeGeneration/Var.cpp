#include "../../include/CodeGeneration/Var.h"

// 获取 index 的值
int Var::getIndex() const {
	return index;
}

// 获取 dim1 的值
int Var::getDim1() const {
	return dim1;
}

// 获取 dim2 的值
int Var::getDim2() const {
	return dim2;
}

// 设置 dim1 的值
void Var::setDim1(int dim1) {
	this->dim1 = dim1;
}

// 设置 dim2 的值
void Var::setDim2(int dim2) {
	this->dim2 = dim2;
}

// 获取 dimension 的值
int Var::getDimension() const {
	return dimension;
}

// 设置 dimension 的值
void Var::setDimension(int dimension) {
	this->dimension = dimension;
}