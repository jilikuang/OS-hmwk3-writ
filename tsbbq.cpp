const int MAX = 10;
const int BOUND = 10;

class BBQ {
	Lock lock;
	CV itemAdded;
	CV itemRemoved;

	int items[MAX];
	int front;
	int nextEmpty;
	int insertNum;
	int removeNum

public:
	BBQ();
	~BBQ() {};
	void insert(int item);
	int remove();
};

BBQ::BBQ() {
	front = nextEmpty = 0;
}

void BBQ::insert(int item) {
	lock.require();
	while (((nextEmpty - front) == MAX) ||
			(insertNum > BOUND)) {
		itemRemoved.wait(&lock);
	}
	items[nextEmpty % MAX] = item;
	nextEmpty++;
	insertNum++;
	if (removeNum > BOUND)
		removeNum = 0;
	itemAdded.signal();
	lock.release();
}

int BBQ::remove() {
	int item;

	lock.acquire();
	while ((front == nextEmpty) ||
			(removeNum > BOUND)) {
		itemAdded.wait(&lock);
	}
	item = items[front % MAX];
	front++;
	removeNum++;
	if (insertNum > BOUND)
		insertNum = 0;
	itemRemoved.signal();
	lock.release();
	return item;
}
