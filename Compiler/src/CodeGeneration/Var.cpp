#include "../../include/CodeGeneration/Var.h"

// ��ȡ index ��ֵ
int Var::getIndex() const {
	return index;
}

// ��ȡ dim1 ��ֵ
int Var::getDim1() const {
	return dim1;
}

// ��ȡ dim2 ��ֵ
int Var::getDim2() const {
	return dim2;
}

// ���� dim1 ��ֵ
void Var::setDim1(int dim1) {
	this->dim1 = dim1;
}

// ���� dim2 ��ֵ
void Var::setDim2(int dim2) {
	this->dim2 = dim2;
}

// ��ȡ dimension ��ֵ
int Var::getDimension() const {
	return dimension;
}

// ���� dimension ��ֵ
void Var::setDimension(int dimension) {
	this->dimension = dimension;
}