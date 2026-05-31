#include <cassert>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

typedef unsigned long long ull;

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;
template <class T, int info_len = 2> class MemoryRiver {
  private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    static_assert(sizeof(T) >= sizeof(int));

  public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {}
    void open() {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        return;
    }
    void close() {
        file.close();
        return;
    }
    void initialise(string FN = "") {
        if (FN != "")
            file_name = FN;
        if (std::filesystem::exists(file_name))
            return;
        file.open(file_name, std::ios::out | std::ios::binary);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i) {
            file.seekp(i * sizeof(int), std::ios::beg);
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        file.close();
        return;
    }

    void get_info(int &tmp, int n) {
        if (n > info_len)
            return;
        file.seekg((n - 1) * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        return;
    }

    void write_info(int tmp, int n) {
        if (n > info_len)
            return;
        file.seekp((n - 1) * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        return;
    }

    int write(T &t) {
        int head = 0;
        get_info(head, 1);
        int index = 0;
        if (head) {
            index = head, file.seekg(head, std::ios::beg);
            int nxt = 0;
            file.read(reinterpret_cast<char *>(&nxt), sizeof(int));
            write_info(nxt, 1);
            file.seekp(index, std::ios::beg);
        } else {
            file.seekp(0, std::ios::end);
            index = static_cast<int>(file.tellp());
        }
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        return index;
    }

    void update(T &t, const int index) {
        file.seekp(index, std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        return;
    }

    void read(T &t, const int index) {
        file.seekg(index, std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        return;
    }

    void Delete(int index) {
        if (index <= 0)
            return;
        int head = 0;
        get_info(head, 1);
        file.seekp(index, std::ios::beg),
            file.write(reinterpret_cast<char *>(&head), sizeof(int));
        write_info(index, 1);
        return;
    }

    void readorder(T &t, const int id) {
        file.seekg(info_len * sizeof(int) + (id - 1) * sizeofT);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        return;
    }

    void writeorder(T t, const int id) {
        int num = 0;
        get_info(num, 1);
        if (num >= id)
            file.seekp(info_len * sizeof(int) + (id - 1) * sizeofT);
        else {
            write_info(num + 1, 1);
            file.seekp(0, std::ios::end), file.tellp();
        }
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        return;
    }

    /*vector<T> query() {
        int len;
        open();
        get_info(len, 1);
        vector<T> ans(len);
        for (int i = 0; i < len; i++)
            read(ans[i], info_len * sizeof(int) + i * sizeofT);
        close();
        return ans;
    }*/
};
template <class Key, class Val, int M> // 至多 M-1 个儿子
class BPT {
  private:
    struct info {
        Key key;
        Val val;
        info() {}
        info(Key _key, Val _val) : key(_key), val(_val) {}
        info &operator=(const info &other) {
            key = other.key;
            val = other.val;
            return *this;
        }
        bool operator<(const info &other) {
            if (key != other.key)
                return key < other.key;
            return val < other.val;
        }
        bool operator<=(const info &other) {
            if (key != other.key)
                return key < other.key;
            return val <= other.val;
        }
        bool operator==(const info &other) {
            return key == other.key && val == other.val;
        }
    };
    struct node {
        size_t siz; // 目前儿子数量，保证 siz 在 M/2 到 2M-1 之间
        info arr[M];
        int son[M];   // 至多 M-1 个儿子在 bpt 文件中的位置
        int pre, nxt; // 叶节点链表
        node() {
            siz = 0;
            pre = nxt = -1;
        }
        node &operator=(const node &other) {
            siz = other.siz;
            for (int i = 0; i < M; i++)
                arr[i] = other.arr[i];
            for (int i = 0; i < M; i++)
                son[i] = other.son[i];
            pre = other.pre;
            nxt = other.nxt;
            return *this;
        }
    };
    MemoryRiver<node, 3> bpt;
    int tot;  // 结点总数
    int root; // 根结点位置
    int D;    // 总深度
    int id[105], to[105];
    bool upd[105];
    node buf[105];

  public:
    BPT(string name) {
        bpt.initialise(name);
        bpt.open();
        bpt.get_info(tot, 1), bpt.get_info(root, 2);
        bpt.get_info(D, 3);
    }
    ~BPT() {
        bpt.write_info(tot, 1), bpt.write_info(root, 2);
        bpt.write_info(D, 3);
        bpt.close();
    }

  private:
    void _insert(int d, int y, info &w) {
        upd[d] = 1;
        node &cur = buf[d];
        int x = id[d];
        int pos = cur.siz;
        for (int i = 0; i < cur.siz; i++)
            if (w == cur.arr[i])
                return;
        for (int i = 0; i < cur.siz; i++)
            if (w < cur.arr[i]) {
                pos = i;
                break;
            }
        for (int i = cur.siz; i > pos; i--)
            cur.arr[i] = cur.arr[i - 1], cur.son[i] = cur.son[i - 1];
        cur.arr[pos] = w;
        cur.son[pos] = y;
        if (cur.siz < M - 1) {
            cur.siz++;
            return;
        }
        int m = M / 2;
        node aux;
        tot++;
        aux.siz = m;
        cur.siz = m;
        for (int i = 0; i < m; i++)
            aux.arr[i] = cur.arr[i], aux.son[i] = cur.son[i];
        for (int i = 0; i < m; i++)
            cur.arr[i] = cur.arr[i + m], cur.son[i] = cur.son[i + m];
        if (d == D) {
            int z = cur.pre;
            aux.pre = z;
            aux.nxt = x;
            if (z != -1) {
                node tmp;
                bpt.readorder(tmp, z);
                tmp.nxt = tot;
                bpt.writeorder(tmp, z);
            }
            cur.pre = tot;
        }
        bpt.writeorder(aux, tot);
        if (d)
            _insert(d - 1, tot, aux.arr[aux.siz - 1]);
        else {
            root = ++tot;
            node nwrt;
            nwrt.siz = 2;
            nwrt.arr[0] = aux.arr[aux.siz - 1],
            nwrt.arr[1] = cur.arr[cur.siz - 1];
            nwrt.son[0] = tot - 1, nwrt.son[1] = x;
            D++;
            bpt.writeorder(nwrt, tot);
        }
        return;
    }

  public:
    void ins(Key key, Val val) {
        info w = info(key, val);
        if (!root) {
            node lf;
            lf.siz = 1;
            D = 0;
            lf.arr[0] = w;
            root = ++tot;
            bpt.writeorder(lf, tot);
            return;
        }
        node cur;
        bpt.readorder(cur, root);
        int tmp = root, DD = D;
        for (int i = 0; i < D; i++) {
            buf[i] = cur;
            id[i] = tmp;
            upd[i] = 0;
            int pos = cur.siz;
            for (int j = 0; j < cur.siz; j++)
                if (w < cur.arr[j]) {
                    pos = j;
                    break;
                }
            if (pos == cur.siz)
                upd[i] = 1, buf[i].arr[cur.siz - 1] = w, pos--;
            tmp = cur.son[pos];
            bpt.readorder(cur, tmp);
        }
        buf[D] = cur, id[D] = tmp, upd[D] = 1;
        _insert(D, 0, w);
        for (int i = 0; i <= DD; i++)
            if (upd[i])
                bpt.writeorder(buf[i], id[i]);
        return;
    }

  private:
    void _delete(int d, info &w) {
        node &cur = buf[d];
        int x = id[d];
        int pos = cur.siz;
        for (int i = 0; i < cur.siz; i++)
            if (w == cur.arr[i]) {
                pos = i;
                break;
            }
        if (pos == cur.siz)
            return;
        for (int i = pos; i < cur.siz; i++)
            cur.arr[i] = cur.arr[i + 1], cur.son[i] = cur.son[i + 1];
        cur.siz--;
        upd[d] = 1;
        info ww = cur.arr[cur.siz - 1];
        if (pos == cur.siz) {
            for (int i = d - 1; i >= 0; i--) {
                node &aux = buf[i];
                int idx = id[i], it = aux.siz;
                for (int j = 0; j < aux.siz; j++)
                    if (aux.arr[j] == w) {
                        it = j;
                        break;
                    }
                if (it == aux.siz)
                    break;
                aux.arr[it] = ww;
                upd[i] = 1;
            }
        }
        if (d == 0) {
            if (cur.siz == 0)
                root = D = 0;
            if (cur.siz == 1 && D)
                root = cur.son[0], D--;
            return;
        }
        if (cur.siz < M / 2) {
            int pre_id = to[d - 1] ? buf[d - 1].son[to[d - 1] - 1] : -1,
                nxt_id = (to[d - 1] < buf[d - 1].siz - 1)
                             ? buf[d - 1].son[to[d - 1] + 1]
                             : -1;
            node pre_node, nxt_node;
            bool ok = 0;
            // 若左右兄弟可以借一个结点给 cur
            if (!ok && pre_id != -1) {
                bpt.readorder(pre_node, pre_id);
                if (pre_node.siz > M / 2) {
                    pre_node.siz--;
                    for (int i = cur.siz; i >= 1; i--)
                        cur.arr[i] = cur.arr[i - 1],
                        cur.son[i] = cur.son[i - 1];
                    cur.arr[0] = pre_node.arr[pre_node.siz];
                    cur.son[0] = pre_node.son[pre_node.siz];
                    cur.siz++;
                    bpt.writeorder(pre_node, pre_id);
                    buf[d - 1].arr[to[d - 1] - 1] =
                        pre_node.arr[pre_node.siz - 1];
                    upd[d - 1] = 1;
                    ok = 1;
                }
            }
            if (!ok && nxt_id != -1) {
                bpt.readorder(nxt_node, nxt_id);
                if (nxt_node.siz > M / 2) {
                    nxt_node.siz--;
                    cur.arr[cur.siz] = nxt_node.arr[0];
                    cur.son[cur.siz] = nxt_node.son[0];
                    cur.siz++;
                    for (int i = 0; i < nxt_node.siz; i++)
                        nxt_node.arr[i] = nxt_node.arr[i + 1],
                        nxt_node.son[i] = nxt_node.son[i + 1];
                    bpt.writeorder(nxt_node, nxt_id);
                    buf[d - 1].arr[to[d - 1]] = cur.arr[cur.siz - 1];
                    upd[d - 1] = 1;
                    ok = 1;
                }
            }
            // 否则，跟某个兄弟合并为一个 M-1 大结点
            if (!ok && pre_id != -1) {
                cur.siz = M - 1;
                for (int i = M / 2; i < M - 1; i++)
                    cur.arr[i] = cur.arr[i - M / 2],
                    cur.son[i] = cur.son[i - M / 2];
                for (int i = 0; i < M / 2; i++)
                    cur.arr[i] = pre_node.arr[i], cur.son[i] = pre_node.son[i];
                if (d == D) {
                    int z = pre_node.pre;
                    cur.pre = z;
                    if (z != -1) {
                        node aux;
                        bpt.readorder(aux, z);
                        aux.nxt = x;
                        bpt.writeorder(aux, z);
                    }
                }
                _delete(d - 1, pre_node.arr[M / 2 - 1]);
                ok = 1;
            }
            if (!ok && nxt_id != -1) {
                nxt_node.siz = M - 1;
                for (int i = M - 2; i >= M / 2 - 1; i--)
                    nxt_node.arr[i] = nxt_node.arr[i - M / 2 + 1],
                    nxt_node.son[i] = nxt_node.son[i - M / 2 + 1];
                for (int i = 0; i < M / 2 - 1; i++)
                    nxt_node.arr[i] = cur.arr[i], nxt_node.son[i] = cur.son[i];
                if (d == D) {
                    int z = cur.pre;
                    nxt_node.pre = z;
                    if (z != -1) {
                        node aux;
                        bpt.readorder(aux, z);
                        aux.nxt = nxt_id;
                        bpt.writeorder(aux, z);
                    }
                }
                buf[d] = nxt_node;
                id[d] = nxt_id;
                _delete(d - 1, cur.arr[M / 2 - 2]);
                ok = 1;
            }
        }
        return;
    }

  public:
    void del(Key key, Val val) {
        info w = info(key, val);
        if (!root)
            return;
        node cur;
        bpt.readorder(cur, root);
        int tmp = root;
        for (int i = 0; i < D; i++) {
            buf[i] = cur;
            id[i] = tmp;
            upd[i] = 0;
            int pos = cur.siz;
            for (int j = 0; j < cur.siz; j++)
                if (w <= cur.arr[j]) {
                    pos = j;
                    break;
                }
            if (pos == cur.siz)
                return;
            to[i] = pos;
            tmp = cur.son[pos];
            bpt.readorder(cur, tmp);
        }
        buf[D] = cur;
        id[D] = tmp;
        upd[D] = 1;
        int DD = D;
        _delete(D, w);
        for (int i = 0; i <= DD; i++)
            if (upd[i])
                bpt.writeorder(buf[i], id[i]);
        return;
    }

    void find(Key key) {
        if (!root) {
            cout << "null" << endl;
            return;
        }
        node cur;
        bpt.readorder(cur, root);
        int tmp = root;
        for (int i = 0; i < D; i++) {
            int pos = cur.siz;
            for (int j = 0; j < cur.siz; j++)
                if (key <= cur.arr[j].key) {
                    pos = j;
                    break;
                }
            if (pos == cur.siz) {
                cout << "null" << endl;
                return;
            }
            tmp = cur.son[pos];
            bpt.readorder(cur, tmp);
        }
        bool tag = 0, ed = 0;
        while (!ed) {
            for (int i = 0; i < cur.siz; i++) {
                if (key == cur.arr[i].key)
                    tag = 1, cout << cur.arr[i].val << " ";
                else if (key < cur.arr[i].key) {
                    ed = 1;
                    break;
                }
            }
            if (!ed) {
                if (cur.nxt == -1)
                    ed = 1;
                else
                    bpt.readorder(cur, cur.nxt);
            }
        }
        if (!tag)
            cout << "null";
        cout << endl;
        return;
    }
};
const int bs = 2333;
const int mod1 = 998244353, mod2 = 1e9 + 7;
ull Hash(string st) {
    int res1 = 0, res2 = 0;
    for (int i = 0; i < st.size(); i++)
        res1 = (1ll * res1 * bs + st[i] + 256) % mod1,
        res2 = (1ll * res2 * bs + st[i] + 256) % mod2;
    return 1ll * res1 * mod2 + res2;
}
int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    BPT<ull, int, 210> t("BPT");
    int n;
    cin >> n;
    while (n--) {
        string op, index;
        int value;
        cin >> op;
        if (op == "insert")
            cin >> index >> value, t.ins(Hash(index), value);
        else if (op == "delete")
            cin >> index >> value, t.del(Hash(index), value);
        else
            cin >> index, t.find(Hash(index));
    }
    return 0;
}