#include<bits/stdc++.h>
using namespace std;

#define UNDEF 0
#define NOISE -1

template<class t = float>
struct point {
    t x,y,z = 0;
    int clusterNo = UNDEF;

    double dist_from(point<t> pt) {
        return sqrt(
        pow(x - pt.x, 2)+
        pow(y - pt.y, 2)+
        pow(z - pt.z, 2));
    }

    point(t x,t y,t z=0) : x(x), y(y), z(z) {}
};


/* 
 * takes the input of points from file provided
 * @param points: vector of points to be stored in
 * @param inputFile: path of input file with name
 */
template<class T>
void take_points_from(vector<point<T>>& points, string inputFile) {
    int i=0;
    ifstream fin;
    fin.open(inputFile);
    while (!fin.eof()) {
        T x,y,gar; fin>>gar;
        fin >> x >> y;
        point<T> pt(x,y);
        points.push_back(pt);
    }
    fin.close();
}

template<class T,class B>
vector<int> range_query(vector<point<T>> db, int pt,B eps) {
    vector<int> neighbour;
    point<T> point = db[pt];
    for(int i=0;i<db.size();i++) {
        if(point.dist_from(db[i]) <= eps) neighbour.push_back(i);
    }
    return neighbour;
}

template<class T,class B>
int DBSCAN(vector<point<T>>& db,B eps,int minPts) {
    int c=0;
    for(int ptIndx = 0; ptIndx<db.size(); ptIndx++) {
        if(db[ptIndx].clusterNo!=UNDEF) continue;
        vector<int> neighbour = range_query(db,ptIndx,eps);
        if(neighbour.size()<minPts) {
            db[ptIndx].clusterNo = NOISE;
            continue;
        }
        c++;
        vector<bool> visited(db.size(),false);
        for(auto x:neighbour) visited[x] = true;
        vector<int> seedSet = neighbour;
        for(int i=0;i<seedSet.size();i++) {
            if(db[seedSet[i]].clusterNo == NOISE) db[seedSet[i]].clusterNo = c;
            if(db[seedSet[i]].clusterNo != UNDEF) continue;
            db[seedSet[i]].clusterNo = c;
            neighbour = range_query(db,seedSet[i],eps);
            if(neighbour.size() >= minPts) {
                for(auto x:neighbour) {
                    if(visited[x]==false) {
                        seedSet.push_back(x);
                        visited[x] = true;
                    }
                }
            }
        }
    }

    return c;
}

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
int main() {
    int n = 100;
    vector<point<>> points;
    string inputPath = "input1.txt";
    take_points_from(points,inputPath);

    vector<float> x,y,z;
    for(auto pt:points) {
        x.push_back(pt.x);
        y.push_back(pt.y);
        z.push_back(pt.z);
    }
    plt::figure(1);
    plt::scatter(x,y);

    plt::save("images/dataset.png");
    plt::show();
    int total = DBSCAN(points,10,5);
    cout<<"number of Clusters: "<<total<<endl;
    ofstream fout("output.txt");
    for(auto pt:points) {
        fout<<"CLUSTER NO:"<<pt.clusterNo<<"\t\t"<<pt.x<<"\t\t"<<pt.y<<"\t\t"<<pt.z<<endl;
    }
    fout.close();
    
    vector<vector<point<>>> clusters(total+2);
    for(auto pt:points) {
        clusters[pt.clusterNo+1].push_back(pt);
    }
    plt::figure(2);
    for(auto cluster:clusters) {
        cout<<cluster.size()<<endl;
        x.clear();y.clear();
        for(auto pt:cluster) {
            x.push_back(pt.x);
            y.push_back(pt.y);
        }
        plt::scatter(x,y);
    }
    plt::save("images/cluster.png");
    plt::show();

    return 1;
}