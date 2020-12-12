#include <iostream>
using namespace std;

template <class K>
struct bstNode
{
    K value;
    int bf; //平衡因子，值为该节点左子树高度减右子树高度
    bstNode<K> *parent;
    bstNode<K> *leftC;
    bstNode<K> *rightC;
};

template <class K>
class AVLTree
{
private:
    bool taller, lower;
    bstNode<K> *tempu, *tempv;

public:
    bstNode<K> *root;
    AVLTree(K rootval)
    {
        this->root = new bstNode<K>{rootval, 0, NULL, NULL, NULL};
    }

    //左旋转
    void leftRot(bstNode<K> *T)
    {
        bstNode<K> *R = T->rightC;
        if (R->leftC != NULL)
        {
            R->leftC->parent = T;
            T->rightC = R->leftC;
        }
        if (T->parent != NULL)
        {
            if (T->parent->leftC == T)
            {
                T->parent->leftC = R;
            }
            else
            {
                T->parent->rightC = R;
            }
        }
        R->parent = T->parent;
        T->parent = R;
        R->leftC = T;
        if (T == root)
        {
            root = R;
        }
    }

    //右旋转
    void rightRot(bstNode<K> *T)
    {
        bstNode<K> *L = T->leftC;
        if (L->rightC != NULL)
        {
            L->rightC->parent = T;
            T->leftC = L->rightC;
        }
        if (T->parent != NULL)
        {
            if (T->parent->leftC == T)
            {
                T->parent->leftC = L;
            }
            else
            {
                T->parent->rightC = L;
            }
        }
        L->parent = T->parent;
        T->parent = L;
        L->rightC = T;
        if (T == root)
        {
            root = L;
        }
    }

    //T 的左边高，不平衡，使其平衡，右旋转，右旋转前先检查L->bf，
    //如果为RH，L要先进行左旋转，使T->leftC->bf和T->bf一致
    void leftHigh(bstNode<K> *T)
    {
        bstNode<K> *L, *Lr;
        L = T->leftC;
        Lr = L->rightC;
        switch (L->bf)
        {
        case 1:
            L->bf = T->bf = 0;
            rightRot(T);
            break;
        case -1:
            switch (Lr->bf)
            {
            case 1:
                L->bf = 0;
                T->bf = -1;
                break;
            case 0:
                L->bf = T->bf = 0;
                break;
            case -1:
                L->bf = 1;
                T->bf = 0;
                break;
            }
            Lr->bf = 0;
            leftRot(L);
            rightRot(T);
            break;
        }
    }

    //T 的右边高，不平衡，使其平衡，左旋转，左旋转前先检查R->bf,
    //如果为LH，R要先进行右旋转，使T->rightC->bf和T->bf一致
    void rightHigh(bstNode<K> *T)
    {
        bstNode<K> *R, *Rl;
        R = T->rightC;
        Rl = R->leftC;
        switch (R->bf)
        {
        case -1:
            R->bf = T->bf = 0;
            leftRot(T);
            break;
        case 1:
            switch (R->bf)
            {
            case 1:
                R->bf = -1;
                T->bf = 0;
                break;
            case 0:
                R->bf = T->bf = 0;
                break;
            case -1:
                R->bf = 0;
                T->bf = 1;
                break;
            }
            Rl->bf = 0;
            rightRot(R);
            leftRot(T);
            break;
        }
    }

    //寻找Successor
    bstNode<K> *findSuc(bstNode<K> *T, bstNode<K> *p, K q)
    {
        if (T == NULL)
        {
            return p;
        }
        else if (q == T->value)
        {
            return T;
        }
        else if (q < T->value)
        {
            p = T;
            return findSuc(T->leftC, p, q);
        }
        else
        {
            return findSuc(T->rightC, p, q);
        }
    }

    //寻找Predecessor
    bstNode<K> *findPred(bstNode<K> *T, bstNode<K> *p, K q)
    {
        if (T == NULL)
        {
            return p;
        }
        else if (q == T->value)
        {
            return T;
        }
        else if (q < T->value)
        {
            return findPred(T->leftC, p, q);
        }
        else
        {
            p = T;
            return findPred(T->rightC, p, q);
        }
    }

