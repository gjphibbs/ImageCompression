// File:        pa3.cpp
// Author:      cs221 instructors
// Date:        2021-03-03
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets

#include "sqtree.h"
#include "stats.h"

int main() {

  // read in image
  PNG orig;
  // orig.readFromFile("images/orig/small1.png");
  
  // orig.readFromFile("images/orig/small2.png");
  // orig.readFromFile("images/orig/pacman.png");
  // orig.readFromFile("images/orig/huckleberry_hound.png");
  // orig.readFromFile("images/orig/rocky.png");
  // orig.readFromFile("images/orig/rocky_bullwinkle.png");
  orig.readFromFile("images/orig/colour.png");
  // orig.readFromFile("images/orig/klee-flora-on-sand.png");
  // orig.readFromFile("images/orig/stanley-totem-poles.png");
 
  
  // use it to build a sqtree (try other tolerances)
  SQtree t(orig, 200000.0);

  // copy the tree (to show you can)
  SQtree tCopy(t);

  // render the sqtree
  PNG pic = tCopy.render();

  cout << t.size() << endl;
 
  // write it out
  pic.writeToFile("images/out/stan1.png");

  // stats stat = stats(orig);
  // cout << stat.getAvg(make_pair(0, 0), orig.width(), orig.height()) << endl;
  // cout << stat.getVar(make_pair(0, 0), orig.width(), orig.height()) << endl;
  return 0;
}
