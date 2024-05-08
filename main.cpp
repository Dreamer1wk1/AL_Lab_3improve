#include <bits/stdc++.h>

using namespace std;
const int N = 450;
const int colorNum = 5;
struct Node {
    int degree;//��
    int ableColor;//��ѡ��ɫ����
    bool colorArr[colorNum + 1];//��ѡ����ɫ
    bool isChange[N];//��ɫ״̬�Ƿ��޸Ĺ�,[pos]=True��ʾ�յ�posɫ��Ӱ����ı�״̬
};
int grid[N][N];
Node temp;
vector<Node> node(N, temp);//�洢ÿ���ڵ�
vector<int> isColor(N, 0);//0δͿɫ,1~NΪ��ɫ
vector<vector<int>> res;

////�����������
//void generate() {
//    int start, end, range = 2000;// �ߵ�����,������̫�ٵ��½���������࣬Ҳ������̫�ർ�½��޽�
//    // �������������ͷֲ�
//    std::random_device rd;
//    std::mt19937 gen(rd()); // ʹ�� Mersenne Twister ����
//    std::uniform_int_distribution<int> get(0, N - 1); // ���� [0, N-1] ��Χ�ڵ���
//    while (range--) {
//        start = get(rd);
//        end = get(rd);
//        grid[start][end] = 1, grid[end][start] = 1;
//        node[start].degree++;
//        node[end].degree++;
//    }
//}

//�������
void writeRes() {
    // ���ļ�������������򴴽�����������򸲸�ԭ������
    ofstream outfile("le450_5a.col.out");
    if (!outfile) {
        cout << "�޷����ļ�" << endl;
        return;
    }
    // ���ļ�д������
    for (auto it: res[0]) {
        outfile << it << endl;
    }
    // �ر��ļ�
    outfile.close();
}

//�����ṩ������
void readIn() {
//    "le450_25a.col"
//    "le450_15b.col"
//    "le450_5a.col"
//    "smallData.txt"
//    "le100_4_random.txt"
//    "le200_4_random.txt"
//    "le300_4_random.txt"
//    "le400_4_random.txt"
    ifstream file("le450_5a.col"); // ���ļ�
    if (!file.is_open()) {
        cout << "�޷����ļ�" << endl;
        return;
    }
    string line;
    int start, end;
    char ch;
    // ѭ����ȡ�ļ��е�ÿһ��
    while (getline(file, line)) {
        if (line.empty()) {
            continue; // ��������
        }
        if (line[0] != 'e') {
            continue; // ������ͷ��
        }
        istringstream iss(line); // ����ǰ��ת��Ϊ�ַ�����
        if (iss >> ch >> start >> end) { // ���ַ������ж�ȡ��������
            start--, end--;
            grid[start][end] = 1, grid[end][start] = 1;
            node[start].degree++;
            node[end].degree++;
        }
    }
    file.close(); // �ر��ļ�
}

//�����Ƿ���ȷ
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
            //ûͿɫ�����ڽڵ�ֻʣcolor
            if (!isColor[i] && node[i].ableColor == 1 && node[i].colorArr[color]) {
                return false;
            }
        }
    }
    return true;
}

//����ѡ���ѡ��ɫ���ٵģ���ͬ�����ѡ������
int getNextNode() {
    int index = 0, minColor = colorNum;
    for (int i = 0; i < N; i++) {
        if (isColor[i] == 0) {
            //��ѡ��ɫ=��С
            if (node[i].ableColor == minColor) {
                //ѡ�������
                if (node[i].degree >= node[index].degree) {
                    index = i;
                }
            }
                //��������ѡ���ѡ��ɫ���ٵ�
            else if (node[i].ableColor < minColor) {
                minColor = node[i].ableColor;
                index = i;
            }
        }
    }
    return index;
}

//�ָ�posͿɫ״̬�Լ�������ɫ��ѡ��״̬
void backTrack(int pos, int color) {
    for (int i = 0; i < N; i++) {
        if (i == pos) {
            isColor[i] = 0;//ȡ��Ϳɫ
            node[i].colorArr[color] = true;
            continue;
        }
        if (node[i].isChange[pos]) {
            node[i].colorArr[color] = true;//����color����ѡȡ
            node[i].ableColor++;//��ѡ��ɫ+1
            node[i].isChange[pos] = false;//���õ�ǰ״̬�ı��־λ
        }
    }
}

//�޸�posͿɫ״̬�Լ�������ɫ��ѡ��״̬
void changeColor(int pos, int color) {
    for (int i = 0; i < N; i++) {
        if (i == pos) {
            //�������Լ���Ϳ����ɫcolor
            isColor[i] = color;//Ϳɫ
            node[i].colorArr[color] = false;
            continue;
        }
        //������δͿɫ
        if (grid[i][pos] && !isColor[i]) {
            if (node[i].colorArr[color]) {
                node[i].colorArr[color] = false;//����pos���ڽڵ�����Ϊ����Ϳcolor
                node[i].ableColor--;//��ѡ����ɫ-1
                node[i].isChange[pos] = true;//���õ�ǰ״̬�ı��־λ
            }
        }
    }
}


//pos����ǰ��ҪͿɫ��λ��
void colorDFS(int pos, int doneCnt) {
    if (doneCnt == N) {
        res.push_back(isColor);//��ӵ��⼯��
        return;
    }
    //�û���֦���½������
    //���Ը�����ɫ
    for (int color = 1; color <= colorNum; color++) {
        if (!node[pos].colorArr[color])continue;//��ǰ��ɫ����ѡ����֦
        if (!check(pos, color))continue;//�޷�ѡ��ǰ��ɫ����֦
//        if (res.size() >= 1e5)return;
//        if (!res.empty())return;//ֻ���һ�����
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
    // ���㺯��ִ�е�ʱ�䣬�Ժ���Ϊ��λ
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    sum += duration.count();
    cout << "single consume time: " << duration.count() << " ms" << endl;
//    }
//    cout << "ƽ������ʱ�䣺" << sum / 5 << " ms" << endl;
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
