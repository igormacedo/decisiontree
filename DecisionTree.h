/*
 * DecisionTree.h
 * Created on: Jun 17, 2015
 *      Author: igormacedo
 */

#include <list>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cfloat>
#include <stdlib.h>
#include <GL/glut.h>

#include <chrono>
#include <unistd.h>

using namespace std;

#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

enum Color { RED, GREEN, BLUE, UNDEFINED};

typedef struct ColorPoint
{
	float x;
	float y;
	Color color;

	ColorPoint()
	{
		this->x = 0;
		this->y = 0;
		this->color = UNDEFINED;
	}
	ColorPoint(float x, float y)
	{
		this->x = x;
		this->y= y;
		this->color = UNDEFINED;
	}
	ColorPoint(float x, float y,Color color)
	{
		this->x = x;
		this->y = y;
		this->color = color;
	}

}ColorPoint;

typedef struct AxisDefinition
{
	bool xaligned;
	double threshold;
	double entropy;
}AxisDefinition;

typedef list<ColorPoint> ColorList;

typedef struct TreeNode
{
	AxisDefinition axis;
	list<ColorPoint> pointList;
	TreeNode* parent;
	TreeNode* rightNode;
	TreeNode* leftNode;
	int level;

	TreeNode()
	{
		this->parent = NULL;
		this->leftNode = NULL;
		this->rightNode = NULL;
		this->level = 1;
	}
	TreeNode(int level, TreeNode* parent)
	{
		this->rightNode = NULL;
		this->leftNode = NULL;
		this->level = level;
		this->parent = parent;
		//this->pointList = new ColorList();
	}
	TreeNode(TreeNode* rightNode, TreeNode* leftNode, TreeNode* parent, AxisDefinition axis)
	{
		this->parent = parent;
		this->leftNode = leftNode;
		this->rightNode = rightNode;
		this->level = -1;
		this->axis = axis;
	}
	bool compareThreshold(float value)
	{
		if(this->axis.threshold > value)
		{
			return true;
		}
		else{
			return false;
		}
	}
	void addColorPointToList(ColorPoint point)
	{
		this->pointList.push_back(point);
	}

}TreeNode;

typedef struct colorCounter
{
	int red, green, blue;

	colorCounter(list<ColorPoint> l)
	{
		newCount(l);
	}

	void newCount(list<ColorPoint> l)
	{
		int r = 0, g = 0, b = 0;
		for(list<ColorPoint>::iterator it = l.begin(); it != l.end(); it++)
		{
			switch(it->color)
			{
				case RED:
					r++; break;
				case GREEN:
					g++; break;
				case BLUE:
					b++; break;
				default:
					break;
			}
		}

		this->red = r;
		this->green = g;
		this->blue = b;
	}

}colorCounter;

typedef struct Line
{
	float x1, y1;
	float x2, y2;

	Line(float x1, float y1, float x2, float y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
}Line;

class DecisionTree
{
	public:
		DecisionTree();
		virtual ~DecisionTree();

		AxisDefinition maximizeInformationGain(ColorList);
		double CalculateInformationGain(ColorList, ColorList);
		double H(double,double);

		void CreateTree(ColorList);
		Color evaluatePoint(ColorPoint);

	private:
		TreeNode head;
		list<TreeNode> myTree;

		list<Line> verticalLines;
		list<Line> HorizontalLines;

		//Line closestLine();

};

#endif /* DECISIONTREE_H_ */




























