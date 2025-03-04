#include <config/WukConfig.hh>
#include <config/WukException.hh>

#include <WukRandom.cc>
#include <WukTime.cc>

#include <iostream>

class LinkedList {
public:
    wSize seq;
    wSize val;
    LinkedList *next;

    LinkedList() : seq(0), val(0), next(nullptr) {}
    LinkedList(wSize val) : seq(0), val(val), next(nullptr) {}
};

LinkedList *link_new(wSize length)
{
    if (!length) {
        throw wuk::Exception(wuk::Error::ERR, "link_new",
            "length is 0.");
    }

    LinkedList *head = nullptr;
    LinkedList *node = nullptr;

    for(wSize i = 0; i < length; ++i) {
        if (!head) {
            head = new LinkedList;
            head->seq = i;
            node = head;
        }
        node->next = new LinkedList;
        node->seq = i;
        node = node->next;
    }

    node->next = nullptr;
    return head;
}

void link_free(LinkedList *ll)
{
    while(ll) {
        LinkedList *p = ll;
        ll = ll->next;
        delete p;
    }
}

int main()
{
    wuk::Random rad;
    LinkedList *node = nullptr;
    LinkedList *ptr = nullptr;
    wSize length = 5;

    node = link_new(length);
    ptr = node;

    for(wSize i = 0; i < length; ++i) {
        ptr->val = rad.randint(1, 65536);
        ptr = ptr->next;
    }

    ptr = node;
    for(wSize i = 0; i < length; ++i) {
        std::cout << "link" << i << " val: " << ptr->val << std::endl;
        ptr = ptr->next;
    }

    link_free(node);
    return 0;
}
