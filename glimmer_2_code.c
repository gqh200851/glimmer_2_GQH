/* ============================================================
 * glimmer_2_code.c
 * 来源：2.md 的「Step2. 链操作 / Part 4 链操作的实现」部分
 * 说明:  文件头补充了 #include 与 Node 结构体定义（它们在 2.md
 *       的链操作部分之前），以便本文件可以独立编译。       
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

/* 单向链表节点（来自 2.md Part 3） */
typedef struct Node
{
    int data;          // 数据域：存一个整数
    struct Node *next; // 指针域：存下一个节点的地址
} Node;

/* 1）创建节点的函数 */
Node* createNode(int n)
{
    Node *newNode = (Node*)malloc(sizeof(Node));  // 向内存申请一个节点的空间
    newNode->data = n;      // 数据域存 n
    newNode->next = NULL;   // 新节点后面暂时没有节点
    return newNode;
}

/* 2）头插函数 */
Node* headInsert(Node *head, int n)
{
    Node *newNode = createNode(n);
    newNode->next = head;   // 新节点指向原来的第一个节点
    head = newNode;         // 头指针指向新节点
    return head;            // 头指针变了，所以要把新的头指针返回
}

/* 3）尾插函数 */
Node* tailInsert(Node *head, int n)
{
    Node *newNode = createNode(n);
    Node *p = head;
    while (p->next != NULL)  // 只要后面还有节点，就一直往后走
    {
        p = p->next;
    }
    p->next = newNode;       // 走到最后一个节点，把新节点接在它后面
    return head;
}

/* 4）打印链表函数（遍历，且不改变头指针） */
void printList(Node *head)
{
    Node *p = head;
    while (p != NULL)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

/* 5）查找函数 */
int findNode(Node *head, int n)
{
    Node *p = head;
    int count = 0;
    while (p != NULL)
    {
        count++;                  // 每经过一个节点，计数加 1
        if (p->data == n)
        {
            return count;         // 找到了，返回是第几个节点
        }
        p = p->next;
    }
    return 0;                     // 走完都没找到，返回 0（false）
}

/* 6）删除第 n 个节点的函数（双指针思想）——替身法版本 */
int deleteNode1(Node *head, int n)
{
    Node *pre = NULL;   // 用pre指向p的前一个节点
    Node *p = head;     // 用p指向当前节点（最终指向要删除的节点）
    int count = 1;

    while (p != NULL)
    {
        if (count == n)
        {
            if (pre == NULL)
            {
                // 要删的是第一个节点。题目只让传头指针，没法改变外面 head 的指向，
                // 所以用“替身”的办法：把第二个节点的数据搬到第一个，再删掉第二个节点，
                // 而不能直接把head给删了。
                Node *nextNode = p->next;
                if (nextNode == NULL)
                {
                    return 0;   // 链表只有一个节点，没法删
                }
                p->data = nextNode->data;
                p->next = nextNode->next;
                free(nextNode);
                return 1;
            }
            pre->next = p->next;   // 前一个节点绕过要删除的节点
            free(p);               // 释放要删除的节点
            return 1;
        }
        pre = p;
        p = p->next;
        count++;        // 没遍历完就一直向下挪动双指针
    }
    return 0;   // 能走到这里，说明节点数少于 n
}

/* 探索版：允许返回新头指针的删除（有缺陷，见 2.md 说明） */
Node* deleteNode2(Node *head, int n)
{
    Node *pre = NULL;
    Node *p = head;
    int count = 1;

    if (n == 1)              // 删第一个节点：直接删，返回新的头
    {
        Node *newHead = p->next;
        free(p);
        return newHead;      // 新头 = 原第二个节点（链表只有一个节点时返回 NULL）
    }

    while (p != NULL)        // 删非第一个节点：还是老套路
    {
        if (count == n)
        {
            pre->next = p->next;
            free(p);
            return head;     // 头没变，返回原 head
        }
        pre = p;
        p = p->next;
        count++;
    }
    return head;             // 没删到，头也没变
}

/* 探索版：用结构体同时带回 头指针 和 成功标志 */
typedef struct DeleteResult
{
    Node *head;    // 执行完删除操作后的头指针（带回删除后的newHead或没删时的原head）
    int ok;        // 1 表示删除成功（true），0 表示节点数少于 n（false）
} DeleteResult;

DeleteResult deleteNode3(Node *head, int n)
{
    DeleteResult result;
    
    Node *pre = NULL;
    Node *p = head;
    int count = 1;

    if (n == 1)     // 删第一个节点：直接删，新头带出去
    {
        Node *newHead = p->next;
        free(p);
        
        result.head = newHead; // 新头 = 原第二个节点（单节点时是 NULL）
        result.ok = 1;
        return result;
    }

    while (p != NULL)
    {
        if (count == n)
        {
            pre->next = p->next;
            free(p);
            result.head = head;   // 头没变，原样带回
            result.ok = 1;
            return result;
        }
        pre = p;
        p = p->next;
        count++;
    }

    result.head = head;   // 节点数少于 n，没删到
    result.ok = 0;        // 返回 false
    return result;
}

/* 探索版：二级指针版本 */
int deleteNode(Node **head, int n)   // 形参变成“指向头指针的指针”
{
    Node *pre = NULL;
    Node *p = *head;                 // p 指向当前节点
    int count = 1;

    while (p != NULL)
    {
        if (count == n)
        {
            if (pre == NULL)
            {
                *head = p->next;     // 直接改外部 head，指向第二个节点
            }
            else
            {
                pre->next = p->next; // 非第一个节点，老套路
            }
            free(p);
            return 1;                // 删除成功 → true
        }
        pre = p;
        p = p->next;
        count++;
    }
    return 0;                        // 节点数少于 n → false
}

/* 7）反转函数——数组版 (纯自己写的那坨的版本)（有固定上限 666） */
Node *Reverse(Node *head)
{
    Node *pre = NULL;
    Node *p = head;
    int count = 0;
    Node* arr[666];

    while (p != NULL)
    {        
        pre = p;
        p = p->next;
        arr[count] = pre;
        count++;
    }// 先遍历并找到链表的末节点（此时*pre正指向末节点）
    

    head = arr[count - 1];

    int i = 1;
    while (count - i > 0)
    {
        arr[count - i]->next = arr[count - i - 1];
        i++;
    }               // 然后从尾部到头部反转除头指针外的链表
    
    arr[0]->next = NULL;

    return head;
}

/* 7）反转函数——三指针版（优化版） */
Node* reverseList(Node *head)
{
    Node *pre = NULL;        // 上一个节点，一开始为空
    Node *p = head;          // 当前节点
    Node *nextNode = NULL;   // 用来暂存下一个节点

    while (p != NULL)
    {
        nextNode = p->next;  // 先记住下一个节点，不然改完指针就找不回来了
        p->next = pre;       // 把当前节点的指向倒过来，指向上一个节点
        pre = p;             // 上一个节点往前走一步
        p = nextNode;        // 当前节点往前走一步
    }
    head = pre;              // 循环结束，pre 就是新的第一个节点
    return head;
}
