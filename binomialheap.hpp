#ifndef BINOMIALHEAP_H_
#define BINOMIALHEAP_H_


#define DEBUG false
#include <iostream>

template <typename B>
class BinomialNode;

template <typename B>
class BinomialHeap
{

public:

	friend class BinomialNode<B>;

	BinomialHeap();
	BinomialHeap(const BinomialHeap& other);
	BinomialHeap& operator=(const BinomialHeap& other);
	~BinomialHeap();

	void insert(B value);
	B getMin();
	B extractMin();
	 void deleteKey(B value);
	 void decreaseKey(B value, B newValue);
	 int size();
	 bool isEmpty();
	  void clear();
protected:
	static BinomialNode<B>* unionHeap(BinomialNode<B>* heapA, BinomialNode<B>* heapB);
	static BinomialNode<B>* mergeHeap(BinomialNode<B>* heapA, BinomialNode<B>* heapB);
	BinomialNode<B>* copyHeap(BinomialNode<B>* node, BinomialNode<B>* parent);
	BinomialNode<B>* findKey(B value);
	static int order(BinomialNode<B>* heap);

private:
	BinomialNode<B>* head;
};

template <typename B>
class BinomialNode
{
public:
	friend class BinomialHeap<B>;
	BinomialNode(B v, BinomialNode<B>* p = nullptr);

	void addChild(BinomialNode<B>* child);
	BinomialNode<B>* find(B v);
	int size(int& i);
	void addTo(BinomialHeap<B>* other);
private:
	B value;
	BinomialNode<B>* sibling, * child, * parent;
	int order;
};
#endif /* BINOMIALHEAP_H_ */