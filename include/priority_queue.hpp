// 二项堆实现的优先队列。
#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cassert>
#include <cmath>      // maybe you need it
#include <cstddef>    // for size_t
#include <functional> // for std::less

#include "exceptions.hpp"

namespace sjtu {

template <class T, class Compare = std::less<T>> class priority_queue {
  private:
    struct node {
        T key;
        int deg;
        node *son;
        node *nxt;
        node(const T &v) : key(v), deg(0), son(nullptr), nxt(nullptr) {}
        node(const node &w) : key(w.key), deg(w.deg), son(w.son), nxt(w.nxt) {}
        ~node() {}
        node &operator=(const node &tmp) {
            if ((&tmp) == this)
                return *this;
            key = tmp.key;
            deg = tmp.deg;
            son = tmp.son;
            nxt = tmp.nxt;
            return *this;
        }
    };
    size_t num;
    node *root;
    Compare cmp;
    T *Mx;
    bool tag;

    node *copy(node *q) {
        if (!q)
            return nullptr;
        node *p = new node(q->key);
        p->deg = q->deg;
        p->son = copy(q->son);
        p->nxt = copy(q->nxt);
        return p;
    }
    void Free(node *p) {
        if (!p)
            return;
        Free(p->son);
        Free(p->nxt);
        delete p;
        return;
    }

    // deg_a = deg_b
    void merge(node *a, node *b) {
        b->nxt = a->son;
        a->son = b;
        a->deg++;
        return;
    }
    node *mergelist(node *p, node *q) {
        if (!p)
            return q;
        if (!q)
            return p;
        node *res = nullptr;
        node **it = &res;
        while (p && q) {
            if (p->deg <= q->deg)
                *it = p, p = p->nxt;
            else
                *it = q, q = q->nxt;
            it = &((*it)->nxt);
        }
        *it = p ? p : q;
        return res;
    }
    void carry() {
        if (!root)
            return;
        node *buf[32] = {nullptr};
        node *cur = root;
        int mx = 0;
        while (cur) {
            node *it = cur->nxt;
            int deg = cur->deg;
            while (buf[deg]) {
                node *aux = buf[deg];
                if (cmp(cur->key, aux->key))
                    std::swap(cur, aux);
                merge(cur, aux);
                buf[deg] = nullptr;
                deg++;
            }
            buf[deg] = cur;
            mx = std::max(mx, deg);
            cur = it;
        }
        root = nullptr;
        for (int i = mx; i >= 0; i--) {
            if (!buf[i])
                continue;
            buf[i]->nxt = root;
            root = buf[i];
        }
        return;
    }
    void update() {
        node *it = root;
        if (num) {
            if (tag)
                *Mx = root->key;
            else
                new (Mx) T(root->key), tag = 1;
        }
        while (it) {
            if (cmp(*Mx, it->key))
                *Mx = it->key;
            it = it->nxt;
        }
        return;
    }

  public:
    priority_queue() : num(0), root(nullptr), tag(0) {
        Mx = static_cast<T *>(malloc(sizeof(T)));
    }
    priority_queue(const priority_queue &q) {
        num = q.num;
        root = copy(q.root);
        Mx = static_cast<T *>(malloc(sizeof(T)));
        if (num)
            new (Mx) T(*q.Mx), tag = 1;
        else
            tag = 0;
    }
    ~priority_queue() {
        num = 0;
        Free(root);
        if (tag)
            (*Mx).~T();
        free(Mx);
    }
    priority_queue &operator=(const priority_queue &q) {
        if ((&q) == this)
            return *this;
        num = q.num;
        Free(root);
        root = copy(q.root);
        if (q.num) {
            if (tag)
                *Mx = *q.Mx;
            else
                new (Mx) T(*q.Mx), tag = 1;
        }
        return *this;
    }
    /**
     * @brief Push one element into the queue.
     * @note Its time complexity shall be O(log n).
     */
    void push(const T &v) {
        node *oldroot = root;
        T oldMx(v);
        if (num)
            oldMx = *Mx;
        node *tmp = new node(v);
        int len = 0;
        node *pos[32];
        node *val = static_cast<node *>(malloc(sizeof(node) * 32));
        auto it = root;
        while (it) {
            pos[len] = it;
            new (val + len) node(*it);
            len++;
            it = it->nxt;
        }
        // std::cout<<(*Mx).as_string()<<std::endl;
        try {
            num++;
            root = mergelist(root, tmp);
            carry();
            update();
        } catch (...) {
            num--;
            root = oldroot;
            for (int i = 0; i < len; i++)
                *pos[i] = val[i], val[i].~node();
            free(val);
            delete tmp;
            *Mx = oldMx;
            throw;
        }
        // std::cout<<"+"<<v<<std::endl;
        for (int i = 0; i < len; i++)
            val[i].~node();
        free(val);
        return;
    }

    /**
     * @return A const reference of the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(1).
     */
    const T &top() const {
        if (!num)
            throw container_is_empty();
        return *Mx;
    }

    /**
     * @brief remove the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(log n).
     */
    void pop() {
        if (!num)
            throw container_is_empty();
        node *oldroot = root;
        T oldMx(*Mx);
        int len = 0;
        node *Pos[64];
        node *Val = static_cast<node *>(malloc(sizeof(node) * 64));
        node *it = root, *tmp = nullptr;
        T res = root->key;
        node *pos = root, *lst = nullptr;
        try {
            while (it) {
                if (cmp(res, it->key))
                    res = it->key, pos = it, lst = tmp;
                Pos[len] = it, new (Val + len) node(*it);
                len++;
                tmp = it;
                it = it->nxt;
            }
        } catch (...) {
            free(Val);
            throw;
        }
        it = pos->son;
        int st = len;
        while (it) {
            Pos[len] = it, new (Val + len) node(*it);
            len++;
            it = it->nxt;
        }
        try {
            num--;
            if (lst)
                lst->nxt = pos->nxt;
            else
                root = root->nxt;
            if (len > st) {
                for (int i = len - 1; i > st; i--)
                    Pos[i]->nxt = Pos[i - 1];
                Pos[st]->nxt = nullptr;
                root = mergelist(root, Pos[len - 1]);
                carry();
            }
            update();
            //            if(num)top();
        } catch (...) {
            num++;
            root = oldroot;
            for (int i = 0; i < len; i++)
                *Pos[i] = Val[i], Val[i].~node();
            free(Val);
            *Mx = oldMx;
            throw;
        }
        for (int i = 0; i < len; i++)
            Val[i].~node();
        free(Val);
        delete pos;
        return;
    }

    /**
     * @return number of elements in the queue.
     */
    size_t size() const { return num; }

    /**
     * @return whether there is any element in the queue.
     */
    bool empty() const { return (num == 0); }

    /**
     * @brief Clear all elements in the queue.
     * @note Its time complexity shall be O(n).
     */
    void clear() {
        num = 0;
        Free(root);
        root = nullptr;
        return;
    }

    /**
     * @brief Merge element sets of two queues.
     * @note Its time complexity shall be O(log n).
     */
    void merge(priority_queue &q) {
        if ((&q) == this)
            return;
        if (!q.num)
            return;
        node *oldroot = root;
        T oldMx(*q.Mx);
        if (num)
            oldMx = *Mx;
        int len = 0;
        node *pos[64];
        node *val = static_cast<node *>(malloc(sizeof(node) * 64));
        node *it = root;
        while (it) {
            pos[len] = it;
            new (val + len) node(*it);
            len++;
            it = it->nxt;
        }
        it = q.root;
        while (it) {
            pos[len] = it;
            new (val + len) node(*it);
            len++;
            it = it->nxt;
        }
        try {
            num += q.num;
            root = mergelist(root, q.root);
            carry();
            update();
            //            if(num)top();
        } catch (...) {
            num -= q.num;
            root = oldroot;
            for (int i = 0; i < len; i++)
                *pos[i] = val[i], val[i].~node();
            free(val);
            if (num)
                *Mx = oldMx;
            throw;
        }
        for (int i = 0; i < len; i++)
            val[i].~node();
        free(val);
        q.num = 0;
        q.root = nullptr;
        return;
    }
};

} // namespace sjtu

#endif