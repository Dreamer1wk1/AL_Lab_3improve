#include <bits/stdc++.h>

using namespace std;
const int N = 450;
const int colorNum = 5;
struct Node {
    int degree;//度
    int ableColor;//可选颜色数量
    bool colorArr[colorNum + 1];//可选的颜色
    bool isChange[N];//颜色状态是否被修改过,[pos]=True表示收到pos色块影响而改变状态
};
int grid[N][N];
Node temp;
vector<Node> node(N, temp);//存储每个节点
vector<int> isColor(N, 0);//0未涂色,1~N为颜色
vector<vector<int>> res;

////生成随机数据
//void generate() {
//    int start, end, range = 2000;// 边的数量,不至于太少导致解的数量过多，也不至于太多导致解无解
//    // 设置随机数引擎和分布
//    std::random_device rd;
//    std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
//    std::uniform_int_distribution<int> get(0, N - 1); // 生成 [0, N-1] 范围内的数
//    while (range--) {
//        start = get(rd);
//        end = get(rd);
//        grid[start][end] = 1, grid[end][start] = 1;
//        node[start].degree++;
//        node[end].degree++;
//    }
//}

//输出数据
void writeRes() {
    // 打开文件，如果不存在则创建，如果存在则覆盖原有内容
    ofstream outfile("le450_5a.col.out");
    if (!outfile) {
        cout << "无法打开文件" << endl;
        return;
    }
    // 向文件写入数据
    for (auto it: res[0]) {
        outfile << it << endl;
    }
    // 关闭文件
    outfile.close();
}

//读入提供的数据
void readIn() {
//    "le450_25a.col"
//    "le450_15b.col"
//    "le450_5a.col"
//    "smallData.txt"
//    "le100_4_random.txt"
//    "le200_4_random.txt"
//    "le300_4_random.txt"
//    "le400_4_random.txt"
    ifstream file("le450_5a.col"); // 打开文件
    if (!file.is_open()) {
        cout << "无法打开文件" << endl;
        return;
    }
    string line;
    int start, end;
    char ch;
    // 循环读取文件中的每一行
    while (getline(file, line)) {
        if (line.empty()) {
            continue; // 跳过空行
        }
        if (line[0] != 'e') {
            continue; // 跳过开头行
        }
        istringstream iss(line); // 将当前行转换为字符串流
        if (iss >> ch >> start >> end) { // 从字符串流中读取两个数字
            start--, end--;
            grid[start][end] = 1, grid[end][start] = 1;
            node[start].degree++;
            node[end].degree++;
        }
    }
    file.close(); // 关闭文件
}

//检查答案是否正确
bool checkRes() {
    for (auto path: res) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i != j && grid[i][j] && path[i] == path[j]) return false;
            }
        }
    }
    return true;
}

void initiate() {
    for (int i = 0; i < N; i++) {
        node[i].degree = 0;
        node[i].ableColor = colorNum;
        memset(node[i].colorArr, true, sizeof node[i].colorArr);
        node[i].colorArr[0] = false;
        memset(node[i].isChange, false, sizeof node[i].isChange);
    }
    readIn();
//    generate();
}

bool check(int pos, int color) {
    for (int i = 0; i < N; i++) {
        if (i != pos && grid[i][pos]) {
            //没涂色的相邻节点只剩color
            if (!isColor[i] && node[i].ableColor == 1 && node[i].colorArr[color]) {
                return false;
            }
        }
    }
    return true;
}

//优先选择可选颜色最少的，相同情况下选度最大的
int getNextNode() {
    int index = 0, minColor = colorNum;
    for (int i = 0; i < N; i++) {
        if (isColor[i] == 0) {
            //可选颜色=最小
            if (node[i].ableColor == minColor) {
                //选择度最大的
                if (node[i].degree >= node[index].degree) {
                    index = i;
                }
            }
                //否则优先选择可选颜色最少的
            else if (node[i].ableColor < minColor) {
                minColor = node[i].ableColor;
                index = i;
            }
        }
    }
    return index;
}

//恢复pos涂色状态以及相邻颜色可选择状态
void backTrack(int pos, int color) {
    for (int i = 0; i < N; i++) {
        if (i == pos) {
            isColor[i] = 0;//取消涂色
            node[i].colorArr[color] = true;
            continue;
        }
        if (node[i].isChange[pos]) {
            node[i].colorArr[color] = true;//设置color可以选取
            node[i].ableColor++;//可选颜色+1
            node[i].isChange[pos] = false;//设置当前状态改变标志位
        }
    }
}

//修改pos涂色状态以及相邻颜色可选择状态
void changeColor(int pos, int color) {
    for (int i = 0; i < N; i++) {
        if (i == pos) {
            //遍历到自己，涂上颜色color
            isColor[i] = color;//涂色
            node[i].colorArr[color] = false;
            continue;
        }
        //相邻且未涂色
        if (grid[i][pos] && !isColor[i]) {
            if (node[i].colorArr[color]) {
                node[i].colorArr[color] = false;//将与pos相邻节点设置为不能涂color
                node[i].ableColor--;//能选的颜色-1
                node[i].isChange[pos] = true;//设置当前状态改变标志位
            }
        }
    }
}


//pos：当前需要涂色的位置
void colorDFS(int pos, int doneCnt) {
    if (doneCnt == N) {
        res.push_back(isColor);//添加到解集中
        return;
    }
    //置换剪枝导致结果出错
    //尝试各种颜色
    for (int color = 1; color <= colorNum; color++) {
        if (!node[pos].colorArr[color])continue;//当前颜色不可选，剪枝
        if (!check(pos, color))continue;//无法选择当前颜色，剪枝
//        if (res.size() >= 1e5)return;
//        if (!res.empty())return;//只输出一个结果
        changeColor(pos, color);
        colorDFS(getNextNode(), doneCnt + 1);
        backTrack(pos, color);
    }
}

int main() {
    ios::sync_with_stdio(false);
    initiate();
    cout << "read in data" << endl;
    long long sum = 0, cnt = 5;
//    while (cnt--) {
    res.clear();
    auto start = std::chrono::steady_clock::now();
    colorDFS(0, 0);
    auto end = std::chrono::steady_clock::now();
    // 计算函数执行的时间，以毫秒为单位
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    sum += duration.count();
    cout << "single consume time: " << duration.count() << " ms" << endl;
//    }
//    cout << "平均运行时间：" << sum / 5 << " ms" << endl;
    cout << "solve number:" << res.size() << endl;
    cout << (checkRes() ? "res is right" : "res is false") << endl;
    writeRes();
//    int count = 0;
//    for (auto path: res) {
//        if (count == 30)break;
//        for (auto it: path) {
//            cout << it << " ";
//        }
//        cout << endl;
//        count++;
//    }
    return 0;
}
