#include "stats.h"
#include <stdio.h>
#include <string.h>
#include "stats.h"

stats::stats(PNG & im){
    sumRed = vector<vector<long>>(im.width() + 1);
    sumBlue = vector<vector<long>>(im.width() + 1);
    sumGreen = vector<vector<long>>(im.width() + 1);
    sumsqRed = vector<vector<long>>(im.width() + 1);
    sumsqBlue = vector<vector<long>>(im.width() + 1);
    sumsqGreen = vector<vector<long>>(im.width() + 1);
    vector<long> sumRedC(im.width() + 1);
    vector<long> sumBlueC(im.width() + 1);
    vector<long> sumGreenC(im.width() + 1);
    vector<long> sumRedsqC(im.width() + 1);
    vector<long> sumBluesqC(im.width() + 1);
    vector<long> sumGreensqC(im.width() + 1);

    for(unsigned int i = 0; i <= im.width(); i++){
        for(unsigned int j = 0; j <= im.height(); j++){
            if(i == 0) {
            sumRed[i].push_back(0);
            sumBlue[i].push_back(0);
            sumGreen[i].push_back(0);
            sumsqRed[i].push_back(0);
            sumsqBlue[i].push_back(0);
            sumsqGreen[i].push_back(0);
            continue;
            }
            if(j == 0) {
                sumRedC[i] = 0;
                sumBlueC[i] = 0;
                sumGreenC[i] = 0;
                sumRedsqC[i] = 0;
                sumBluesqC[i] = 0;
                sumGreensqC[i] = 0;
                sumRed[i] = vector<long>(im.height() + 1);
                sumBlue[i] = vector<long>(im.height() + 1);
                sumGreen[i] = vector<long>(im.height() + 1);
                sumsqRed[i] = vector<long>(im.height() + 1);
                sumsqBlue[i] = vector<long>(im.height() + 1);
                sumsqGreen[i] = vector<long>(im.height() + 1);
            } else {
                RGBAPixel *p = im.getPixel(i - 1, j - 1);
                sumRedC[i] += p->r;
                sumBlueC[i] += p->b;
                sumGreenC[i] += p->g;
                sumRedsqC[i] += (p->r) * (p->r);
                sumBluesqC[i] += (p->b) * (p->b);
                sumGreensqC[i] += (p->g) * (p->g);
            }
                sumRed[i][j] = (sumRed[i-1][j] + sumRedC[i]);
                sumBlue[i][j] = (sumBlue[i-1][j] + sumBlueC[i]);
                sumGreen[i][j] = (sumGreen[i-1][j] + sumGreenC[i]);
                sumsqRed[i][j] = (sumsqRed[i-1][j] + sumRedsqC[i]);
                sumsqBlue[i][j] = (sumsqBlue[i-1][j] + sumBluesqC[i]);
                sumsqGreen[i][j] = (sumsqGreen[i-1][j] + sumGreensqC[i]);
        }
    }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
    long retVal = 0;

    if (channel == 'r') {
        retVal = sumRed[ul.first + w][ul.second + h] 
        - sumRed[ul.first + w][ul.second] 
        - sumRed[ul.first][ul.second + h] 
        + sumRed[ul.first][ul.second];
    } else if (channel == 'g') {
        retVal = sumGreen[ul.first + w][ul.second + h] 
        - sumGreen[ul.first + w][ul.second] 
        - sumGreen[ul.first][ul.second + h] 
        + sumGreen[ul.first][ul.second];
    } else if (channel == 'b') {
        retVal = sumBlue[ul.first + w][ul.second + h] 
        - sumBlue[ul.first + w][ul.second] 
        - sumBlue[ul.first][ul.second + h] 
        + sumBlue[ul.first][ul.second];
    }

    return retVal;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
    long retVal = 0;

    if (channel == 'r') {
        retVal = sumsqRed[ul.first + w][ul.second + h] 
        - sumsqRed[ul.first + w][ul.second] 
        - sumsqRed[ul.first][ul.second + h] 
        + sumsqRed[ul.first][ul.second];
    } else if (channel == 'g') {
        retVal = sumsqGreen[ul.first + w][ul.second + h] 
        - sumsqGreen[ul.first + w][ul.second] 
        - sumsqGreen[ul.first][ul.second + h] 
        + sumsqGreen[ul.first][ul.second];
    } else if (channel == 'b') {
        retVal = sumsqBlue[ul.first + w][ul.second + h] 
        - sumsqBlue[ul.first + w][ul.second] 
        - sumsqBlue[ul.first][ul.second + h] 
        + sumsqBlue[ul.first][ul.second];
    }

    return retVal;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */
    double area = w * h;

    double r = getSumSq('r',ul,w,h) - ((getSum('r', ul,w,h) * (getSum('r', ul,w,h)))/(area));
    double g = getSumSq('g',ul,w,h) - ((getSum('g', ul,w,h) * (getSum('g', ul,w,h)))/(area));
    double b = getSumSq('b',ul,w,h) - ((getSum('b', ul,w,h) * (getSum('b', ul,w,h)))/(area));

    return r + g + b;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */
    long area = w * h;

    long r = getSum('r', ul, w, h)/area;
    long g = getSum('g', ul, w, h)/area;
    long b = getSum('b', ul, w, h)/area;

    return RGBAPixel(r,g,b);
}