    void treeInsert(K val)
    {
        bstNode<K> *T = root, *cld;
        while (true)
        {
            if (val == T->value)
            {
                return;
            }
            else if (val < T->value)
            {
                if (T->leftC == NULL)
                {
                    break;
                }
                T = T->leftC;
            }
            else
            {
                if (T->rightC == NULL)
                {
                    break;
                }
                T = T->rightC;
            }
        }
        if (val < T->value)
        {
            T->leftC = new bstNode<K>{val, 0, T, NULL, NULL};
            switch (T->bf)
            {
            case -1:
                T->bf = 0;
                taller = false;
                break;
            case 0:
                T->bf = 1;
                taller = true;
                break;
            }
        }
        else
        {
            T->rightC = new bstNode<K>{val, 0, T, NULL, NULL};
            switch (T->bf)
            {
            case 1:
                T->bf = 0;
                taller = false;
                break;
            case 0:
                T->bf = -1;
                taller = true;
                break;
            }
        }
        while (T != root)
        {
            cld = T;
            T = T->parent;
            if (T->leftC == cld)
            {
                if (taller)
                {
                    switch (T->bf)
                    {
                    case -1:
                        T->bf = 0;
                        taller = false;
                        break;
                    case 0:
                        T->bf = 1;
                        taller = true;
                        break;
                    case 1:
                        leftHigh(T);
                        taller = false;
                        break;
                    }
                }
            }
            else
            {
                if (taller)
                {
                    switch (T->bf)
                    {
                    case -1:
                        rightHigh(T);
                        taller = false;
                        break;
                    case 0:
                        T->bf = -1;
                        taller = true;
                        break;
                    case 1:
                        T->bf = 0;
                        taller = false;
                        break;
                    }
                }
            }
        }
    }

    // bool nodeInsert(bstNode<K> *T, K val)
    // { // 以T为根节点的平衡二叉树中插入元素val，成功返回true，失败返回false，taller反映树是否长高
    //     if (T == NULL)
    //     { // 只有刚开始插入树为空才会执行这一步
    //         T = new bstNode<K>{val, 0, NULL, NULL};
    //         return true;
    //     }
    //     if (val == T->value)
    //     {
    //         taller = false;
    //         return false;
    //     }
    //     else if (val < T->value)
    //     {
    //         if (T->leftC == NULL)
    //         { // 检查左子树
    //             bstNode<K> *q = new bstNode<K>{val, 0, NULL, NULL};
    //             T->leftC = q; // 直接插入
    //             switch (T->bf)
    //             {
    //             case 0:
    //                 T->bf = 1;
    //                 taller = true;
    //                 break;
    //             case -1:
    //                 T->bf = 0;
    //                 taller = false;
    //                 break;
    //             }
    //             return true; //成功插入节点之后直接弹出回到上一级，携带taller变量
    //         }
    //         if (!nodeInsert(T->leftC, val)) //递归调用，这里进入if语句的情况只有发现重复节点
    //         {
    //             return false;
    //         }
    //         if (taller) // taller变量为true说明其左子树长高了
    //         {
    //             switch (T->bf)
    //             {       // 检查结点的平衡因子
    //             case 1: // 原本结点的左子树比右子树高，且在左子树中插入了，需要做左平衡处理，处理之后树的高度不变
    //                 leftHigh(T);
    //                 taller = false;
    //                 break;
    //             case 0: // 左右子树同样高，在左子树中插入，只是树变高了、平衡因子变为1，但当前不用做平衡处理
    //                 T->bf = 1;
    //                 taller = true;
    //                 break;
    //             case -1: // 右子树比左子树高，在左子树中插入，树的高度不变，当前结点的平衡因子变为0
    //                 T->bf = 0;
    //                 taller = false;
    //                 break;
    //             }
    //         } // 每次递归都平衡一次，直到根节点
    //     }
    //     else
    //     {
    //         if (T->rightC == NULL)
    //         { // 检查右子树
    //             bstNode<K> *q = new bstNode<K>{val, 0, NULL, NULL};
    //             T->rightC = q;
    //             switch (T->bf)
    //             {
    //             case 0:
    //                 T->bf = -1;
    //                 taller = true;
    //                 break;
    //             case 1:
    //                 T->bf = 0;
    //                 taller = false;
    //                 break;
    //             }
    //             return true;
    //         }
    //         if (!nodeInsert(T->rightC, val))
    //         {
    //             return false;
    //         }
    //         if (taller)
    //         {
    //             switch (T->bf)
    //             {
    //             case 1:
    //                 T->bf = 0;
    //                 taller = false;
    //                 break;
    //             case 0:
    //                 T->bf = -1;
    //                 taller = true;
    //                 break;
    //             case -1:
    //                 rightHigh(T);
    //                 taller = false;
    //                 break;
    //             }
    //         }
    //     }
    //     return true;
    // }

