/*
 * DecisionTree.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: igormacedo
 */

#include "DecisionTree.h"
using namespace std;

DecisionTree::DecisionTree() {
	// TODO Auto-generated constructor stub

}

DecisionTree::~DecisionTree() {
	// TODO Auto-generated destructor stub
}

AxisDefinition DecisionTree::maximizeInformationGain(ColorList myList)
{
	//This function calculates the threshold that maximizes the information gain
	// and diminishes the entropy.

	AxisDefinition myAxis;
 	double parentEntropy;
	int size = myList.size();
	colorCounter counter(myList);
	//cout << "creating color counter" << endl;

	//double bestThreshold = 0.0;
	double informationGain = DBL_MAX ;

	parentEntropy = (H(counter.red,size)+H(counter.green,size)+H(counter.blue, size));
	myAxis.entropy = parentEntropy;
	//printf("parentEntropy = %0.3f\n", parentEntropy);

	// Separates the colorpoints between two lists according to the threshold
	for(ColorList::iterator it = myList.begin(); it != myList.end(); it++)
	{
		double testThresholdX = it->x;
		ColorList leftList;
		ColorList rightList;

		double testThresholdY = it->y;
		ColorList upperList;
		ColorList lowerList;

		for(ColorList::iterator p = myList.begin(); p != myList.end(); p++)
		{
			if(p->x > testThresholdX)
			{
				rightList.push_back(*p);
			}
			else
			{
				leftList.push_back(*p);
			}

			if(p->y > testThresholdY)
			{
				upperList.push_back(*p);
			}
			else
			{
				lowerList.push_back(*p);
			}
		}

		double IGVertical = CalculateInformationGain(rightList,leftList);
		double IGHorizontal = CalculateInformationGain(upperList,lowerList);

		if(IGVertical < informationGain && IGVertical < IGHorizontal){
			informationGain = IGVertical;
			myAxis.threshold = testThresholdX;
			myAxis.xaligned = true;

			//myAxis.entropy = parentEntropy;
		}

		if(IGHorizontal < informationGain && IGHorizontal < IGVertical){
			informationGain = IGHorizontal;
			myAxis.threshold = testThresholdY;
			myAxis.xaligned = false;
			//myAxis.entropy = parentEntropy;
		}

	}

	//NOTE: this algorithm can be improved by organizing the list by threshold while creating the list
	//cout << "\tIs X-aligned = " << myAxis.xaligned << endl;
	//cout << "\tInformation Gain = " << informationGain << endl;
	//cout << "\tThreshold = " << myAxis.threshold << endl;
	return myAxis;
}

double DecisionTree::CalculateInformationGain(ColorList l1, ColorList l2)
{
			double sizel1 = (double)l1.size();
			double sizel2 = (double)l2.size();
			double size = sizel1+sizel2;

			colorCounter countl1(l1);
			colorCounter countl2(l2);

			double entropyl1 = H(countl1.red, sizel1) + H(countl1.green, sizel1) + H(countl1.blue, sizel1);
			double entropyl2 = H(countl2.red, sizel2) + H(countl2.green, sizel2) + H(countl2.blue, sizel2);
			double newIG = ((sizel1/size)*(entropyl1) + (sizel2/size)*(entropyl2));

			return newIG;
}

double DecisionTree::H(double p, double n)
{
	//Calculates the (partial) entropy of the variable
	if(p/n == 0 || p == n) return 0.0 ;
	else return -p/n*log2(p/n);
}

void DecisionTree::CreateTree(ColorList myList){
	this->head.pointList = myList;
	//this->myTree.push_back(head);
	//this->head.axis = maximizeInformationGain(head.pointList);

	list<TreeNode*> myQueue;
	myQueue.push_back(&head);

	while(!myQueue.empty())
	{
		TreeNode* node = myQueue.front();
		myQueue.pop_front();

		//cout << "Current node Address: " << node << endl;
		//cout << "Node list size: " << node->pointList.size() << endl;
		AxisDefinition axis = maximizeInformationGain(node->pointList);
		//cout << "Node axis entropy = " << axis.entropy << endl;

		if(axis.entropy > 0 && node->level <= 15)
		{
			TreeNode* left = new TreeNode(node->level + 1, node);
			TreeNode* right = new TreeNode(node->level + 1, node);

			//cout << "Address :" << left << " and " << right << endl;

			//left.level = node->level + 1;
			//right.level = node->level + 1;

			//left.parent = node;
			//right.parent = node;

			node->rightNode = right;
			node->leftNode = left;

			node->axis = axis;

			ColorList colorlist = node->pointList;

			for(ColorList::iterator p = colorlist.begin(); p != colorlist.end(); p++)
			{
				double newThreshold;
				if(axis.xaligned)
				{
					newThreshold = p->x;
				}
				else
				{
					newThreshold = p->y;
				}

				if(newThreshold > node->axis.threshold)
				{
					right->pointList.push_back(*p);
				}
				else
				{
					left->pointList.push_back(*p);
				}
			}

			//cout << "left list size: " << left->pointList.size() << endl;
			//cout << "right list size: " << right->pointList.size() << endl;
			myQueue.push_back(left);
			myQueue.push_back(right);

		}

		//Create Line

		glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 0.0f);
			if(axis.xaligned)
			{
				glVertex3f(axis.threshold, -1.0f, 0.0f);
				glVertex3f(axis.threshold, 1.0f, 0.0f);
				//verticalLines.push_back(Line(axis.threshold, -1.0f,axis.threshold, 1.0f));
			}
			else
			{
				glVertex3f(-1.0f, axis.threshold, 0.0f);
				glVertex3f(1.0f, axis.threshold, 0.0f);
				//HorizontalLines.push_back(Line(-1.0f, axis.threshold,1.0f,axis.threshold));
			}
		glEnd();
		glFlush();
		glutSwapBuffers();
		//usleep(2000000);

	}


}

Color DecisionTree::evaluatePoint(ColorPoint point)
{
	TreeNode *node = &this->head;
	TreeNode *lastNode;
	while(node != NULL)
	{
		//cout << "Node Info" << endl;
		//cout << node->axis.threshold << endl;
		//cout << node->axis.xaligned << endl;
		lastNode = node;

		if (node->axis.xaligned)
		{
			//cout << "Aqui 2.1" << endl;
			if(point.x > node->axis.threshold)
			{ //goes to the right
				node = node->rightNode;
			}
			else
			{ //goes to the left
				node = node->leftNode;
			}
		}
		else
		{
			//cout << "Aqui 2.2" << endl;
			if(point.y > node->axis.threshold)
			{
				node = node->rightNode;
			}
			else
			{
				node = node->leftNode;
			}
		}
		//cout << "Aqui 3 out" << endl;
	}

	//cout << "Aqui 4" << endl;

	colorCounter counter(lastNode->pointList);

	//cout << "Aqui 5" << endl;

	if(counter.red > counter.green && counter.red > counter.blue){
		//cout << "ret RED" << endl;
		return RED;
	}
	else if(counter.green > counter.red && counter.green > counter.blue){
		//cout << "ret GREEN" << endl;
		return GREEN;
	}
	else if(counter.blue > counter.red && counter.blue > counter.green){
		//cout << "ret BLUE" << endl;
		return BLUE;
	}
	else{
		//cout << "ret UNDEFINED" << endl;
		return UNDEFINED;
	}

}

