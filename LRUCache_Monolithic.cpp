#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

// ---------------------------
// Node: یک گره از لیست دوطرفه
// ---------------------------
struct Node{
   Node* next;   // اشاره‌گر به گره بعدی
   Node* prev;   // اشاره‌گر به گره قبلی
   int value;    // مقدار ذخیره‌شده
   int key;      // کلید مربوط به این گره (برای پاک کردن از map لازم است)

   // سازنده با اشاره به قبلی و بعدی
   Node(Node* p, Node* n, int k, int val)
       :prev(p), next(n), key(k), value(val){};

   // سازنده ساده (برای گره جدید)
   Node(int k, int val)
       :prev(NULL), next(NULL), key(k), value(val){};
};


// ---------------------------
// کلاس پایه کش
// ---------------------------
class Cache{
protected:
   map<int,Node*> mp;   // map کلید → اشاره‌گر به گره
   int cp;              // ظرفیت کش
   Node* tail;          // اشاره‌گر به قدیمی‌ترین گره
   Node* head;          // اشاره‌گر به جدیدترین گره

   // دو تابع باید توسط کلاس‌های فرزند پیاده‌سازی شود
   virtual void set(int, int) = 0;
   virtual int get(int) = 0;
};


// ---------------------------
// LRU Cache — پیاده‌سازی کامل
// ---------------------------
class LRUCache: public Cache
{
public:

    // سازنده: مقداردهی اولیه ظرفیت و خالی‌بودن لیست
    LRUCache(int capacity)
    {
        cp = capacity;
        tail = NULL;
        head = NULL;
    }
    
    // ---------------------------
    // تابع Set (قرار دادن مقدار)
    // ---------------------------
    void set(int key, int value) override
    {
        // اگر کلید از قبل وجود داشته باشد → آپدیت مقدار و انتقال به head
        if(mp.find(key) != mp.end())
        {
            Node* node = mp[key];
            node->value = value;    // مقدار جدید

            // اگر خودش head است نیازی به جابه‌جایی نیست
            if(node == head) return;
            
            // جدا کردن گره از جای قبلی‌اش
            node->prev->next = node->next;

            // اگر گره وسط بود
            if(node->next != NULL)
            {
                node->next->prev = node->prev;
            } 
            else {
                // اگر tail بود → tail جدید می‌شود گره قبلی
                tail = node->prev;
            }
            
            // انتقال گره به head (جدیدترین)
            node->next = head;
            node->prev = NULL;
            head->prev = node;
            head = node;
        }

        // ---------------------------
        // اگر کلید جدید باشد
        // ---------------------------
        else {
            
            // ساختن گره جدید
            Node* node = new Node(NULL, NULL, key, value);
            
            // اگر ظرفیت پر شده باشد → حذف قدیمی‌ترین گره (tail)
            if(mp.size() >= cp)
            {
                Node* oldTail = tail;         // ذخیره برای حذف
                mp.erase(oldTail->key);       // حذف از map

                // اگر فقط یک گره در لیست باشد
                if(head == tail)
                {
                    head = NULL;
                    tail = NULL;
                } 
                else {
                    // تغییر tail به گره قبلی
                    tail = tail->prev;
                    tail->next = NULL;
                }

                delete oldTail;               // آزاد کردن حافظه
            }
            
            // اضافه کردن کلید جدید به map
            mp[key] = node;
            
            // اگر لیست خالی باشد → head و tail همین گره می‌شوند
            if(head == NULL){
                head = node;
                tail = node;
            } 
            else {
                // اضافه کردن گره جدید به ابتدای لیست (head)
                node->next = head;
                head->prev = node;
                head = node;
            }
        }
    }
    
    // ---------------------------
    // تابع Get (دریافت مقدار)
    // ---------------------------
    int get(int key) override
    {
        // اگر کلید وجود ندارد → مقدار -1 (cache miss)
        if(mp.find(key) == mp.end())
        {
           return -1;
        }
        
        Node* node = mp[key];

        // اگر این گره head نیست، باید جابه‌جا شود به head
        if(node != head)
        {
            // جدا کردن از موقعیت فعلی
            node->prev->next = node->next;
            
            // اگر وسط لیست بود
            if (node->next != NULL) {
                node->next->prev = node->prev;
            } 
            else {
                // اگر tail بود → tail جدید گره قبلی‌اش می‌شود
                tail = node->prev;
            }
            
            // انتقال گره به head
            node->next = head;
            node->prev = NULL;
            head->prev = node;
            head = node;
        }

        // مقدار گره را برمی‌گرداند
        return node->value;
    }
};


// ---------------------------
// برنامه اصلی
// ---------------------------
int main() {
   int n, capacity, i;
   cin >> n >> capacity;

   LRUCache l(capacity);  // ساخت کش با ظرفیت موردنظر

   for(i=0;i<n;i++) {
      string command;
      cin >> command;

      if(command == "get") {
         int key;
         cin >> key;
         cout << l.get(key) << endl;   // چاپ مقدار
      } 
      else if(command == "set") {
         int key, value;
         cin >> key >> value;
         l.set(key,value);             // ذخیره مقدار
      }
   }

   return 0;
}