    void treeDelete(K val)
    {
        bstNode<K> *T = root, *temp, *cld, *U;
        while (val != T->value)
        {
            if (val < T->value)
            {
                T = T->leftC;
            }
            else
            {
                T = T->rightC;
            }
        }
        if (T->leftC == NULL && T->rightC == NULL) //T本身为叶子节点 直接删除T即可
        {
            temp = T;
            T = T->parent;
            if (T == NULL)
            {
                root = NULL;
            }
            else if (T->leftC == temp)
            {
                T->leftC = NULL;
                switch (T->bf)
                {
                case -1:
                    rightHigh(T);
                    lower = 0;
                    break;
                case 0:
                    T->bf = -1;
                    lower = false;
                    break;
                case 1:
                    T->bf = 0;
                    lower = true;
                    break;
                }
            }
            else
            {
                T->rightC = NULL;
                switch (T->bf)
                {
                case -1:
                    T->bf = 0;
                    lower = true;
                    break;
                case 0:
                    T->bf = 1;
                    lower = false;
                    break;
                case 1:
                    leftHigh(T);
                    lower = true;
                    break;
                }
            }
            delete temp;
            lower = true;
            while (T != NULL)
            {

                cld = T;
                T = T->parent;
            }
        }
        else if (T->rightC == NULL)
        {
            U = T->leftC;
            temp = T;
            T = T->parent;
            U->parent = T;
            if (T == NULL)
            {
                root = U;
            }
            else if (T->leftC == temp)
            {
                T->leftC = U;
            }
            else
            {
                T->rightC = U;
            }
            delete temp;
            lower = true;
            cld = U;
            while (T != NULL)
            {
                if (T->leftC == cld)
                {
                    if (lower)
                    {
                        switch (T->bf)
                        {
                        case -1:
                            rightHigh(T);
                            lower = false;
                            break;
                        case 0:
                            T->bf = -1;
                            lower = false;
                        case 1:
                            T->bf = 0;
                            lower = true;
                        }
                    }
                }
                else
                {
                    if (lower)
                    {
                        switch (T->bf)
                        {
                        case -1:
                            T->bf = 0;
                            lower = true;
                            break;
                        case 0:
                            T->bf = 1;
                            lower = false;
                        case 1:
                            leftHigh(T);
                            lower = false;
                        }
                    }
                }
                cld = T;
                T = T->parent;
            }
        }
        else
        {
            U = findPred(T);
            T->value = U->value;
            if (U->rightC == NULL)
            {
                temp = U;
                U = U->parent;
                if (U->leftC == temp)
                {
                    U->leftC = NULL;
                }
                else
                {
                    U->rightC = NULL;
                }
                lower = true;
                delete temp;
            }
            else
            {
                temp = U;
                U = U->rightC;
                U->parent = temp->parent;
                if (temp->parent->rightC == temp)
                {
                    temp->parent->rightC = U;
                }
                else
                {
                    temp->parent->leftC = U;
                }
                lower = true;
                delete temp;
            }
            while (U != root)
            {
                cld = U;
                U = U->parent;
                if (U->leftC == cld)
                {
                    if (lower)
                    {
                        switch (T->bf)
                        {
                        case -1:
                            T->bf = 0;
                            taller = false;
                            break;
                        case 0:
                            T->bf = 1;
                            taller = true;
                            break;
                        case 1:
                            leftHigh(T);
                            taller = false;
                            break;
                        }
                    }
                }
                else
                {
                    if (taller)
                    {
                        switch (T->bf)
                        {
                        case -1:
                            rightHigh(T);
                            taller = false;
                            break;
                        case 0:
                            T->bf = -1;
                            taller = true;
                            break;
                        case 1:
                            T->bf = 0;
                            taller = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    void reBalanceL(bstNode<K> *T, bool lower)
    {

        if (T == NULL)
        {
            return;
        }
        bstNode<K> *cld;
        cld = T;
        T = T->parent;
        while (T != NULL)
        {
            if (T->leftC == cld)
            {
                if (lower)
                {
                    switch (T->bf)
                    {
                    case -1:
                        rightHigh(T);
                        lower = false;
                        break;
                    case 0:
                        T->bf = -1;
                        lower = false;
                        break;
                    case 1:
                        T->bf = 0;
                        lower = true;
                        break;
                    }
                }
            }
            else
            {
                if (lower)
                {
                    switch (T->bf)
                    {
                    case -1:
                        T->bf = 0;
                        lower = true;
                        break;
                    case 0:
                        T->bf = 1;
                        lower = false;
                        break;
                    case 1:
                        leftHigh(T);
                        lower = false;
                        break;
                    }
                }
            }
        }
    }

    // bool nodeDelete(bstNode<K> *T, K val)
    // {
    //     if (val < T->value)
    //     {
    //         if (T->leftC->value == val)
    //         {
    //             tempu = T->leftC;
    //             if (tempu->leftC == NULL && tempu->rightC == NULL)
    //             {
    //                 delete tempu;
    //                 T->leftC = NULL;
    //                 lower = true;
    //                 return true;
    //             }
    //             else if (tempu->rightC != NULL)
    //             {
    //             }
    //         }
    //         nodeDelete(T->leftC, val);
    //         if (taller)
    //         {
    //             switch (T->bf)
    //             {       // 检查结点的平衡因子
    //             case 1: // 原本结点的左子树比右子树高，且在左子树中插入了，需要做左平衡处理，处理之后树的高度不变
    //                 leftHigh(T);
    //                 taller = false;
    //                 break;
    //             case 0: // 左右子树同样高，在左子树中插入，只是树变高了、平衡因子变为1，但当前不用做平衡处理
    //                 T->bf = 1;
    //                 taller = true;
    //                 break;
    //             case -1: // 右子树比左子树高，在左子树中插入，树的高度不变，当前结点的平衡因子变为0
    //                 T->bf = 0;
    //                 taller = false;
    //                 break;
    //             }
    //         }
    //     }
    //     else
    //     { // ③
    //         if (T->rightC == NULL)
    //         { // 检查右子树
    //             bstNode<K> *q = new bstNode<K>{val, 0, NULL, NULL};
    //             T->rightC = q;
    //             switch (T->bf)
    //             {
    //             case 0:
    //                 T->bf = -1;
    //                 taller = true;
    //                 break;
    //             case 1:
    //                 T->bf = 0;
    //                 taller = false;
    //                 break;
    //             }
    //             return true;
    //         }
    //         nodeDelete(T->rightC, val);
    //         if (taller)
    //         {
    //             switch (T->bf)
    //             {
    //             case 1:
    //                 T->bf = 0;
    //                 taller = false;
    //                 break;
    //             case 0:
    //                 T->bf = -1;
    //                 taller = true;
    //                 break;
    //             case -1:
    //                 rightHigh(T);
    //                 taller = false;
    //                 break;
    //             }
    //         }
    //     }
    //     return true;
    // }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    AVLTree<int> *a = new AVLTree<int>(80);
    a->treeInsert(70);
    a->treeInsert(60);
    a->treeInsert(50);
    cout << a->root->leftC->leftC->value << '\n';
    return 0;
}