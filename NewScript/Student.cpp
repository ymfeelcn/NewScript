//
//  Student.cpp
//  MyCppGame
//
//  Created by guanghui on 8/11/14.
//
//

#include "stdafx.h"

#include "Student.h"


Student::Student()
	:name("default")
{
	LogTrace("Student Contructor called");
}

Student::~Student()
{
	LogTrace("Student Destructor called");
}

CString Student::getName()
{
	return name;
}

void Student::setName(CString name)
{
	this->name = name;
}

int Student::getAge()
{
	return age;
}

void Student::setAge(int age)
{
	this->age = age;
}

void Student::print()
{
	//cout << "My name is: " << name << ", and my age is " << age << endl;
	CString strTmp;
	strTmp.Format(_T("My name is: %s , and my age is %d"), name, age);
	AfxMessageBox(strTmp);
}