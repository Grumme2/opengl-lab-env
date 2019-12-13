//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2017 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
//#include <bits/stdc++.h>
#include "imgui.h"
//#include "btree.h"
#include <glm/glm.hpp>
#include <math.h> 
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
# define M_PIl          3.141592653589793238462643383279502884L
#define STRING_BUFFER_SIZE 8192

#include <fstream>
using namespace std;

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
//"uniform mat4 rotationMatrix;\n"
"uniform float angle;\n"
"uniform bool foo;\n"
"void main()\n"
"{\n"
"vec3 newPos;\n"
"float offset = float(gl_VertexID);\n"
//"gl_Position = rotationMatrix * vec4(pos.x,pos.y,pos.z,1.0f);\n"
//"pos.x = pos.x + cos(angle + offset)\n;"
"if (foo){\n"
"newPos.x = pos.x + 0.05f * cos(angle + offset)\n;"
//"pos.y = pos.y + sin(angle + offset)\n;" 
"newPos.y = pos.y + 0.05 * sin(angle + offset)\n;"
"newPos.z = -1\n;"
//"vec3 posrot(x,y,-1)"
"gl_Position = vec4(newPos, 1);\n"
"}\n"
"else{\n"
	"gl_Position = vec4(pos, 1); \n"
"}\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace Example
{

	//------------------------------------------------------------------------------
	/**
	*/
	ExampleApp::ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ExampleApp::~ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/

	struct Dbg
	{
		GLfloat pos[3];
		GLfloat color[4];
	};

	std::vector<GLfloat> pointsarr;
	std::vector<GLfloat> convexhullarr;
	std::vector<GLfloat> fantra;
	std::vector<GLfloat> fantraLines;

	std::vector<unsigned int> pointsindex;
	std::vector<unsigned int> convexhullindex;
	std::vector<unsigned int> fantraindex;
	std::vector<unsigned int> fantraLinesindex;



	std::vector<glm::vec2> insideconvexhull;
	std::vector<glm::vec2> allPointsVec;
	size_t bufferSize;
	size_t bufferSize2;
	size_t bufferSize3;
	size_t bufferSize4;
	GLfloat sqrt3 = 1.73205080757;
	GLfloat* buf;
	unsigned int* bufindex;
	GLfloat* buf2;
	unsigned int* buf2index;
	GLfloat* buf3;
	unsigned int* buf3index;
	GLfloat* buf4;
	unsigned int* buf4index;
	GLfloat angle2 = 0;
	int rect = 2;
	int refreshmill = 1;
	bool pointsBool = true;
	bool convexhullbool = true;
	bool fantriangulationbool = true;
	bool trianglesoupbool = true;
	bool readfromfilebool = true;
	bool randompointsbool = false;
	bool movepointsbool = true;
	

	struct triangle
	{
		glm::vec2 c;
		glm::vec2 ci;
		glm::vec2 cj;
	};

	struct node 
	{ 
		virtual ~node() { };
		node *parrent = NULL;
	};

	struct binnode:node
	{
		glm::vec2 c;
		glm::vec2 cm;
		glm::vec2 ci;
		glm::vec2 cj;

		//node *parrent = NULL;
		node *left = NULL;
		node *right = NULL;
	};

	struct threenode:node
	{
		glm::vec2 c;
		glm::vec2 cm;
		glm::vec2 ci;
		glm::vec2 cj;

		//node *parrent = NULL;
		node *left = NULL;
		node *middle  = NULL;
		node *right = NULL;
	};


	struct leaf:node
	{
		//node *parrent = NULL;
		triangle *triangle = NULL;
	};
	
	struct root:node 
	{
		node *child = NULL;
	};

	root* fantriangluation;

	ifstream inFile;

	/*
	Takes a std::vector<glm::vec2> vector and makes it a std::vector<GLfloat>
	*/
	std::vector<GLfloat> vec2ToGlfloat(std::vector<glm::vec2> points) {
		std::vector<GLfloat> glfloatvec;
		for (int i = 0; i < points.size(); i++) {
			std::vector<GLfloat> glfloatvecTemp = {
				points.at(i).x,	points.at(i).y,	-1,
				1,	0,	0,	 0.5
			};
			glfloatvec.insert(glfloatvec.end(), glfloatvecTemp.begin(), glfloatvecTemp.end());
		}
		return glfloatvec;
	}

	/*
	Takes a std::vector<GLfloat> vector and makes it a std::vector<glm::vec2> 
	*/
	std::vector<glm::vec2> glfloatToVec2(std::vector<GLfloat> points) {
		std::vector<glm::vec2> vec2vec;
		for (int i = 0; i < points.size() / 7; i++) {
			float x = points.at(i * 7);
			float y = points.at((i * 7) + 1);
			vec2vec.push_back(glm::vec2(x, y));
		}
		return vec2vec;
	}

	/*
	Readspoints from chosen file
	*/
	std::vector<GLfloat> readfromfile() {
		int sum = 0;
		float x, y;
		std::vector<GLfloat> fin;
		inFile.open("datafile.txt");
		if (!inFile) {
			cerr << "Unable to open file datafile.txt";
			exit(1);   // call system to stop
		}
		while (inFile >> x >> y) {
			std::vector<GLfloat> calcpoints = {
			x,	y,	-1,
			1,	0,	1,	 0.5
			};
			fin.insert(fin.end(), calcpoints.begin(), calcpoints.end());
		}
		inFile.close();

		return fin;
	}


	std::vector<GLfloat> readfromfile2() {
		int sum = 0;
		float x, y;
		std::vector<GLfloat> fin;
		inFile.open("datafile2.txt");
		if (!inFile) {
			cerr << "Unable to open file datafile.txt";
			exit(1);   // call system to stop
		}
		while (inFile >> x >> y) {
			std::vector<GLfloat> calcpoints = {
			x,	y,	-1,
			1,	0,	1,	 0.5
			};
			fin.insert(fin.end(), calcpoints.begin(), calcpoints.end());
		}
		inFile.close();

		return fin;
	}





	/*
	Generates a set of radom points.
	*/
	std::vector<GLfloat> randomPoints(int nrOfPoints) {
		std::vector<GLfloat> fin;
		int i = 0;
		while (i < nrOfPoints) {
			float xrand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2);
			float xpoint = xrand - 1;
			float yrand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2);
			float ypoint = yrand - 1;
			std::vector<GLfloat> calcpoints = {
			xpoint,	ypoint,	-1,
			1,	0,	0,	 0.5
			};
			fin.insert(fin.end(), calcpoints.begin(), calcpoints.end());
			i++;
		}
		return fin;
	}

	/*
	Help function for sortig of glm::vec2
	*/
	bool MyVec3ComparisonFunc(const glm::vec2 &vecA, const glm::vec2 &vecB)
	{
		return vecA.x < vecB.x;
	}

	/*
	Checks if point turns clockwise relative two other points 
	*/
	int TurnsClockwise(glm::vec2 head, glm::vec2 tail, glm::vec2 newpoint) {
		float d = (newpoint.x - head.x)*(tail.y - head.y) - (newpoint.y - head.y)*(tail.x - head.x);
		if (d > 0) {
			return 1;
		}else if(d==0){
			return 0;
		}
		else {
			return -1;
		}

	}


	/*
	function that checks if points is in sector 
	returns 1 if its in the sector 
	returns 0 if its on the edge of the sector
	returns -1 if its outside the sector
	leftof is cm in håkans exemple
	rightof is ci in håkans example
	at https://youtu.be/Xbl5j02EN-Q?t=982
	*/
	int checksector(glm::vec2 leftof, glm::vec2 center, glm::vec2 rightof, glm::vec2 newpoint) {

		int case1 = TurnsClockwise(center, leftof, rightof);
		int a1 = TurnsClockwise(center, rightof, newpoint);
		int a2 = TurnsClockwise(center, leftof, newpoint);
		if (case1 < 1) {
			if ((a1 == 1) && (a2 == -1)) {
				return 1;
			}
			else if (((a1 == 1) && (a2 == 0)) || ((a1 == 0) && (a2 == -1)) || ((a1 == 0) && (a2 == 0))) {
				return 0;
			}
			else {
				return -1;
			}
		}
		else {
			if ((a1 == 1) || (a2 == -1)) {
				return 1;
			}
			else if (((a1 == 1) || (a2 == 0)) || ((a1 == 0) || (a2 == -1)) || ((a1 == 0) || (a2 == 0))) {
				return 0;
			}
			else {
				return -1;
			}
		}
	}

	/*
	creates the convex hull of aa given set of points
	*/
	std::vector<glm::vec2> convexhull(std::vector<glm::vec2> points) {

		std::sort(points.begin(), points.end(), &MyVec3ComparisonFunc);

		std::vector<glm::vec2 > upper;
		std::vector<glm::vec2 > lower;

		for (int i = 0; i < points.size(); i++) {
			int lowsize = lower.size();
			while (lowsize >= 2 && (TurnsClockwise(lower.at(lowsize - 1), lower.at(lowsize - 2), points.at(i))==1)) {
				lower.pop_back();
				lowsize = lower.size();
			}

			lower.push_back(points.at(i));
		}

		for (int i = points.size()-1; i >=0 ; i--) {
			int uppersize = upper.size();
			while (uppersize >= 2 && (TurnsClockwise(upper.at(uppersize - 1), upper.at(uppersize - 2), points.at(i))==1)) {
				upper.pop_back();
				uppersize = upper.size();
			}

			upper.push_back(points.at(i));
		}

		//lower.pop_back();
		upper.pop_back();

		upper.insert(upper.end(), lower.begin(), lower.end());

		return upper;
	}


	/*
	Builds the fantriangultion binary tree 
	*/
	node* buildTree(glm::vec2 c, std::vector<glm::vec2> convexhull, node* parrent) {
		binnode* mybinnode = new binnode();
		if (convexhull.size() == 2) {
			leaf* leafvar = new leaf();
			triangle* trianglevar = new triangle();

			trianglevar->c = c;
			trianglevar->ci = convexhull.at(0);
			trianglevar->cj = convexhull.at(convexhull.size() - 1);

			leafvar->parrent = parrent;
			leafvar->triangle = trianglevar;
			return leafvar;
		}
		int m = convexhull.size() / 2;
		mybinnode->c = c;
		mybinnode->cm = convexhull.at(m);
		mybinnode->ci = convexhull.at(0);
		mybinnode->cj = convexhull.at(convexhull.size() - 1);

		vector<glm::vec2>::const_iterator first = convexhull.begin();
		vector<glm::vec2>::const_iterator last = convexhull.begin() + m+1;
		vector<glm::vec2> newVec1(first, last);

		vector<glm::vec2>::const_iterator first2 = convexhull.begin()+m;
		vector<glm::vec2>::const_iterator last2 = convexhull.end();
		vector<glm::vec2> newVec2(first2, last2);

		mybinnode->left = buildTree(c, newVec1, mybinnode); 
		mybinnode->right = buildTree(c, newVec2, mybinnode);
		mybinnode->parrent = parrent;

		return mybinnode;

			
	}


	/*
	Calls buildtree function and takes out tge values that buildtree needs
	*/
	root* fanTriangulation(std::vector<glm::vec2> allpoints, std::vector<glm::vec2> convexhull) {
		insideconvexhull.clear();
		for (int i = 0; i < allpoints.size(); i++) {
			if (std::find(convexhull.begin(), convexhull.end(), allpoints.at(i)) != convexhull.end()) {
				continue;
			}
			else {
				insideconvexhull.push_back(allpoints.at(i));
			}
		}
		if (insideconvexhull.size() > 0) {
			int cind = insideconvexhull.size() / 2;
			root* rootnode = new root();
			rootnode->child = buildTree(insideconvexhull.at(cind), convexhull, rootnode);
			insideconvexhull.erase(insideconvexhull.begin() + cind);
			return rootnode;
		}
		root* rootnode = new root();
		rootnode->child = buildTree(convexhull.at(0), convexhull, rootnode);
		return rootnode;
	}

	/*
	traverses tree recursivly and returns all the triangles in a vector
	*/
	std::vector<glm::vec2> getfromtree(node* btree) {
		vector<glm::vec2> trianglepoints;
		
		if (leaf* leafnode = dynamic_cast<leaf*>(btree)) {
			triangle* tria = leafnode->triangle;
			trianglepoints.push_back(tria->c);
			trianglepoints.push_back(tria->ci);
			trianglepoints.push_back(tria->cj);
			return trianglepoints;
		}
		
		if (binnode* binode = dynamic_cast<binnode*>(btree)) {
			vector<glm::vec2> trianglesleft = getfromtree(binode->left);
			vector<glm::vec2> trianglesright = getfromtree(binode->right);

			trianglesleft.insert(trianglesleft.end(), trianglesright.begin(), trianglesright.end());
			return trianglesleft;
		}

		if (threenode* trenode = dynamic_cast<threenode*>(btree)) {
			vector<glm::vec2> trianglesleft = getfromtree(trenode->left);
			vector<glm::vec2> trianglesright = getfromtree(trenode->right);
			vector<glm::vec2> trianglesmiddle = getfromtree(trenode->middle);

			trianglesleft.insert(trianglesleft.end(), trianglesright.begin(), trianglesright.end());
			trianglesleft.insert(trianglesleft.end(), trianglesmiddle.begin(), trianglesmiddle.end());
			return trianglesleft;
		}
	}

	/*
	gets child from root and calls getfromtree dunction
	*/
	std::vector<glm::vec2> getfromroot(root* btree) {

		node *rootchild = btree->child;
		return getfromtree(rootchild);
	}
	
	/*
	traverses tree recursivly and returns all the lines in a vector
	*/
	std::vector<glm::vec2> getLinesFromTree(node* btree) {
		vector<glm::vec2> linepoints;

		if (leaf* leafnode = dynamic_cast<leaf*>(btree)) {
			triangle* tria = leafnode->triangle;
			linepoints.push_back(tria->c);
			linepoints.push_back(tria->ci);

			linepoints.push_back(tria->ci);
			linepoints.push_back(tria->cj);

			linepoints.push_back(tria->cj);
			linepoints.push_back(tria->c);
			return linepoints;
		}

		if (binnode* binode = dynamic_cast<binnode*>(btree)) {
			vector<glm::vec2> linesLeft = getLinesFromTree(binode->left);
			vector<glm::vec2> linesRight = getLinesFromTree(binode->right);

			linesLeft.insert(linesLeft.end(), linesRight.begin(), linesRight.end());
			return linesLeft;
		}

		if (threenode* trenode = dynamic_cast<threenode*>(btree)) {
			vector<glm::vec2> linesLeft = getLinesFromTree(trenode->left);
			vector<glm::vec2> linesRight = getLinesFromTree(trenode->right);
			vector<glm::vec2> lineMiddle = getLinesFromTree(trenode->middle);

			linesLeft.insert(linesLeft.end(), linesRight.begin(), linesRight.end());
			linesLeft.insert(linesLeft.end(), lineMiddle.begin(), lineMiddle.end());
			return linesLeft;
		}
	}
	/*
	gets child from root and calls getLinesfromtree dunction
	*/
	std::vector<glm::vec2> getLinesFromRoot(root* btree) {

		node *rootchild = btree->child;
		return getLinesFromTree(rootchild);
	}



	/*
	Recursive function that get called when a point is on the line 
	then when it hits a leaf it inserts a binary node with new triangle
	*/
	node* onTheLine(node* btree, glm::vec2 point) {
		if (leaf* leafnode = dynamic_cast<leaf*>(btree)) {
			triangle* tria = leafnode->triangle;
			glm::vec2 c = tria->c;
			glm::vec2 ci = tria->ci;
			glm::vec2 cj = tria->cj;

			leaf* leaf1 = new leaf();
			leaf* leaf2 = new leaf();

			triangle* triangle1 = new triangle();
			triangle* triangle2 = new triangle();

			binnode* binode = new binnode();

			int onLineCCi = TurnsClockwise(c, ci, point);
			int onLineCjC = TurnsClockwise(cj, c, point);
			int onLineCJCi = TurnsClockwise(cj, ci, point);

			if (onLineCCi == 0) {
				if ((TurnsClockwise(point, cj, c) > 0)) {
					triangle2->c = point;
					triangle2->ci = cj;
					triangle2->cj = c;

					triangle1->c = point;
					triangle1->ci = ci;
					triangle1->cj = cj;
				}
				else {
					triangle1->c = point;
					triangle1->ci = cj;
					triangle1->cj = c;

					triangle2->c = point;
					triangle2->ci = ci;
					triangle2->cj = cj;
				}
				leaf1->triangle = triangle1;
				leaf2->triangle = triangle2;
			}
			else if(onLineCjC == 0){
				if ((TurnsClockwise(point, ci, c) > 0)) {
					triangle1->c = point;
					triangle1->ci = cj;
					triangle1->cj = ci;

					triangle2->c = point;
					triangle2->ci = ci;
					triangle2->cj = c;
				}
				else {
					triangle2->c = point;
					triangle2->ci = cj;
					triangle2->cj = ci;

					triangle1->c = point;
					triangle1->ci = ci;
					triangle1->cj = c;
				}
				leaf1->triangle = triangle1;
				leaf2->triangle = triangle2;
			}
			else {
				if ((TurnsClockwise(point, c, cj) > 0)) {
					triangle1->c = point;
					triangle1->ci = ci;
					triangle1->cj = c;

					triangle2->c = point;
					triangle2->ci = c;
					triangle2->cj = cj;
				}
				else {
					triangle2->c = point;
					triangle2->ci = ci;
					triangle2->cj = c;

					triangle1->c = point;
					triangle1->ci = c;
					triangle1->cj = cj;
				}

				leaf1->triangle = triangle1;
				leaf2->triangle = triangle2;
			}

			leaf1->parrent = binode;
			leaf2->parrent = binode;
			
			binode->c = point;
			binode->ci = ci;
			binode->cj = cj;
			binode->cm = c;
			binode->left = leaf1;
			binode->right = leaf2;
			binode->parrent = leafnode->parrent;

			return binode;

		}
		if (binnode* binode = dynamic_cast<binnode*>(btree)) {
			glm::vec2 c = binode->c;
			glm::vec2 ci = binode->ci;
			glm::vec2 cj = binode->cj;
			glm::vec2 cm = binode->cm;

			int leftsector = checksector(cm, c, ci, point);
			int rightsector = checksector(cj, c, cm, point);

			if (leftsector == 0) {
				binode->left = onTheLine(binode->left, point);
				binode->left->parrent = binode;
			}
			else{
				binode->right = onTheLine(binode->right, point);
				binode->right->parrent = binode;
			}

			return binode;

		}

		if (threenode* trenode = dynamic_cast<threenode*>(btree)) {
			glm::vec2 c = trenode->c;
			glm::vec2 ci = trenode->ci;
			glm::vec2 cj = trenode->cj;
			glm::vec2 cm = trenode->cm;

			int leftsector = checksector(cm, c, ci, point);
			int middlesector = checksector(cj, c, ci, point);
			int rightsector = checksector(cj, c, cm, point);

			int leftbase = TurnsClockwise(ci, cm, point);
			int middlebase = TurnsClockwise(cj, ci, point);
			int rightbase = TurnsClockwise(cj, cm, point);
			

			if ((leftsector == 0) || (leftbase == 0)) {
				trenode->left = onTheLine(trenode->left, point);
				trenode->left->parrent = trenode;
			}
			if ((middlesector == 0) || (middlebase == 0)) {
				trenode->middle = onTheLine(trenode->middle, point);
				trenode->middle->parrent = trenode;
			}
			if ((rightsector == 0) || (rightbase == 0)) {
				trenode->right = onTheLine(trenode->right, point);
				trenode->right->parrent = trenode;
			}
			return trenode;
		}
	}

	/*
	Recursive function that takes a point and find where in the tree it should lie 
	ad then creates a ternary node that it inserts the three new triangles in and 
	adds the ternery node to the tree
	*/
	node* addPointToTree(node* btree, glm::vec2 point) {
		if (leaf* leafnode = dynamic_cast<leaf*>(btree)) {
			triangle* tria = leafnode->triangle;
			glm::vec2 c = tria->c;
			glm::vec2 ci = tria->ci;
			glm::vec2 cj = tria->cj;

			leaf* leaf1 = new leaf();
			leaf* leaf2 = new leaf();
			leaf* leaf3 = new leaf();

			triangle* triangle1 = new triangle();
			triangle* triangle2 = new triangle();
			triangle* triangle3 = new triangle();

			triangle1->c = point;
			triangle1->ci = ci;
			triangle1->cj = cj;

			triangle2->c = point;
			triangle2->ci = cj;
			triangle2->cj = c;

			triangle3->c = point;
			triangle3->ci = c;
			triangle3->cj = ci;

			leaf1->triangle = triangle1;
			leaf2->triangle = triangle2;
			leaf3->triangle = triangle3;

			threenode* trenode = new threenode();

			leaf1->parrent = trenode;
			leaf2->parrent = trenode;
			leaf3->parrent = trenode;


			trenode->c = point;
			trenode->ci = ci;
			trenode->cj = cj;
			trenode->cm = c;
			trenode->left = leaf3;
			trenode->middle = leaf1;
			trenode->right = leaf2;
			trenode->parrent = leafnode->parrent;

			 //return switchchild(leafnode->parrent, trenode, point);
			return trenode;

		}
		if (binnode* binode = dynamic_cast<binnode*>(btree)) {
			glm::vec2 c = binode->c;
			glm::vec2 ci= binode->ci;
			glm::vec2 cj= binode->cj;
			glm::vec2 cm = binode->cm;

			int leftsector = checksector(cm, c, ci, point);
			int rightsector = checksector(cj, c, cm, point);
		
			if (leftsector == 1) {
				 binode->left = addPointToTree(binode->left, point);
				 binode->left->parrent = binode;
			}
			else if (leftsector == 0 || rightsector == 0) {
				if (leftsector == 0) {
					binode->left = onTheLine(binode->left, point);
					binode->left->parrent = binode;
				}
				if (rightsector == 0) {
					binode->right = onTheLine(binode->right, point);
					binode->right->parrent = binode;
				}
			}
			else {
				binode->right = addPointToTree(binode->right, point);
				binode->right->parrent = binode;
			}

			return binode;
	
			
		}

		if (threenode* trenode = dynamic_cast<threenode*>(btree)) {
			glm::vec2 c = trenode->c;
			glm::vec2 ci = trenode->ci;
			glm::vec2 cj = trenode->cj;
			glm::vec2 cm = trenode->cm;


			int rightsector = checksector(cm, c, cj, point);
			int leftsector = checksector(ci, c, cm, point);
			int middlesector = checksector(cj, c, ci, point);


			if (rightsector == 1) {
				trenode->right = addPointToTree(trenode->right, point);
				trenode->right->parrent = trenode;
			}
			else if(middlesector == 1) {
				trenode->middle =  addPointToTree(trenode->middle, point);
				trenode->middle->parrent = trenode;
			}
			else if (rightsector == 0 || leftsector == 0 || middlesector == 0) {
				if (leftsector == 0) {
					trenode->left = onTheLine(trenode->left, point);
					trenode->left->parrent = trenode;
				}
				if (middlesector == 0) {
					trenode->middle = onTheLine(trenode->middle, point);
					trenode->middle->parrent = trenode;
				}
				if (rightsector == 0){
					trenode->right = onTheLine(trenode->right, point);
					trenode->right->parrent = trenode;	
				}
			}
			else {
				trenode->left = addPointToTree(trenode->left, point);
				trenode->left->parrent = trenode;
			}
			return trenode;
			
			
		}
	}

	/*
	Takes all points not in the convexhull and inserts them in the tree
	*/
	root* tringleSoup(root* btree) {
		for (int i = 0; i < insideconvexhull.size();  i++) {
			glm::vec2 point = insideconvexhull.at(i);
			btree->child = addPointToTree(btree->child, point);
		}
		return btree;
	}


	/*
	https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points

	gets the distance between point and a linne between two other points  
	*/
	float distanceToLine(glm::vec2 linepoint1, glm::vec2 linepoint2, glm::vec2 point) {
		float numeratorNotAbs = ((linepoint2.y - linepoint1.y) * point.x) - ((linepoint2.x - linepoint1.x) * point.y) + (linepoint2.x*linepoint1.y) - (linepoint2.y*linepoint1.x);
		float numerator = std::abs(numeratorNotAbs); 
		float denominatorNotSqrt = (linepoint2.y - linepoint1.y)*(linepoint2.y - linepoint1.y) + (linepoint2.x - linepoint1.x)*(linepoint2.x - linepoint1.x);
		float denominator = std::sqrtf(denominatorNotSqrt);

		float  dist = numerator / denominator;
		return dist;
	}


	/*
	selects three points and asigns them a colour then depending on the distance to 
	those points for a given vertex those colours become more or less bright

	it also takes the distance to the convexc hull and the closer ut is the more transperant
	*/
	std::vector<GLfloat> colourTriangles(std::vector<glm::vec2>convexhulllocal, std::vector<glm::vec2>allpoints) {
		glm::vec2 red;
		glm::vec2 green;
		glm::vec2 blue;
		std::vector<GLfloat> colouredList;
		std::vector<glm::vec2> insideconvexhulllocal;
		for (int i = 0; i < allpoints.size(); i++) {
			if (std::find(convexhulllocal.begin(), convexhulllocal.end(), allpoints.at(i)) != convexhulllocal.end()) {
				continue;
			}
			else {
				insideconvexhulllocal.push_back(allpoints.at(i));
			}
		}
		std::sort(insideconvexhulllocal.begin(), insideconvexhulllocal.end(), &MyVec3ComparisonFunc);
		int inconhullsize = insideconvexhulllocal.size();
		if (inconhullsize > 2) {
			red = insideconvexhulllocal.at(0);
			green = insideconvexhulllocal.at(inconhullsize / 3);
			blue = insideconvexhulllocal.at((2*inconhullsize)/3);
		}
		else if (inconhullsize > 1) {
			red = convexhulllocal.at(0);
			green = insideconvexhulllocal.at(0);
			blue = insideconvexhulllocal.at(1);
		}
		else if (inconhullsize == 1) {
			red = convexhulllocal.at(0);
			green = convexhulllocal.at(convexhulllocal.size()/2);
			blue = insideconvexhulllocal.at(0);
		}
		else {
			red = convexhulllocal.at(0);
			green = convexhulllocal.at(convexhulllocal.size() / 3);
			blue = convexhulllocal.at((2*convexhulllocal.size()) / 3);
		}

		float maxDistance = 0;
		for (int i = 0; i < allpoints.size(); i++) {
			float minDistance = 100;
			for (int j = 0; j < convexhulllocal.size(); j++) {
				int indexpoint1 = j; 
				int indexpoint2 = (j + 1) % convexhulllocal.size();

				float distance = distanceToLine(convexhulllocal.at(indexpoint1), convexhulllocal.at(indexpoint2), allpoints.at(i));
				if (distance < minDistance) {
					minDistance = distance;

				}

			}
			if (minDistance > maxDistance) {
				maxDistance = minDistance;
			}

			float reddist = glm::distance(red, allpoints.at(i));
			float greendist = glm::distance(green, allpoints.at(i));
			float bluedist = glm::distance(blue, allpoints.at(i));

			float totdist = reddist + greendist + bluedist;

			std::vector<GLfloat> coloredpoint = {
				allpoints.at(i).x, allpoints.at(i).y, -1,
				reddist / totdist, greendist / totdist, bluedist / totdist, minDistance,
			};
			colouredList.insert(colouredList.end(), coloredpoint.begin(), coloredpoint.end());
		}

		for (int i = 0; i < (colouredList.size() / 7); i++) {
			colouredList.at(((i + 1) * 7)-1) = colouredList.at(((i + 1) * 7)-1) / maxDistance;
		}
		return colouredList;
	}






	/*
	returns the index for  for vertexes 
	*/
	std::vector<unsigned int> getindex(std::vector<glm::vec2> vertexbuffer, std::vector<glm::vec2> vector) {
		std::vector<unsigned int> res;
		for (int i = 0; i < vector.size(); i++) {
			auto it = std::find(vertexbuffer.begin(), vertexbuffer.end(), vector.at(i));
			if (it != vertexbuffer.end())
			{
			unsigned int index = std::distance(vertexbuffer.begin(), it);
			res.push_back(index);
			}
		}
		return res;
		

	}


	/**
	peovides the angle for vertexshader to move the points
	*/
	void rotate(GLuint programcurr, GLfloat angle2) {
		glUseProgram(programcurr);
		GLuint Location = glGetUniformLocation(programcurr, "angle");

		glUniform1f(Location, angle2);
		glUseProgram(0);
	}

	void stopmovement(GLuint programcurr, GLboolean boolean) {
		glUseProgram(programcurr);
		GLuint Location = glGetUniformLocation(programcurr, "foo");

		glUniform1i(Location, boolean);
		glUseProgram(0);
	}

	bool
		ExampleApp::Open()
	{
		App::Open();
		this->window = new Display::Window;
		//this->window->SetSize(1200, 1200);
		window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32)
		{
			if ((GLFW_KEY_UP == key) && (action == GLFW_RELEASE)) {
				if (rect < 7) {
					rect++;
				}
			}
			else if ((GLFW_KEY_DOWN == key) && (action == GLFW_RELEASE)) {
				if (rect > 1) {
					rect--;
				}
			}
			else if ((GLFW_KEY_E == key) && (action == GLFW_RELEASE)) {
				//lineBool = !lineBool;
			}
			else if ((GLFW_KEY_F == key) && (action == GLFW_RELEASE)) {
				//fillBool = !fillBool;
			}
			else if ((GLFW_KEY_ESCAPE == key) && (action == GLFW_RELEASE)) {
				this->window->Close();
			}
		});

		/*if (movepointsbool) {
			rotate(this->program, (angle2));
		}
*/
		//readfromfile();
		if (pointsBool == true) {
			//kochSnowFlake = kochSnowFlakeLine(rect);
			//kochSnowFlake = readfromfile();
			pointsarr = readfromfile();

			allPointsVec = glfloatToVec2(pointsarr);

			pointsindex = getindex(allPointsVec, allPointsVec);

		}
		if (convexhullbool == true) {
			std::vector<glm::vec2> points = convexhull(allPointsVec);
			convexhullarr = vec2ToGlfloat(points);
			convexhullindex = getindex(allPointsVec, points);
		}
		if (fantriangulationbool == true) {
			auto allpoints = glfloatToVec2(pointsarr);
			auto convexpoints = glfloatToVec2(convexhullarr);
			fantriangluation = fanTriangulation(allpoints, convexpoints);

			auto trinaglesfan = getfromroot(fantriangluation);
			auto linestrianglefan = getLinesFromRoot(fantriangluation);

			fantra = vec2ToGlfloat(trinaglesfan);
			fantraindex = getindex(allPointsVec, trinaglesfan);
			fantraLines = vec2ToGlfloat(linestrianglefan);
			fantraLinesindex = getindex(allPointsVec, linestrianglefan);

			if (trianglesoupbool == true) {
				fantriangluation = tringleSoup(fantriangluation);
				auto trinaglesfan2 = getfromroot(fantriangluation);
				fantra = vec2ToGlfloat(trinaglesfan2);
				fantraindex = getindex(allPointsVec, trinaglesfan2);
				auto linestrianglefan = getLinesFromRoot(fantriangluation);
				fantraLines = vec2ToGlfloat(linestrianglefan);
				fantraLinesindex = getindex(allPointsVec, linestrianglefan);
			}
		}

		
		if (pointsBool) {
			GLfloat* buf = &pointsarr[0];
			bufferSize = (pointsarr.size());
			bufindex = &pointsindex[0];

		}
		
		if (convexhullbool) {

			buf2 = &convexhullarr[0];
			bufferSize2 = (convexhullarr.size());

			buf2index = &convexhullindex[0];
		}
		if (fantriangulationbool) {
			buf3 = &fantra[0];
			bufferSize3 = (fantra.size());

			buf3index = &fantraindex[0];

			buf4index = &fantraLinesindex[0];

			bufferSize4 = (fantraLines.size());
		}
		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


			this->vsBuffer = new GLchar[STRING_BUFFER_SIZE];
			this->fsBuffer = new GLchar[STRING_BUFFER_SIZE];

			// copy the hardcoded shader into buffer
			strncpy_s(this->vsBuffer, STRING_BUFFER_SIZE, vs, STRING_BUFFER_SIZE);
			strncpy_s(this->fsBuffer, STRING_BUFFER_SIZE, ps, STRING_BUFFER_SIZE);

			// setup vertex shader
			this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLint length = (GLint)std::strlen(vs);
			glShaderSource(this->vertexShader, 1, &vs, &length);
			glCompileShader(this->vertexShader);

			// get error log
			GLint shaderLogSize;
			GLint InfoLogLength;
			glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (shaderLogSize > 0)
			{
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}
			if (InfoLogLength > 0) {
				std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(this->vertexShader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				printf("[info log length part]%s\n", &VertexShaderErrorMessage[0]);
			}
			// setup pixel shader
			this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			length = (GLint)std::strlen(ps);
			glShaderSource(this->pixelShader, 1, &ps, &length);
			glCompileShader(this->pixelShader);

			// get error log
			shaderLogSize;
			glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0)
			{
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// create a program object
			this->program = glCreateProgram();
			glAttachShader(this->program, this->vertexShader);
			glAttachShader(this->program, this->pixelShader);
			glLinkProgram(this->program);
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0)
			{
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// setup vbo
			glGenBuffers(1, &this->triangle);
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pointsarr.size(), buf, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window->SetUiRender([this]()
			{
				this->RenderUI();
			});

			return true;
		}
		return false;

	}




	//------------------------------------------------------------------------------
	/**
	*/

	/*void glfunc(GLuint thing) {

	}*/
	void
		ExampleApp::Run()
	{
		while (this->window->IsOpen())
		{
			glLineWidth(3);

			
			/*if (pointsBool) {
				auto pointsvec = glfloatToVec2(pointsarr);
				pointsvec = movepoints(pointsvec);
				pointsarr = vec2ToGlfloat(pointsvec);
				allPointsVec = glfloatToVec2(pointsarr);
				pointsindex = getindex(allPointsVec, allPointsVec);
			}
			
		
			if (convexhullbool == true) {
				std::vector<glm::vec2> points = convexhull(allPointsVec);
				convexhullarr = vec2ToGlfloat(points);
				convexhullindex = getindex(allPointsVec, points);
	
			}
			if (fantriangulationbool == true) {
				auto allpoints = glfloatToVec2(pointsarr);
				auto convexpoints = glfloatToVec2(convexhullarr);
				root* jda = fanTriangulation(allpoints, convexpoints);
	
				auto trinaglesfan = getfromroot(jda);
				auto linestrianglefan = getLinesFromRoot(jda);
	
				fantra = vec2ToGlfloat(trinaglesfan);
				fantraindex = getindex(allPointsVec, trinaglesfan);
				fantraLines = vec2ToGlfloat(linestrianglefan);
				fantraLinesindex = getindex(allPointsVec, linestrianglefan);
	
				if (trianglesoupbool) {
					jda = tringleSoup(jda);
					auto trinaglesfan2 = getfromroot(jda);
					auto linestrianglefan = getLinesFromRoot(jda);
					fantra = vec2ToGlfloat(trinaglesfan2);
					fantraindex = getindex(allPointsVec, trinaglesfan);
					fantraLines = vec2ToGlfloat(linestrianglefan);
					fantraLinesindex = getindex(allPointsVec, linestrianglefan);
				}
			}
			*/
			if (pointsBool) {
				buf = &pointsarr[0];
				bufferSize = (pointsarr.size());


				bufindex = &pointsindex[0];

				auto test = glfloatToVec2(pointsarr);
				std::cout << "/n size of points: " << test.size() << std::endl;
				for (int i = 0; i < test.size(); i++) {
					std::cout << "points loop:  " << i << " x: " << test.at(i).x << " y:  " << test.at(i).y << std::endl;
				}
			}
			if (convexhullbool) {
				buf2 = &convexhullarr[0];
				bufferSize2 = (convexhullarr.size());

				buf2index = &convexhullindex[0];

				auto test2 = glfloatToVec2(convexhullarr);
				std::cout << "/n size of convexhull " << test2.size() << std::endl;
				for (int i = 0; i < test2.size(); i++) {
					std::cout << "conhul loop:  " << i << " x: " << test2.at(i).x << " y:  " << test2.at(i).y << std::endl;
				}

			}
			if (fantriangulationbool) {
				buf3 = &fantra[0];
				bufferSize3 = (fantra.size());


				buf3index = &fantraindex[0];

				auto test3 = glfloatToVec2(fantra);
				std::cout << "/n size of fantriangulation: " << test3.size() << std::endl;
				for (int i = 0; i < test3.size(); i++) {
					std::cout << "fantri loop:  " << i << " x: " << test3.at(i).x << " y:  " << test3.at(i).y << std::endl;
				}
				buf4 = &fantraLines[0];
				bufferSize4 = (fantraLines.size());

				buf4index = &fantraLinesindex[0];
			
			}
			glClear(GL_COLOR_BUFFER_BIT);

			this->window->Update();

			if (movepointsbool) {
				rotate(this->program, (angle2));
			}





			//Draw lineloop 
			if (pointsBool == true) {
				glGenBuffers(1, &this->triangle);
				glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pointsarr.size(), buf, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				volatile Dbg* ptr = (Dbg*)&pointsarr[0];

				glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
				glUseProgram(this->program);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));


				unsigned int ibo;
				glGenBuffers(1, &ibo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointsindex.size() * sizeof(unsigned int), bufindex, GL_STATIC_DRAW);
		
				glPointSize(5.0f);
				glDrawElements(GL_POINTS, pointsindex.size(), GL_UNSIGNED_INT, nullptr);
				
			}
			if (convexhullbool == true) {
					glGenBuffers(1, &this->triangle);
					glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
					glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pointsarr.size(), buf, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
					glUseProgram(this->program);

					glEnableVertexAttribArray(0);
					glEnableVertexAttribArray(1);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
					glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));


					unsigned int ibo2;
					glGenBuffers(1, &ibo2);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, convexhullindex.size() * sizeof(unsigned int), buf2index, GL_STATIC_DRAW);

					//glPointSize(5.0f);
					glDrawElements(GL_LINE_LOOP, convexhullindex.size(), GL_UNSIGNED_INT, nullptr);

	
			}
			if (fantriangulationbool){
				glGenBuffers(1, &this->triangle);
				glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pointsarr.size(), buf, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
				glUseProgram(this->program);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));



				unsigned int ibo3;
				glGenBuffers(1, &ibo3);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, fantraindex.size() * sizeof(unsigned int), buf3index, GL_STATIC_DRAW);

				//glPointSize(5.0f);
				glDrawElements(GL_TRIANGLES, fantraindex.size(), GL_UNSIGNED_INT, nullptr);

				glGenBuffers(1, &this->triangle);
				glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pointsarr.size(), buf, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
				glUseProgram(this->program);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));



				unsigned int ibo4;
				glGenBuffers(1, &ibo4);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo4);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, fantraLinesindex.size() * sizeof(unsigned int), buf4index, GL_STATIC_DRAW);

				//glPointSize(5.0f);
				glDrawElements(GL_LINES, fantraLinesindex.size(), GL_UNSIGNED_INT, nullptr);

			}


			angle2 = angle2 + 0.01;

			this->window->SwapBuffers();
		}
	}

	void
		ExampleApp::RenderUI()
	{
		if (this->window->IsOpen())
		{
			bool show = true;
			// create a new window
			ImGui::Begin("Shader Sources", &show, ImGuiWindowFlags_NoSavedSettings);

			// apply button
			if (ImGui::Button("read from file"))
			{
				pointsarr = readfromfile();
				allPointsVec = glfloatToVec2(pointsarr);
				pointsindex = getindex(allPointsVec, allPointsVec);
			}
			if (ImGui::Button("read from file2"))
			{
				pointsarr = readfromfile2();
				allPointsVec = glfloatToVec2(pointsarr);
				pointsindex = getindex(allPointsVec, allPointsVec);
			}
			if (ImGui::Button("random points"))
			{
				pointsarr = randomPoints(rect);
				allPointsVec = glfloatToVec2(pointsarr);
				pointsindex = getindex(allPointsVec, allPointsVec);
			}
			if (ImGui::Button("convexhull"))
			{
				//if pressed boolean cthat controls to fill triangles is toggled
				std::vector<glm::vec2> points = glfloatToVec2(pointsarr);
				points = convexhull(points);
				convexhullarr = vec2ToGlfloat(points);
				convexhullindex = getindex(allPointsVec, points);
			}
			if (ImGui::Button("fantrianglulation"))
			{
				//if pressed boolean cthat controls to draw line is toggled
				auto allpoints = glfloatToVec2(pointsarr);
				auto convexpoints = glfloatToVec2(convexhullarr);
				fantriangluation = fanTriangulation(allpoints, convexpoints);

				auto trinaglesfan = getfromroot(fantriangluation);
				fantra = vec2ToGlfloat(trinaglesfan);

				fantraindex = getindex(allPointsVec, trinaglesfan);

				auto trinaglesfanLines = getLinesFromRoot(fantriangluation);
				fantraLinesindex = getindex(allPointsVec, trinaglesfanLines);
				fantraLines = vec2ToGlfloat(trinaglesfanLines);



			}
			if (ImGui::Button("trianglesoup"))
			{
				//if pressed boolean cthat controls to draw line is toggled
				fantriangluation = tringleSoup(fantriangluation);
				auto trinaglesfan = getfromroot(fantriangluation);

				fantraindex = getindex(allPointsVec, trinaglesfan);
				fantra = vec2ToGlfloat(trinaglesfan);
				auto trinaglesfanLines = getLinesFromRoot(fantriangluation);
				fantraLinesindex = getindex(allPointsVec, trinaglesfanLines);
				fantraLines = vec2ToGlfloat(trinaglesfanLines);
			}
			if (ImGui::Button("fancy colour"))
			{
				//if pressed boolean cthat controls to draw line is toggled

				//auto fantrafvec = glfloatToVec2(fantra);
				//auto fantraLinesvec = glfloatToVec2(fantraLines);
				auto convexvec = glfloatToVec2(convexhullarr);
				//convexhullarr = colourTriangles(convexvec, convexvec);
				//fantra = colourTriangles(convexvec, fantrafvec);
				//fantraLines = colourTriangles(convexvec, fantraLinesvec);
				pointsarr = colourTriangles(convexvec, allPointsVec);
				

			}
			if (ImGui::Button("red colour"))
			{
				pointsarr = vec2ToGlfloat(allPointsVec);
			}
			if (ImGui::Button("show points"))
			{
				//if pressed boolean cthat controls to fill triangles is toggled
				pointsBool = !pointsBool;
			}
			if (ImGui::Button("show convexhull"))
			{
				//if pressed boolean cthat controls to fill triangles is toggled
				convexhullbool = !convexhullbool;
			}
			if (ImGui::Button("show fantriangulation"))
			{
				//if pressed boolean cthat controls to fill triangles is toggled
				fantriangulationbool = !fantriangulationbool;
			}
			if (ImGui::Button("show trianglesoup"))
			{
				//if pressed boolean cthat controls rotation is toggled
				trianglesoupbool = !trianglesoupbool;
			}
			if (ImGui::Button("move points"))
			{
				//if pressed boolean cthat controls rotation is toggled
				movepointsbool = !movepointsbool;
				if (!movepointsbool) {
					rotate(this->program, 0);
					stopmovement(this->program, false);
				}
				else {
					stopmovement(this->program, true);
				}
			}
			ImGui::SliderInt("number of random points", &rect, 1, 100);

			if (this->compilerLog.length())
			{
				// if compilation produced any output we display it here
				ImGui::TextWrapped(this->compilerLog.c_str());
			}
			// close window
			ImGui::End();
		}
	}

}