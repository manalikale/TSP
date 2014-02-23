#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <cassert>
#include <queue>
#include <algorithm>


struct point {
    double x, y;
    std::string name;
    bool isVisited;
    double g, f;

    point() {
        g = f = 1e10;
        isVisited = false;
    }
};

std::vector<point> points;

inline double dist(point p1, point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

struct State {
    int idx;
    std::vector<int> visitedPoints;
    double g;
    double f;

    bool operator>(const State& oth) const {
        return f > oth.f;
    }
};


inline double getHeuristic(std::string task, int currentIdx, int startingIdx, State st) {
    if (task == "2") {
        return dist(points[currentIdx], points[startingIdx]);
    } else {
        //constructing spanning tree for unvisted points and starting point
        std::vector<int> used(points.size(), 0);
        double res = 0;
        used[startingIdx] = 1;

        while (true) {
            int x = -1;
            int y = -1;
            double distance = 1e10;
    
            for (int i = 0; i < used.size(); ++i) {
                if ((i != startingIdx) && (std::find(st.visitedPoints.begin(), st.visitedPoints.end(), i) != st.visitedPoints.end()) || !used[i]) {
                    continue;
                }
                for (int j = 0; j < used.size(); ++j) {
                    if ((j != startingIdx) && (std::find(st.visitedPoints.begin(), st.visitedPoints.end(), j) != st.visitedPoints.end()) || used[j]) {
                        continue;
                    }
                    if (dist(points[i], points[j]) < distance) {
                        distance = dist(points[i], points[j]);
                        x = i;
                        y = j;
                    }
                }
            }
            if (x == -1) {
                break;
            }
            used[y] = true;
            res += distance;
        }
        return res;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 11) {
        std::cerr << "Error: Usage is \"tsp -t <task> -s <start_node> -i <input_file> -op <output_path> -ol <output_log>\"\n";
        exit(7);
    }

    std::string task = argv[2];
    std::string startNode = argv[4];
    std::string inputFile = argv[6];
    std::string outputPath = argv[8];
    std::string outputLog = argv[10];

    std::ifstream fin(inputFile.c_str());
    if (!fin) {
        std::cerr << "Error: input file can't be opened\n";
        exit(7);
    }


    while (true) {
        point p;
        char c;
        fin >> c;
        p.name = std::string(1, c);
        if (fin.eof()) {
            break;
        }
        fin.ignore();
        fin >> p.x;
        fin.ignore();
        fin >> p.y;

        points.push_back(p);
    }

    int startingIdx = -1;
    for (int i = 0; i < points.size(); ++i) {
        if (points[i].name == startNode) {
            startingIdx = i;
            break;
        }
    }
    if (startingIdx == -1) {
        std::cerr << "Error: Start node doesn't exist in graph\n";
        exit(7);
    }


    std::ofstream foutputPath(outputPath.c_str());
    foutputPath.precision(14);
    std::ofstream foutputLog(outputLog.c_str());
    foutputLog.precision(14);

    int currentIdx = startingIdx;
    double totalCost = 0;
    std::string tour = points[startingIdx].name;

    if (task == "1") {
        points[currentIdx].isVisited = true;
        foutputPath << points[currentIdx].name << std::endl;

        for (int k = 0; k < points.size() - 1; ++k) {
            int nextNode = -1;
            double distSoFor = 1e10;
            for (int e = 0; e < points.size(); ++e) {
                if (points[e].isVisited) {
                    continue;
                }
                if (dist(points[currentIdx], points[e]) < distSoFor) {
                    distSoFor = dist(points[currentIdx], points[e]);
                    nextNode = e;
                }
            }
            assert(nextNode != -1);
            totalCost += distSoFor;
            currentIdx = nextNode;
            points[currentIdx].isVisited = true;
            tour += points[currentIdx].name;

            foutputPath << points[currentIdx].name << std::endl;
            foutputLog << tour << "," << std::fixed << totalCost << ",0," << std::fixed << totalCost << std::endl;
        }
        totalCost += dist(points[currentIdx], points[startingIdx]);
        tour += points[startingIdx].name;

        foutputPath << points[startingIdx].name << std::endl;
        foutputPath << "Total Tour Cost: " << std::fixed << totalCost << std::endl;

        foutputLog << tour << "," << std::fixed << totalCost << ",0," << std::fixed << totalCost << std::endl;
    } else if (task == "2" || task == "3") {
        std::priority_queue<State, std::vector<State>, std::greater<State> > pQueue;
        State st;
        st.idx = startingIdx;
        st.visitedPoints.push_back(st.idx);
        st.g = st.f = 0;

        pQueue.push(st);

        while (!pQueue.empty()) {
            State st = pQueue.top();
            pQueue.pop();

            for (int i = 0; i < st.visitedPoints.size(); ++i) {
                foutputLog << points[st.visitedPoints[i]].name;
            }
            foutputLog << "," << std::fixed << st.g << "," << std::fixed << st.f - st.g<< "," << std::fixed << st.f << std::endl;
            if (st.visitedPoints.size() == points.size()) {
                for (int i = 0; i < st.visitedPoints.size(); ++i) {
                    foutputLog << points[st.visitedPoints[i]].name;
                    foutputPath << points[st.visitedPoints[i]].name << std::endl;
                }
                foutputLog << points[startingIdx].name << "," << std::fixed << st.f << "," << std::fixed << 0.0 << "," << std::fixed << st.f << std::endl;
                foutputPath << points[startingIdx].name << std::endl;
                foutputPath << "Total Tour Cost: " << std::fixed << st.f << std::endl;
                break;
            }

            for (int i = 0; i < points.size(); ++i) {
                double newG = st.g + dist(points[st.idx], points[i]);
                double newF = newG + getHeuristic(task, i, startingIdx, st);

                if (std::find(st.visitedPoints.begin(), st.visitedPoints.end(), i) == st.visitedPoints.end()) {
                    points[i].g = newG;
                    points[i].f = newF;

                    State newSt(st);
                    newSt.g = newG;
                    newSt.f = newF;
                    newSt.idx = i;
                    newSt.visitedPoints.push_back(i);
                    pQueue.push(newSt);
                }
            }
        }
    }  else {
        std::cerr << "Error: Task should be 1, 2 or 3.\n";
        exit(7);
    }
}



