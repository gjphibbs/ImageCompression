/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */
#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.
  int w = imIn.width();
  int h = imIn.height();
  stats imStats = stats(imIn);
  pair<int, int> temp;
  temp.first = 0;
  temp.second = 0;
  root = buildTree(imStats, temp, w, h, tol);
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  // Your code here.
  Node* newNode = new Node(s, ul, w, h);
  if (w == 1 && h == 1) {
    return newNode;
  }

  if (newNode->var < tol) {
    return newNode;
  }
  
  double minMax = -1;
  double curVar;
  pair<int, int> insUL;
  pair<int, int> bestUL;
  int bestWidth1 = 0;
  int bestWidth2 = 0;
  int bestHeight1 = 0;
  int bestHeight2 = 0;

  int w1 = 0;
  int w2 = 0;
  int h1 = 0;
  int h2 = 0;
 
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      if (i == 0 && j == 0) {
        continue;
      }

      insUL.first = ul.first + i;
      insUL.second = ul.second + j;

      w1 = i;
      w2 = w - i;
      h1 = j;
      h2 = h - j;

      if (i == 0) {
        curVar = max(s.getVar(ul, w, h1), s.getVar(insUL, w, h2));
      } else if (j == 0) {
        curVar = max(s.getVar(ul, w1, h), s.getVar(insUL, w2, h));
      } else {
        pair<int, int> temp1 = make_pair(insUL.first, ul.second);
        pair<int, int> temp2 = make_pair(ul.first, insUL.second);
        curVar = max(s.getVar(ul, w1, h1), s.getVar(temp1, w2, h1));
        double curVar2 = max(s.getVar(temp2, w1, h2), s.getVar(insUL, w2, h2));
        curVar = max(curVar, curVar2);
      }

      if (curVar < minMax || minMax == -1) {

      bestUL = insUL;
      bestWidth1 = w1;
      bestWidth2 = w2;
      bestHeight1 = h1;
      bestHeight2 = h2;

      minMax = curVar;
      }
    }
  }

  if (bestHeight1 == 0) {
    newNode->NW = buildTree(s, ul, bestWidth1, bestHeight2, tol);
    newNode->NE = buildTree(s, bestUL, bestWidth2, bestHeight2, tol);
  } else if (bestWidth1 == 0) {
    newNode->NW = buildTree(s, ul, bestWidth2, bestHeight1, tol);
    newNode->SE = buildTree(s, bestUL, bestWidth2, bestHeight2, tol);
  } else {
    pair<int, int> temp1 = make_pair(bestUL.first, ul.second);
    pair<int, int> temp2 = make_pair(ul.first, bestUL.second);

    newNode->NE = buildTree(s, temp1, bestWidth2, bestHeight1, tol);
    newNode->NW = buildTree(s, ul, bestWidth1, bestHeight1, tol);
    newNode->SW = buildTree(s, temp2, bestWidth1, bestHeight2, tol);
    newNode->SE = buildTree(s, bestUL, bestWidth2, bestHeight2, tol);
  }

  return newNode;
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  // Your code here.
  PNG png = PNG(root->width, root->height);
  PNG * pngP = & png;
  recursiveRender(root, pngP);
  return png;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
  recursiveClear(root);
}

void SQtree::copy(const SQtree & other) {
  // Your code here.
  root = recursiveCopy(other.root);
}

int SQtree::size() {
  return recursiveSize(root);
}

void SQtree::recursiveRender(Node * tRoot, PNG * p) {
  if (tRoot == NULL) {
    return;
  }
  if (tRoot->NW == NULL && tRoot->NE == NULL && tRoot->SE == NULL && tRoot->SW == NULL) {
    for (int i = tRoot->upLeft.first; i < tRoot->width + tRoot->upLeft.first; i++) {
      for (int j = tRoot->upLeft.second; j < tRoot->height + tRoot->upLeft.second; j++) {
        RGBAPixel* tempP = p->getPixel(i, j);
        *tempP = tRoot->avg;
      }
    }
  }
  recursiveRender(tRoot->NW, p);
  recursiveRender(tRoot->NE, p);
  recursiveRender(tRoot->SE, p);
  recursiveRender(tRoot->SW, p);
}

void SQtree::recursiveClear(Node * tRoot) {
  if (tRoot == NULL) {
    return;
  }
  recursiveClear(tRoot->NW);
  recursiveClear(tRoot->NE);
  recursiveClear(tRoot->SW);
  recursiveClear(tRoot->SE);
  delete tRoot;
  tRoot = NULL;
}

SQtree::Node* SQtree::recursiveCopy(const Node* otherRoot) {
  if (otherRoot == NULL) {
    return NULL;
  }
  Node* node = new Node(otherRoot->upLeft, otherRoot->width, otherRoot->height, otherRoot->avg, otherRoot->var);
  node->NW = recursiveCopy(otherRoot->NW);
  node->NE = recursiveCopy(otherRoot->NE);
  node->SE = recursiveCopy(otherRoot->SE);
  node->SW = recursiveCopy(otherRoot->SW);
  return node;
}

int SQtree::recursiveSize(const Node* mRoot) {
  if (mRoot == NULL) {
    return 0;
  }

  return 1 + recursiveSize(mRoot->NW) + recursiveSize(mRoot->NE) + recursiveSize(mRoot->SE) + recursiveSize(mRoot->SW);
}

