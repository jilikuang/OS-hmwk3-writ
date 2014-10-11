const int MAX = 10;

class BBQ {
	Lock lock;
	CV itemAdded;
	CV itemRemoved;

	int items[MAX];
	int front;
	int nextEmpty;

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
	while ((nextEmpty - front) == MAX) {
		itemRemoved.wait(&lock);
	}
	items[nextEmpty % MAX] = item;
	nextEmpty++;
	itemAdded.signal();
	lock.release();
}

int BBQ::remove() {
	int item;

	lock.acquire();
	while (front == nextEmpty) {
		itemAdded.wait(&lock);
	}
	item = items[front % MAX];
	front++;
	itemRemoved.signal();
	lock.release();
	return item;
}
