#ifndef BPT_HPP
#define BPT_HPP
#include "vector.hpp"
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "MemoryRiver.hpp"

using sjtu::vector;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

template <class Key, class Val, int M> // 至多 M-1 个儿子
class BPT {
  private:
    struct info {
        Key key;
        Val val;
        info() = default;
        info(Key _key, Val _val) : key(_key), val(_val) {}
        info &operator=(const info &other) {
            key = other.key;
            val = other.val;
            return *this;
        }
        bool operator<(const info &other) {
            if (!(key == other.key))
                return key < other.key;
            return val < other.val;
        }
        bool operator<=(const info &other) {
            if (!(key == other.key))
                return key < other.key;
            return !(other.val < val);
        }
        bool operator==(const info &other) {
            return key == other.key && val == other.val;
        }
    };
    struct node {
        size_t siz; // 目前儿子数量，保证 siz 在 M/2 到 M-1 之间
        info arr[M];
        int son[M];   // 至多 M-1 个儿子在 bpt 文件中的位置
        int pre, nxt; // 叶节点链表
        node() {
            siz = 0;
            pre = nxt = -1;
        }
        node &operator=(const node &other) {
            siz = other.siz;
            for (int i = 0; i < siz; i++)
                arr[i] = other.arr[i];
            for (int i = 0; i < siz; i++)
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
    int id[15], to[15];
    bool upd[15];
    node buf[15];

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

    void clear() {
        root = 0, tot = 0, D = 0;
        bpt.clear();
        return;
    }

    bool empty() { return root == 0; }

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
        if (pos == cur.siz && d) {
            info ww = cur.arr[cur.siz - 1];
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

    vector<Val> find(Key key) {
        vector<Val> res;
        if (!root)
            return res;
        node cur;
        bpt.readorder(cur, root);
        int tmp = root;
        for (int i = 0; i < D; i++) {
            int pos = cur.siz;
            for (int j = 0; j < cur.siz; j++)
                if (!(cur.arr[j].key < key)) {
                    pos = j;
                    break;
                }
            if (pos == cur.siz)
                return res;
            tmp = cur.son[pos];
            bpt.readorder(cur, tmp);
        }
        bool tag = 0, ed = 0;
        while (!ed) {
            for (int i = 0; i < cur.siz; i++) {
                if (key == cur.arr[i].key)
                    tag = 1, res.push_back(cur.arr[i].val);
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
        return res;
    }
};
#endif