#include "Game.h"
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include "ConsolePrint.h"
//#include <stdio.h>
#include <iostream>
#include "Debug.h"
#include "HeapManager_UnitTest.cpp"
#include "MainRenderLoop.cpp"
//#include "Matrix4.h"
//#include "Vector2.h"
//#include "Vector3.h"
//#include "GStar_init.cpp"
//#include "Coordinate.h"
//#include "SingleLinkedList.h"
//void test();
//void test1();
//void test2();
//void test3();
int main() {
	{
		//HeapManager_UnitTest();
		MainEntrance();
		int a = 0;
		std::cin >> a;
		return a;
		/*GStar::SingleLinkedList<char> TestChar = GStar::SingleLinkedList<char>();
		char next;
		std::cin >> next;
		mygame.Draw();
}
//void test3() {
//	GStar::SingleLinkedList<char> IntList = GStar::SingleLinkedList<char>();
//	char a = '1';
//	char b = '2';
//	char c = '3';
//	IntList.Push(a);
//	IntList.Push(b);
//	IntList.Push(c);
//	GStar::SingleLinkedListNode<char>* temp = IntList.GetHead();
//	temp = temp->GetNext();
//	while (temp != nullptr) {
//		printf("%c", temp->GetData());
//		temp = temp->GetNext();
//	}
//	int d;
//	std::cin >> d;
//}
//void test2() {
//	float daat[4][4] = {
//			{1,0,0,1},
//		{0,1, 0, 1},
//		{ 0,0,1,1 },
//		{ 0,0,0,1 }, };
//	GStar::Matrix4 test = GStar::Matrix4(daat);
//	float determinant = test.determinant();
//	GStar::Matrix4 test2 = test.I();
//
//	GStar::Initialize();
//	GStar::Vector3 Object = GStar::Vector3(0, 0, 1);
//	GStar::Coordinate tempcoordinate = GStar::Coordinate(GStar::Coordinate::TYPE::OBJECT);
//
//	tempcoordinate.Rotate(90, 90, 90);
//	Object = tempcoordinate.Apply(Object);
//
//	Object = tempcoordinate.Reverse(Object);
//
//
//
//	float da[4][4] = {
//		{1,2,3,0},
//	{2, 0, 0, 0},
//	{ 0,0,0,0 },
//	{ 0,0,0,0 }, };
//	GStar::Matrix4 matrix1 = GStar::Matrix4(da);
//	GStar::Matrix4 matrix2 = GStar::Matrix4(da);
//	GStar::Matrix4 matrix3 = (matrix1.Dot(matrix2).Dot(matrix1));
//	GStar::Matrix4 matrix4 = matrix1.T();
//
//	float da1[4][4] = {
//		{1,0,0,0},
//	{0, 2, 0, 0},
//	{ 0,0,3,0 },
//	{ 0,0,0,4 }, };
//	GStar::Matrix4 matrix5 = GStar::Matrix4(da1);
//	float y = matrix5.determinant();
//	float prey = matrix5.determinantc();
//	GStar::Matrix4 matrix6 = matrix5.I();
//
//	GStar::Terminate();
//	int a;
//	std::cin >> a;
//}
//void test() {
//	GStar::Matrix4 matrix1 = GStar::Matrix4(da);
//	GStar::Matrix4 matrix2 = GStar::Matrix4(da);
//
//	GStar::Matrix4 matrix3 = matrix1 + matrix2;
//	matrix3 = (matrix1 + matrix2 + matrix3);
//
//
//	float a = 0;
//
//	return;
//}
//
//void test1() {
//
//	GStar::Initialize();
//	GStar::Vector3 v1(1, 0, 0);
//	GStar::Vector3 v2(0, 0, 1);
//	printf("%f\n", (v1 + v2).Length());
//	printf("%f\n", (v1 * v2).Length());
//	printf("%f\n", v1.Dot(v2));
//	printf("%f\n", GStar::Vector3::ProjAtoB(v1, v2).Length());
//}
//
