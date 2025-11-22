#include <iostream>
#include <map>
#include <string>
using namespace std;

// ---------------------------
// Node: یک گره از لیست دوطرفه
// ---------------------------
struct Node {
    Node* next;   // اشاره‌گر به گره بعدی
    Node* prev;   // اشاره‌گر به گره قبلی
    int value;    // مقدار ذخیره‌شده در کش
    int key;      // کلید مربوط به این گره

    Node(int k, int val)
        : next(NULL), prev(NULL), value(val), key(k) {}
};


// ---------------------------
// کلاس پایه Cache
// ---------------------------
class Cache {
protected:
    map<int, Node*> mp;   // map: کلید → اشاره‌گر به گره در لیست
    int cp;               // ظرفیت کش
    Node* head;           // جدیدترین گره (Most Recently Used)
    Node* tail;           // قدیمی‌ترین گره (Least Recently Used)

    // توابعی که باید در کلاس‌های فرزند پیاده‌سازی شوند
    virtual void set(int, int) = 0;
    virtual int get(int) = 0;
};


// ---------------------------
// LRU Cache — نسخهٔ تمیزتر با توابع کمکی
// ---------------------------
class LRUCache : public Cache {
public:
    // سازنده: مقداردهی ظرفیت و خالی‌بودن لیست
    LRUCache(int capacity) {
        cp = capacity;
        head = NULL;
        tail = NULL;
    }

    // ---------------------------
    // تابع Set: قرار دادن / به‌روزرسانی مقدار
    // ---------------------------
    void set(int key, int value) override {
        // اگر کلید از قبل وجود داشته باشد → مقدار را آپدیت و گره را به head منتقل کن
        if (mp.find(key) != mp.end()) {
            Node* node = mp[key];
            node->value = value;
            moveToHead(node);  // چون استفاده شد، جدیدترین می‌شود
            return;
        }

        // اگر کلید جدید است:
        Node* node = new Node(key, value);

        // اگر ظرفیت پر شده → یک گره از انتها (tail) حذف می‌کنیم
        if ((int)mp.size() >= cp) {
            Node* removed = removeTail();     // حذف از لیست
            mp.erase(removed->key);           // حذف از map
            delete removed;                   // آزاد کردن حافظه
        }

        // اضافه کردن گره جدید به map و لیست
        mp[key] = node;
        addToHead(node);   // گره جدید همیشه تازه‌ترین است
    }

    // ---------------------------
    // تابع Get: گرفتن مقدار یک کلید
    // ---------------------------
    int get(int key) override {
        // اگر کلید پیدا نشود → cache miss
        if (mp.find(key) == mp.end()) {
            return -1;
        }

        // پیدا کردن گره و انتقالش به head چون تازه استفاده شد
        Node* node = mp[key];
        moveToHead(node);

        // برگرداندن مقدار
        return node->value;
    }

private:
    // ---------------------------
    // addToHead: اضافه کردن گره به ابتدای لیست
    // ---------------------------
    void addToHead(Node* node) {
        node->prev = NULL;
        node->next = head;

        if (head != NULL) {
            head->prev = node;
        }

        head = node;

        // اگر لیست قبلاً خالی بود، tail هم باید همین گره باشد
        if (tail == NULL) {
            tail = node;
        }
    }

    // ---------------------------
    // removeNode: جدا کردن یک گره از لیست
    // (بدون delete کردن آن)
// ---------------------------

    void removeNode(Node* node) {
        // اگر گره قبلی دارد، next گره قبلی را تنظیم کن
        if (node->prev != NULL) {
            node->prev->next = node->next;
        } else {
            // اگر prev ندارد یعنی خودش head است
            head = node->next;
        }

        // اگر گره بعدی دارد، prev گره بعدی را تنظیم کن
        if (node->next != NULL) {
            node->next->prev = node->prev;
        } else {
            // اگر next ندارد یعنی خودش tail است
            tail = node->prev;
        }

        // گره اکنون از لیست جدا شده
        node->prev = NULL;
        node->next = NULL;
    }

    // ---------------------------
    // moveToHead: بردن یک گره به ابتدای لیست
    // ---------------------------
    void moveToHead(Node* node) {
        // اگر همین الان head است، نیازی به کاری نیست
        if (node == head) return;

        // ابتدا از جای قبلی جداش کن
        removeNode(node);

        // بعد به ابتدای لیست اضافه‌اش کن
        addToHead(node);
    }

    // ---------------------------
    // removeTail: حذف گره tail از لیست
    // و برگرداندن اشاره‌گر به آن
    // ---------------------------
    Node* removeTail() {
        if (tail == NULL) return NULL;  // اگر لیست خالی باشد

        Node* oldTail = tail;

        // اگر فقط یک گره در لیست است
        if (head == tail) {
            head = NULL;
            tail = NULL;
        } else {
            // اگر بیش از یک گره است
            tail = tail->prev;
            tail->next = NULL;
        }

        // oldTail هنوز زنده است؛ delete بیرون از تابع انجام می‌شود
        return oldTail;
    }
};


// ---------------------------
// برنامهٔ اصلی برای تست
// ---------------------------
int main() {
    int n, capacity, i;
    cin >> n >> capacity;

    LRUCache l(capacity);

    for (i = 0; i < n; i++) {
        string command;
        cin >> command;

        if (command == "get") {
            int key;
            cin >> key;
            cout << l.get(key) << endl;
        } 
        else if (command == "set") {
            int key, value;
            cin >> key >> value;
            l.set(key, value);
        }
    }

    return 0;
}
